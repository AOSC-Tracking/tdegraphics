/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Andreas Zehender
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

#include "pmvectorlistedit.h"
#include "pmdebug.h"

#include <tqheader.h>
#include <tqapplication.h>
#include <palette.h>
#include <tqpainter.h>
#include <kmessagebox.h>
#include <klocale.h>

const int c_columnWidth = 65;


/*
PMTableItem::PMTableItem( TQTable* table )
      : TQTableItem( table, TQTableItem::OnTyping, TQString( "" ) )
{
   m_readOnly = false;
}

void PMTableItem::paint( TQPainter* p, const TQColorGroup& cg,
                         const TQRect& cr, bool selected )
{
   p->fillRect( 0, 0, cr.width( ), cr.height( ),
                selected ? cg.brush( TQColorGroup::Highlight )
                : ( m_readOnly ? cg.brush( TQColorGroup::Background ) :
                    cg.brush( TQColorGroup::Base ) ) );

   int w = cr.width( );
   int h = cr.height( );

   int x = 0;

   if( selected )
      p->setPen( cg.highlightedText( ) );
   else
      p->setPen( cg.text( ) );
   p->drawText( x + 2, 0, w - x - 4, h,
                wordWrap( ) ? ( alignment( ) | WordBreak ) : alignment( ), text( ) );
}
*/

PMVectorListEdit::PMVectorListEdit( TQWidget* parent, const char* name )
      : TQTable( 1, 3, parent, name )
{
   init( 3 );

   TQHeader* header = horizontalHeader( );
   header->setLabel( 0, "x" );
   header->setLabel( 1, "y" );
   header->setLabel( 2, "z" );
}

PMVectorListEdit::PMVectorListEdit( const TQString& c1, const TQString& c2,
                                    const TQString& c3, const TQString& c4,
                                    TQWidget* parent, const char* name )
      : TQTable( 1, 4, parent, name )
{
   init( 4 );

   TQHeader* header = horizontalHeader( );
   header->setLabel( 0, c1 );
   header->setLabel( 1, c2 );
   header->setLabel( 2, c3 );
   header->setLabel( 3, c4 );
}

PMVectorListEdit::PMVectorListEdit( const TQString& c1, const TQString& c2,
                                    const TQString& c3, TQWidget* parent,
                                    const char* name )
      : TQTable( 1, 3, parent, name )
{
   init( 3 );

   TQHeader* header = horizontalHeader( );
   header->setLabel( 0, c1 );
   header->setLabel( 1, c2 );
   header->setLabel( 2, c3 );
}

PMVectorListEdit::PMVectorListEdit( const TQString& c1, const TQString& c2,
                                    TQWidget* parent, const char* name )
      : TQTable( 1, 2, parent, name )
{
   init( 2 );

   TQHeader* header = horizontalHeader( );
   header->setLabel( 0, c1 );
   header->setLabel( 1, c2 );
}

void PMVectorListEdit::init( int dimensions )
{
   int i;

   m_dimension = dimensions;
   m_size = 0;

   horizontalHeader( )->setResizeEnabled( false );
   verticalHeader( )->setResizeEnabled( false );

   setSelectionMode( TQTable::MultiRow );
   for( i = 0; i < m_dimension; ++i )
      setColumnStretchable( i, true );
   connect( this, TQT_SIGNAL( valueChanged( int, int ) ),
            TQT_SLOT( slotTextChanged( int, int ) ) );
   setSizePolicy( TQSizePolicy( TQSizePolicy::Preferred, TQSizePolicy::Fixed ) );
}

void PMVectorListEdit::setVectors( const TQValueList<PMVector>& l,
                                   bool resize, int prec )
{
   TQValueList<PMVector>::ConstIterator it;
   int r, nl = 0;

   for( r = 0; r < ( signed ) m_links.size( ); ++r )
      if( m_links[r] >= 0 )
         ++nl;

   if( nl == 0 && resize )
      setSize( l.size( ) );

   for( r = 0, it = l.begin( ); it != l.end( ) && r < m_size; ++r )
   {
      if( m_disabled[r] )
      {
         if( !isReadOnly( ) )
            setRowReadOnly( r, true );
      }
      else
      {
         setVector( r, *it, prec );
         if( !isReadOnly( ) )
            setRowReadOnly( r, false );
         if( m_links[r] >= 0 )
            setVector( m_links[r], *it, prec );
         ++it;
      }
   }
   if( it != l.end( ) )
      kdError( PMArea ) << "Wrong size of vector list in PMVectorListEdit::setVectors" << endl;
}

TQValueList<PMVector> PMVectorListEdit::vectors( ) const
{
   TQValueList<PMVector> l;
   int i;

   for( i = 0; i < m_size; ++i )
      if( !m_disabled[i] )
         l.append( vector( i ) );

   return l;
}

void PMVectorListEdit::setVector( int r, const PMVector& v, int precision )
{
   if( r < 0 || r >= m_size )
   {
      kdError( PMArea ) << "Wrong vector index in PMVectorListEdit::setVector"
                        << endl;
      return;
   }

   bool sb = signalsBlocked( );
   blockSignals( true );

   int i;
   TQString str;

   for( i = 0; i < m_dimension && i <= ( signed ) v.size( ); ++i )
   {
      str.setNum( v[i], 'g', precision );
      setText( r, i, str );
   }

   blockSignals( sb );
}

PMVector PMVectorListEdit::vector( int r ) const
{
   PMVector v( m_dimension );

   if( r < 0 || r >= m_size )
   {
      kdError( PMArea ) << "Wrong vector index in PMVectorListEdit::vector"
                        << endl;
      return v;
   }

   int i;
   for( i = 0; i < m_dimension; ++i )
      v[i] = text( r, i ).toDouble( );
   return v;
}

void PMVectorListEdit::setSize( int s )
{
   if( s < 0 || s == m_size )
      return;

   setNumRows( s );
   int i;
   TQHeader* h = verticalHeader( );
   TQString str;

   for( i = 0; i < s; ++i )
   {
      setRowStretchable( i, true );
      setRowReadOnly( i, false );

      str.setNum( i + 1 );
      h->setLabel( i, str );
   }
   m_links.fill( -1, s );
   m_disabled.fill( false, s );
   m_size = s;
   updateGeometry( );
}

void PMVectorListEdit::setLink( int p1, int p2 )
{
   if( p1 < 0 || p1 >= m_size || p2 >= m_size )
      return;

   TQHeader* h = verticalHeader( );
   TQString str;

   // remove old link
   if( m_links[p1] >= 0 )
   {
      str.setNum( m_links[p1] + 1 );
      h->setLabel( m_links[p1], str );
      if( !isReadOnly( ) )
         setRowReadOnly( m_links[p1], false );
      m_disabled[m_links[p1]] = false;
   }

   if( p2 >= 0 )
   {
      m_disabled[p2] = true;
      str = TQString( "%1 (=%2)" ).arg( p2 + 1 ).arg( p1 + 1 );
      h->setLabel( p2, str );
      if( !isReadOnly( ) )
         setRowReadOnly( p2, true );
   }

   m_links[p1] = p2;
}

void PMVectorListEdit::clearLinks( )
{
   int i;
   for( i = 0; i < ( signed ) m_links.size( ); ++i )
      if( m_links[i] >= 0 )
         setLink( i, -1 );
}

bool PMVectorListEdit::isSelected( int i ) const
{
   return isRowSelected( i );
}

void PMVectorListEdit::select( int i )
{
   selectRow( i );
}

void PMVectorListEdit::select( int i, int j )
{
   TQTableSelection sel( i, 0, j, m_dimension - 1 );
   addSelection( sel );

}

bool PMVectorListEdit::isDataValid( )
{
   int r, i;
   bool valid = true;
   double d;

   for( r = 0; r < m_size && valid; ++r )
   {
      if( !m_disabled[r] )
      {
         for( i = 0; i < m_dimension && valid; ++i )
         {
            d = text( r, i ).toDouble( &valid );
            if( !valid )
            {
               setCurrentCell( r, i );
               KMessageBox::error( this, i18n( "Please enter a valid float value!" ),
                                   i18n( "Error" ) );
            }
         }
      }
   }
   return valid;
}

TQSize PMVectorListEdit::sizeHint( ) const
{
   return TQSize( c_columnWidth * m_dimension + frameWidth( ) * 2,
                 frameWidth( ) * 2 + horizontalHeader( )->height( )
                 + verticalHeader( )->sizeHint( ).height( ) );
}

void PMVectorListEdit::slotTextChanged( int, int )
{
   emit dataChanged( );
}

void PMVectorListEdit::blockSelectionUpdates( bool block )
{
   setUpdatesEnabled( !block );
   verticalHeader( )->setUpdatesEnabled( !block );
   horizontalHeader( )->setUpdatesEnabled( !block );
   if( !block )
   {
      updateContents( );
      verticalHeader( )->update( );
      horizontalHeader( )->update( );
   }
}

bool PMVectorListEdit::eventFilter( TQObject* o, TQEvent* e )
{
   if( e->type( ) == TQEvent::Wheel && parent( ) )
      return TQApplication::sendEvent( parent( ), e );
   if( e->type( ) == TQEvent::MouseButtonPress
       && ( ( TQMouseEvent* ) e )->button( ) == Qt::RightButton )
   {
      bool b = TQTable::eventFilter( o, e );
      emit showContextMenu( );
      return b;
   }
   return TQTable::eventFilter( o, e );
}

#include "pmvectorlistedit.moc"
