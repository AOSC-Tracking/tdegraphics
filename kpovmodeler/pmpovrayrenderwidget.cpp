/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2003 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmpovrayrenderwidget.h"
#include "pmdefaults.h"
#include "pmdebug.h"
#include "pmdragwidget.h"

#include <kprocess.h>
#include <tdemessagebox.h>
#include <tdelocale.h>
#include <tdeconfig.h>
#include <kurl.h>
#include <tdetempfile.h>
#include <tqcolor.h>
#include <tqpainter.h>
#include <tqtextstream.h>
#include <tqdragobject.h>

#ifdef KDE_NO_COMPAT
#undef KDE_NO_COMPAT
#endif

#include <tdeapplication.h>

TQString PMPovrayRenderWidget::s_povrayCommand = c_defaultPovrayCommand;
TQStringList PMPovrayRenderWidget::s_libraryPaths;

PMPovrayRenderWidget::PMPovrayRenderWidget( TQWidget* parent, const char* name )
      : PMDragWidget( parent, name )
{
   m_pProcess = 0;
   m_bSuspended = false;
   m_rcvHeader = false;
   m_skipBytes = 0;
   m_bPixmapUpToDate = false;
   m_pTempFile = 0;

   setBackgroundColor( TQColor( 0, 0, 0 ) );
}

PMPovrayRenderWidget::~PMPovrayRenderWidget( )
{
   cleanup( );
}

bool PMPovrayRenderWidget::render( const TQByteArray& scene,
                                   const PMRenderMode& m,
                                   const KURL& documentURL )
{
   cleanup( );

   m_povrayOutput = "";
   m_renderMode = m;

   if( !scene.data( ) )
   {
      KMessageBox::sorry( this, i18n( "Can't render an empty scene.\n" ) );
      return false;
   }

   // output to tmp file
   m_pTempFile = new KTempFile( TQString(), ".pov" );
   TQDataStream* dstr = m_pTempFile->dataStream( );

   if( ( m_pTempFile->status( ) != 0 ) || !dstr )
   {
      KMessageBox::sorry( this, i18n( "Couldn't write the scene to a temp file.\n" ) );
      return false;
   }

   dstr->writeRawBytes( scene.data( ), scene.size( ) );
   m_pTempFile->close( );

   m_pProcess = new TDEProcess( );
   connect( m_pProcess, TQT_SIGNAL( receivedStdout( TDEProcess*, char*, int ) ),
            TQT_SLOT( slotPovrayImage( TDEProcess*, char*, int ) ) );
   connect( m_pProcess, TQT_SIGNAL( receivedStderr( TDEProcess*, char*, int ) ),
            TQT_SLOT( slotPovrayMessage( TDEProcess*, char*, int ) ) );
   connect( m_pProcess, TQT_SIGNAL( processExited( TDEProcess* ) ),
            TQT_SLOT( slotRenderingFinished( TDEProcess* ) ) );

   *m_pProcess << s_povrayCommand;

   TQStringList::ConstIterator it;
   TQStringList args = m_renderMode.commandLineSwitches( );
   for( it = args.begin( ); it != args.end( ); ++it )
      *m_pProcess << *it;

   for( it = s_libraryPaths.begin( ); it != s_libraryPaths.end( ); ++it )
   {
      TQString path = *it;
      if( path != TQString( "/" ) )
         if( path.right( 1 ) == TQString( "/" ) )
            path.truncate( path.length( ) - 1 );
      *m_pProcess << ( TQString( "+L" ) + path );
   }
   *m_pProcess << TQString( "+I" ) + m_pTempFile->name( ) << "+O-" << "+FT"
               << "+K0.0" << "+KFI1" << "+KFF1" << "+KI0.0" << "+KF0.0"
               << "+SF1" << "+EF1" << "-KC" << "-D";

#if ( ( TDE_VERSION_MAJOR == 2 ) && ( TDE_VERSION_MINOR >= 9 ) ) || ( TDE_VERSION_MAJOR == 3 )
   if( !documentURL.isEmpty( ) && documentURL.isLocalFile( ) )
      m_pProcess->setWorkingDirectory( documentURL.directory( ) );
#endif

   m_rcvHeader = true;
   m_rcvHeaderBytes = 0;
   m_rcvPixels = 0;
   m_progress = 0;
   m_numRestBytes = 0;
   m_line = 0;
   m_column = 0;
   m_skipBytes = 0;

   int width = m_renderMode.width( );
   int height = m_renderMode.height( );

   m_image.create( width, height, 32 );
   m_image.setAlphaBuffer( m_renderMode.alpha( ) );
   m_image.fill( tqRgb( 0, 0, 0 ) );
   m_bPixmapUpToDate = false;
   repaint( );

   if( !m_pProcess->start( TDEProcess::NotifyOnExit, TDEProcess::AllOutput ) )
   {
      KMessageBox::error( this, i18n( "Couldn't call povray.\n"
                                      "Please check your installation "
                                      "or set another povray command." ) );
      delete m_pProcess;
      m_pProcess = 0;
      return false;
   }

   m_bSuspended = false;
   return true;
}

void PMPovrayRenderWidget::killRendering( )
{
   if( m_pProcess )
   {
      if( m_bSuspended )
         m_pProcess->kill( SIGCONT );
      m_bSuspended = false;
      m_pProcess->kill( );
   }
}

void PMPovrayRenderWidget::suspendRendering( )
{
   if( m_pProcess )
   {
      m_bSuspended = true;
      m_pProcess->kill( SIGSTOP );
   }
}

void PMPovrayRenderWidget::resumeRendering( )
{
   if( m_pProcess )
   {
      m_pProcess->kill( SIGCONT );
      m_bSuspended = false;
   }
}

void PMPovrayRenderWidget::slotPovrayMessage( TDEProcess*,
                                              char* buffer, int buflen )
{
   TQString str;
   str.setLatin1( buffer, buflen );
   m_povrayOutput += str;
   emit povrayMessage( str );
}

void PMPovrayRenderWidget::slotPovrayImage( TDEProcess*, char* buffer, int buflen )
{
   int index = 0;
   int i;
   int oldLine = m_line;

   if( m_rcvHeader )
   {
      // receive targa header
      while( ( m_rcvHeaderBytes < 18 ) && ( index < buflen ) )
      {
         m_header[m_rcvHeaderBytes] = ( unsigned char ) buffer[index];
         m_rcvHeaderBytes++;
         index++;
      }

      if( m_rcvHeaderBytes == 18 )
      {
         // complete targa header received
         m_rcvHeader = false;
         m_skipBytes = m_header[0]; // id length
         m_bytespp = m_header[16] / 8;
      }
   }

   if( m_skipBytes > 0 )
   {
      int skip = buflen - index;
      if( skip > m_skipBytes )
         skip = m_skipBytes;
      m_skipBytes -= skip;
      index += skip;
   }

   if( ( m_numRestBytes > 0 ) && ( index < buflen ) )
   {
      while( ( m_numRestBytes < m_bytespp ) && ( index < buflen ) )
      {
         m_restBytes[m_numRestBytes] = ( unsigned char ) buffer[index];
         index++;
         m_numRestBytes++;
      }
      if( m_numRestBytes == m_bytespp )
      {
         m_numRestBytes = 0;

         if( m_bytespp == 4 )
            setPixel( m_column, m_line,
                      tqRgba( m_restBytes[2], m_restBytes[1],
                             m_restBytes[0], m_restBytes[3] ) );
         else
            setPixel( m_column, m_line,
                      tqRgb( m_restBytes[2], m_restBytes[1], m_restBytes[0] ) );

         m_column++;
         m_rcvPixels++;
         if( m_column == m_renderMode.width( ) )
         {
            m_column = 0;
            m_line++;
         }
      }
   }

   if( index < buflen )
   {
      int num = ( buflen - index ) / m_bytespp;
      for( i = 0; i < num; i++ )
      {
         if( m_bytespp == 4 )
            setPixel( m_column, m_line,
                      tqRgba( buffer[index+2], buffer[index+1],
                             buffer[index], buffer[index+3] ) );
         else
            setPixel( m_column, m_line,
                      tqRgb( buffer[index+2], buffer[index+1],
                            buffer[index] ) );
         index += m_bytespp;

         m_column++;
         m_rcvPixels++;
         if( m_column == m_renderMode.width( ) )
         {
            m_column = 0;
            m_line++;
         }
      }
   }

   if( index < buflen )
   {
      m_numRestBytes = buflen - index;
      for( i = 0; i < m_numRestBytes; i++ )
      {
         m_restBytes[i] = buffer[index];
         index++;
      }
   }

   if( m_line != oldLine )
   {
      TQPainter paint( this );
      int offset = 0;
      if( m_renderMode.subSection( ) )
      {
         double sr = m_renderMode.startRow( );
         if( sr < 1 )
            offset = ( int ) ( m_renderMode.height( ) * sr + 0.5 );
         else
            offset += ( int ) sr;
      }
      paint.drawImage( 0, offset + oldLine,
                       m_image.copy( 0, offset + oldLine, m_image.width( ), offset + m_line - oldLine ) );

      emit lineFinished( m_line - 1 );
   }

   int oldProgress = m_progress;
   int numPixels = 0;
   if( m_renderMode.subSection( ) )
   {
      int sr = 0;
      if( m_renderMode.startRow( ) < 1 )
         sr = ( int ) ( m_renderMode.height( ) * m_renderMode.startRow( ) + 0.5 );
      else
         sr = ( int ) m_renderMode.startRow( );
      int er = 0;
      if( m_renderMode.endRow( ) < 1 )
         er = ( int ) ( m_renderMode.height( ) * m_renderMode.endRow( ) + 0.5 );
      else
         er = ( int ) m_renderMode.endRow( );

      numPixels = m_renderMode.width( ) * ( er - sr );
   }
   else
      numPixels = m_renderMode.width( ) * m_renderMode.height( );

   m_progress = m_rcvPixels * 100 / numPixels;

   if( m_progress != oldProgress )
      emit progress( m_progress );
   m_bPixmapUpToDate = false;
}

void PMPovrayRenderWidget::setPixel( int x, int y, uint c )
{
   if( m_renderMode.subSection( ) )
   {
      double sr = m_renderMode.startRow( );
      if( sr < 1 )
         y += ( int ) ( m_renderMode.height( ) * sr + 0.5 );
      else
         y += ( int ) sr;
   }

   if( x >= 0 && x < m_image.width( ) &&
       y >= 0 && y < m_image.height( ) )
      m_image.setPixel( x, y, c );
}

/**
void PMPovrayRenderWidget::slotWroteStdin( TDEProcess* )
{
   if( m_pProcess )
      m_pProcess->closeStdin( );
   m_data.resize( 0 );
}
*/

void PMPovrayRenderWidget::slotRenderingFinished( TDEProcess* )
{
   if( m_pProcess->normalExit( ) )
      emit( finished( m_pProcess->exitStatus( ) ) );
   else
      emit( finished( -1000 ) );

   cleanup( );
}

void PMPovrayRenderWidget::paintEvent( TQPaintEvent* ev )
{
   if( !m_bPixmapUpToDate )
   {
      if( !m_image.isNull( ) )
         m_pixmap.convertFromImage( m_image );
      m_bPixmapUpToDate = true;
   }
   bitBlt( this, ev->rect( ).left( ), ev->rect( ).top( ),
           &m_pixmap, ev->rect( ).left( ), ev->rect( ).top( ),
           ev->rect( ).width( ), ev->rect( ).height( ), CopyROP );
}

void PMPovrayRenderWidget::cleanup( )
{
   if( m_pProcess )
      delete m_pProcess;
   m_pProcess = 0;
   if( m_pTempFile )
   {
      m_pTempFile->unlink( );
      delete m_pTempFile;
   }
   m_pTempFile = 0;
}

TQSize PMPovrayRenderWidget::sizeHint( ) const
{
   TQSize s;
   if( m_image.isNull( ) )
      s = TQSize( 200, 200 );
   else
      s = m_image.size( );

   return s.expandedTo( minimumSize( ) );
}

void PMPovrayRenderWidget::saveConfig( TDEConfig* cfg )
{
   cfg->setGroup( "Povray" );
#if ( ( TDE_VERSION_MAJOR == 3 ) && ( TDE_VERSION_MINOR <= 1 ) )
   cfg->writeEntry( "PovrayCommand", s_povrayCommand );
   cfg->writeEntry( "LibraryPaths", s_libraryPaths );
#else
   cfg->writePathEntry( "PovrayCommand", s_povrayCommand );
   cfg->writePathEntry( "LibraryPaths", s_libraryPaths );
#endif
}

void PMPovrayRenderWidget::restoreConfig( TDEConfig* cfg )
{
   cfg->setGroup( "Povray" );
#if ( ( TDE_VERSION_MAJOR == 3 ) && ( TDE_VERSION_MINOR <= 1 ) )
   s_povrayCommand = cfg->readEntry( "PovrayCommand", s_povrayCommand );
   s_libraryPaths = cfg->readListEntry( "LibraryPaths" );
#else
   s_povrayCommand = cfg->readPathEntry( "PovrayCommand", s_povrayCommand );
   s_libraryPaths = cfg->readPathListEntry( "LibraryPaths" );
#endif
}

void PMPovrayRenderWidget::startDrag( )
{
   TQImageDrag* d = new TQImageDrag( m_image, this );
   d->dragCopy( );
}

#include "pmpovrayrenderwidget.moc"
