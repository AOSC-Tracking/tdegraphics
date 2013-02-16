/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Carvalho
    email                : lpassos@mail.telepac.pt
    copyright            : (C) 2003 by Andreas Zehender
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

#include "pmviewlayoutmanager.h"
#include "pmdockwidget_private.h"

#include <tdeconfig.h>
#include <kstandarddirs.h>
#include <tdelocale.h>
#include <kfilterdev.h>
#include <tdepopupmenu.h>
#include <kiconloader.h>

#include <tqfile.h>
#include <tqdom.h>
#include <tqwidgetlist.h>
#include <tqlistbox.h>
#include <tqlineedit.h>
#include <tqlayout.h>
#include <tqlabel.h>

#include "pmshell.h"
#include "pmdebug.h"
#include "pmviewfactory.h"

PMViewLayoutManager* PMViewLayoutManager::s_pInstance = 0;
KStaticDeleter<PMViewLayoutManager> PMViewLayoutManager::s_staticDeleter;


//================  PMViewLayoutEntry ====================

PMViewLayoutEntry::PMViewLayoutEntry( )
{
   m_viewType = "treeview";
   m_dockPosition = PMDockWidget::DockRight;
   m_columnWidth = 33;
   m_height = 50;
   m_floatingWidth = 400;
   m_floatingHeight = 400;
   m_floatingPositionX = 200;
   m_floatingPositionY = 200;
   m_pCustomOptions = 0;
}

PMViewLayoutEntry::PMViewLayoutEntry( const PMViewLayoutEntry& e )
{
   m_viewType = e.m_viewType;
   m_dockPosition = e.m_dockPosition;
   m_columnWidth = e.m_columnWidth;
   m_height = e.m_height;
   m_floatingWidth = e.m_floatingWidth;
   m_floatingHeight = e.m_floatingHeight;
   m_floatingPositionX = e.m_floatingPositionX;
   m_floatingPositionY = e.m_floatingPositionY;
   if( e.m_pCustomOptions )
      m_pCustomOptions = e.m_pCustomOptions->copy( );
   else
      m_pCustomOptions = 0;
}

PMViewLayoutEntry::~PMViewLayoutEntry( )
{
   if( m_pCustomOptions )
      delete m_pCustomOptions;
}

void PMViewLayoutEntry::loadData( TQDomElement& e )
{
   TQString s;
   bool ok;

   // Read the view type
   s = e.tagName( );
   m_viewType = s;

   // Read dock position
   s = e.attribute( "position", "Right" );
   if( s == "Right" ) m_dockPosition = PMDockWidget::DockRight;
   else if( s == "Bottom" ) m_dockPosition = PMDockWidget::DockBottom;
   else if( s == "Center" ) m_dockPosition = PMDockWidget::DockCenter;
   else if( s == "None" ) m_dockPosition = PMDockWidget::DockNone;

   // Read the column width
   s = e.attribute( "columnWidth", "33" );
   m_columnWidth = s.toInt( &ok );
   if( !ok ) m_columnWidth = 33;

   // and view height
   s = e.attribute( "height", "50" );
   m_height = s.toInt( &ok );
   if( !ok ) m_height = 50;

   s = e.attribute( "floatingHeight", "400" );
   m_floatingHeight = s.toInt( &ok );
   if( !ok ) m_floatingHeight = 400;
   s = e.attribute( "floatingWidth", "400" );
   m_floatingWidth = s.toInt( &ok );
   if( !ok ) m_floatingWidth = 400;

   s = e.attribute( "floatingPosX", "200" );
   m_floatingPositionX = s.toInt( &ok );
   if( !ok ) m_floatingPositionX = 200;
   s = e.attribute( "floatingPosY", "200" );
   m_floatingPositionY = s.toInt( &ok );
   if( !ok ) m_floatingPositionY = 200;

   // Load custom options
   if( m_pCustomOptions )
      delete m_pCustomOptions;
   m_pCustomOptions =
      PMViewFactory::theFactory( )->newOptionsInstance( m_viewType );
   if( m_pCustomOptions )
      m_pCustomOptions->loadData( e );
}

void PMViewLayoutEntry::saveData( TQDomElement& e ) const
{
   TQString s;
   e.setTagName( m_viewType );
   switch( m_dockPosition )
   {
      case PMDockWidget::DockRight:
         e.setAttribute( "position", "Right" );
         break;
      case PMDockWidget::DockBottom:
         e.setAttribute( "position", "Bottom" );
         break;
      case PMDockWidget::DockCenter:
         e.setAttribute( "position", "Center" );
         break;
      case PMDockWidget::DockNone:
         e.setAttribute( "position", "None" );
         break;
      default:
         kdError( PMArea ) << i18n( "Unknown dock position." )
                           << endl;
         break;
   }
   s.setNum( m_columnWidth );
   e.setAttribute( "columnWidth", s );
   s.setNum( m_height );
   e.setAttribute( "height", s );
   s.setNum( m_floatingHeight );
   e.setAttribute( "floatingHeight", s );
   s.setNum( m_floatingWidth );
   e.setAttribute( "floatingWidth", s );
   s.setNum( m_floatingPositionX );
   e.setAttribute( "floatingPosX", s );
   s.setNum( m_floatingPositionY );
   e.setAttribute( "floatingPosY", s );

   // save custom options
   if( m_pCustomOptions )
      m_pCustomOptions->saveData( e );
}

void PMViewLayoutEntry::setViewType( const TQString& v )
{
   m_viewType = v;
}

void PMViewLayoutEntry::setDockPosition( PMDockWidget::DockPosition i )
{
   m_dockPosition = i;
}

void PMViewLayoutEntry::setColumnWidth( int i )
{
   m_columnWidth = i;
   if( i < 1 )
      m_columnWidth = 1;
}

void PMViewLayoutEntry::setHeight( int i )
{
   m_height = i;
   if( i < 1 )
      m_height = 1;
}

const TQString PMViewLayoutEntry::viewTypeAsString( )
{
   PMViewTypeFactory* f = PMViewFactory::theFactory( )->viewFactory( m_viewType );
   if( f )
   {
      if( m_pCustomOptions )
         return f->description( m_pCustomOptions );
      else
         return f->description( );
   }

   kdError( PMArea ) << i18n( "Unknown view type." )<< endl;
   return i18n( "Unknown" );
}

const TQString PMViewLayoutEntry::dockPositionAsString( )
{
   switch( m_dockPosition )
   {
      case PMDockWidget::DockRight:
         return i18n( "New Column" );
      case PMDockWidget::DockBottom:
         return i18n( "Below" );
      case PMDockWidget::DockCenter:
         return i18n( "Tabbed" );
      case PMDockWidget::DockNone:
         return i18n( "Floating" );
      default:
         kdError( PMArea ) << i18n( "Unknown dock position." )
                           << endl;
         return i18n( "unknown" );
   }
}

void PMViewLayoutEntry::setCustomOptions( PMViewOptions* o )
{
   if( m_pCustomOptions && m_pCustomOptions != o )
      delete m_pCustomOptions;
   m_pCustomOptions = o;
}

//==================  PMViewLayout ======================

PMViewLayout::PMViewLayout( )
{
   m_entries.clear( );
}

PMViewLayout::PMViewLayout( const PMViewLayout& vl )
{
   m_entries = vl.m_entries;
   m_name = vl.m_name;
}

PMViewLayout& PMViewLayout::operator = ( const PMViewLayout& vl )
{
   m_entries = vl.m_entries;
   m_name = vl.m_name;
   return *this;
}

void PMViewLayout::loadData( TQDomElement& e )
{
   m_entries.clear( );
   m_name = e.attribute( "name", "unnamed" );

   TQDomNode m = e.firstChild( );
   while( !m.isNull( ) )
   {
      if( m.isElement( ) )
      {
         TQDomElement me = m.toElement( );
         PMViewLayoutEntry vle;

         vle.loadData( me );
         m_entries.append( vle );
      }
      m = m.nextSibling( );
   }
   normalize( );
}

void PMViewLayout::saveData( TQDomElement& e, TQDomDocument& doc ) const
{
   TQValueList< PMViewLayoutEntry>::const_iterator it;

   e.setAttribute( "name", m_name );
   for( it = m_entries.begin( ); it != m_entries.end( ) ; ++it )
   {
      TQDomElement a;
      a = doc.createElement( "unknown" );
      ( *it ).saveData( a );
      e.appendChild( a );
   }
}

void PMViewLayout::setName( const TQString& n )
{
   m_name = n;
}

void PMViewLayout::addEntry( const PMViewLayoutEntry& e, int index )
{
   if( index == -1 )
      m_entries.append( e );
   else
      m_entries.insert( m_entries.at( index ), 1, e );
}

void PMViewLayout::removeEntry( int index )
{
   m_entries.remove( m_entries.at( index ) );
}

void PMViewLayout::displayLayout( PMShell* shell )
{
   PMDockWidget* lastWidget = 0;
   TQValueList< PMViewLayoutEntry>::const_iterator it;
   bool isViewSet = false;
   int lastWidth = 0, width = 100;
   int lastHeight = 0, height = 100;

   for( it = m_entries.begin( ); it != m_entries.end( ) ; ++it )
   {
      PMDockWidget* dock = shell->createView( ( *it ).viewType( ),
                                              ( *it ).customOptions( ),
                                              false );
      // Dock the widget
      if( ( *it ).dockPosition( ) == PMDockWidget::DockNone )
      {
         // the specified target is the desktop
         dock->manualDock( 0, PMDockWidget::DockDesktop, 50,
                           TQPoint( ( *it ).floatingPositionX( ),
                                   ( *it ).floatingPositionY( ) ) );
         dock->resize( ( *it ).floatingWidth( ), ( *it ).floatingHeight( ) );
         dock->show( );
      }
      else
      {
         // the first dockwidget is not docked but placed on all the window
         // through setView( )
         if( !isViewSet )
         {
            shell->setView( dock );
            isViewSet = true;
            lastWidget = dock;
         }
         else
         {
            switch( ( *it ).dockPosition( ) )
            {
               case PMDockWidget::DockRight:
                  dock->manualDock( lastWidget, ( *it ).dockPosition( ),
                                    lastWidth * 100 / width );

                  width -= lastWidth;
                  if( width < 1 ) width = 1;
                  height = 100;
                  lastWidget = dock;
                  break;
               case PMDockWidget::DockBottom:
                  dock->manualDock( lastWidget, ( *it ).dockPosition( ),
                                    lastHeight * 100 / height );
                  height -= lastHeight;
                  if( height < 1 ) height = 1;
                  lastWidget = dock;
                  break;
               case PMDockWidget::DockCenter:
                  dock->manualDock( lastWidget, ( *it ).dockPosition( ), 100 );
                  break;
               default:
                  dock->manualDock( 0, PMDockWidget::DockDesktop, 100 );
                  break;
            }
         }

         switch( ( *it ).dockPosition( ) )
         {
            case PMDockWidget::DockRight:
               lastWidth = ( *it ).columnWidth( );
               lastHeight = ( *it ).height( );
               break;
            case PMDockWidget::DockBottom:
               lastHeight = ( *it ).height( );
               break;
            default:
               break;
         }
      }
   }
}

void PMViewLayout::normalize( )
{
   iterator it;
   int sumColWidth = 0;

   it = m_entries.begin( );
   // the first entry has to be a new column
   if( it != m_entries.end( ) )
      if( ( *it ).dockPosition( ) != PMDockWidget::DockRight )
         ( *it ).setDockPosition( PMDockWidget::DockRight );

   // find negative or zero widths and heights
   for( it = m_entries.begin( ); it != m_entries.end( ); ++it )
   {
      if( ( *it ).columnWidth( ) < 1 )
         ( *it ).setColumnWidth( 1 );
      if( ( *it ).height( ) < 1 )
         ( *it ).setHeight( 1 );
   }

   // normalize the column widths
   for( it = m_entries.begin( ); it != m_entries.end( ); ++it )
      if( ( *it ).dockPosition( ) == PMDockWidget::DockRight )
         sumColWidth += ( *it ).columnWidth( );
   if( sumColWidth == 0 )
      sumColWidth = 1;

   for( it = m_entries.begin( ); it != m_entries.end( ); ++it )
   {
      if( ( *it ).dockPosition( ) == PMDockWidget::DockRight )
      {
         ( *it ).setColumnWidth( ( int ) ( ( *it ).columnWidth( ) * 100.0
                                           / sumColWidth + 0.5 ) );
         // normalize each column
         iterator it2 = it;
         int height = ( *it ).height( );

         for( it2++; it2 != m_entries.end( ) &&
             ( *it2 ).dockPosition( ) != PMDockWidget::DockRight; ++it2 )
            if( ( *it2 ).dockPosition( ) == PMDockWidget::DockBottom )
               height += ( *it2 ).height( );
         if( height == 0 )
            height = 1;
         ( *it ).setHeight( ( int ) ( ( *it ).height( ) * 100.0
                                      / height + 0.5 ) );
         it2 = it;
         for( it2++; it2 != m_entries.end( ) &&
             ( *it2 ).dockPosition( ) != PMDockWidget::DockRight; ++it2 )
            if( ( *it2 ).dockPosition( ) == PMDockWidget::DockBottom )
               ( *it2 ).setHeight( ( int ) ( ( *it2 ).height( ) * 100.0
                                             / height + 0.5 ) );
      }
   }
}

PMViewLayout PMViewLayout::extractViewLayout( PMShell* shell )
{
   PMViewLayout layout;

   TQValueList< TQValueList< PMViewLayoutEntry > > cols;
   cols.append( TQValueList< PMViewLayoutEntry >( ) );

   // find docked widgets
   recursiveExtractColumns( cols, cols.begin( ), 100,
                            shell->centralWidget( ) );

   TQValueListIterator< TQValueList< PMViewLayoutEntry > > cit;
   TQValueListIterator< PMViewLayoutEntry > eit;

   for( cit = cols.begin( ); cit != cols.end( ); ++cit )
      for( eit = ( *cit ).begin( ); eit != ( *cit ).end( ); ++eit )
         layout.addEntry( *eit );

   // find floating widgets
   TQPtrList<PMDockWidget> list;
   shell->manager( )->findFloatingWidgets( list );
   TQPtrListIterator<PMDockWidget> it( list );

   for( ; it.current( ); ++it )
   {
      kdDebug( PMArea ) << it.current( ) << " " << it.current( )->isVisible( ) << endl;
      TQWidget* w = it.current( )->getWidget( );
      if( w )
      {
         bool invalid = false;
         PMViewLayoutEntry e;
         e.setDockPosition( PMDockWidget::DockNone );
         TQPoint p = it.current( )->pos( );
         e.setFloatingPositionX( p.x( ) );
         e.setFloatingPositionY( p.y( ) );
         e.setFloatingWidth( it.current( )->width( ) );
         e.setFloatingHeight( it.current( )->height( ) );

         if( w->inherits( "PMViewBase" ) )
         {
            PMViewBase* v = ( PMViewBase* ) w;
            e.setViewType( v->viewType( ) );
            PMViewOptions* vo =
               PMViewFactory::theFactory( )->newOptionsInstance( v->viewType( ) );
            if( vo )
            {
               v->saveViewConfig( vo );
               e.setCustomOptions( vo );
            }
         }
         else
            invalid = true;

         if( !invalid )
            layout.addEntry( e );
      }
   }

   return layout;
}

void PMViewLayout::recursiveExtractColumns(
   TQValueList< TQValueList< PMViewLayoutEntry > >& cols,
   TQValueList< TQValueList< PMViewLayoutEntry > >::iterator cit,
   int width, TQWidget* widget )
{
   if( !widget )
      return;

   if( widget->inherits( "PMDockWidget" ) )
   {
      PMDockWidget* dw = ( PMDockWidget* ) widget;
      TQWidget* w = dw->getWidget( );
      if( w )
      {
         bool colStart = true;
         if( w->inherits( "PMDockSplitter" ) )
         {
            PMDockSplitter* sp = ( PMDockSplitter* ) w;
            if( sp->splitterOrientation( ) == Qt::Vertical )
            {
               colStart = false;
               // vertical splitter, split up the current column
               int w1 = ( int ) ( width * 0.01 * sp->separatorPos( ) + 0.5 );
               int w2 = width - w1;
               if( w1 == 0 ) w1++;
               if( w2 == 0 ) w2++;

               TQValueList< TQValueList< PMViewLayoutEntry > >::iterator cit1
                  = cols.insert( cit, TQValueList< PMViewLayoutEntry >( ) );

               recursiveExtractColumns( cols, cit1, w1, sp->getFirst( ) );
               recursiveExtractColumns( cols, cit, w2, sp->getLast( ) );
            }
         }
         if( colStart )
         {
            // widget is a view, a horizontal splitter or a tab widget
            // a new column starts
            PMViewLayoutEntry e;
            e.setColumnWidth( width );
            ( *cit ).append( e );
            recursiveExtractOneColumn( *cit, ( *cit ).begin( ), 100, dw );
         }
      }
   }
}

void PMViewLayout::recursiveExtractOneColumn(
   TQValueList< PMViewLayoutEntry >& entries,
   TQValueList< PMViewLayoutEntry >::iterator eit,
   int height, TQWidget* widget )
{
   if( !widget )
      return;

   if( widget->inherits( "PMDockWidget" ) )
   {
      PMDockWidget* dw = ( PMDockWidget* ) widget;
      TQWidget* w = dw->getWidget( );
      if( w )
      {
         if( w->inherits( "PMDockSplitter" ) )
         {
            PMDockSplitter* sp = ( PMDockSplitter* ) w;
            // splitter, split up the current column
            int h1 = ( int ) ( height * 0.01 * sp->separatorPos( ) + 0.5 );
            int h2 = height - h1;
            if( h1 == 0 ) h1++;
            if( h2 == 0 ) h2++;

            ( *eit ).setHeight( h1 );
            ( *eit ).setDockPosition( PMDockWidget::DockRight );

            PMViewLayoutEntry e;
            e.setHeight( h2 );
            e.setDockPosition( PMDockWidget::DockBottom );
            TQValueList< PMViewLayoutEntry >::iterator eit1 = eit;
            eit1 = entries.insert( ++eit1, e );

            recursiveExtractOneColumn( entries, eit, h1, sp->getFirst( ) );
            recursiveExtractOneColumn( entries, eit1, h2, sp->getLast( ) );
         }
         else if( w->inherits( "PMDockTabGroup" ) )
         {
            PMDockTabGroup* g = ( PMDockTabGroup* ) w;
            int num = g->count( );
            TQWidget* tw;
            int i;
            for( i = 0; i < num; i++ )
            {
               tw = g->page( i );
               if( i == 0 )
                  recursiveExtractOneColumn( entries, eit, height, tw );
               else
               {
                  PMViewLayoutEntry e;
                  e.setHeight( height );
                  e.setDockPosition( PMDockWidget::DockCenter );

                  eit++;
                  eit = entries.insert( eit, e );
                  recursiveExtractOneColumn( entries, eit, height, tw );
               }
            }
         }
         else
         {
            // a kpovmodeler view???
            if( w->inherits( "PMViewBase" ) )
            {
               PMViewBase* v = ( PMViewBase* ) w;
               ( *eit ).setViewType( v->viewType( ) );
               PMViewOptions* vo =
                  PMViewFactory::theFactory( )->newOptionsInstance( v->viewType( ) );
               if( vo )
               {
                  v->saveViewConfig( vo );
                  ( *eit ).setCustomOptions( vo );
               }
            }
         }
      }
   }
}

//=============== PMViewLayoutManager ===================

PMViewLayoutManager::PMViewLayoutManager( )
{
   m_layoutsLoaded = false;
   m_layoutDisplayed = false;
   loadData( );
}

PMViewLayoutManager::~PMViewLayoutManager( )
{
}

void PMViewLayoutManager::setDefaultLayout( const TQString& name )
{
   m_defaultLayout = name;
}

TQStringList PMViewLayoutManager::availableLayouts( )
{
   TQStringList result;
   TQValueListIterator<PMViewLayout> it;

   for( it = m_layouts.begin( ); it != m_layouts.end( ); ++it )
      result.push_back( ( *it ).name( ) );

   return result;
}

void PMViewLayoutManager::loadData( )
{
   if( m_layoutsLoaded )
      m_layouts.clear( );

   m_layoutsLoaded = true;

   TQString fileName = locate( "data", "kpovmodeler/viewlayouts.xml" );
   if( fileName.isEmpty( ) )
   {
      // Generate a default layout
      // I have a feeling this shouldn't be here but hey, it works for now
      // TODO Must find a way to move this cleanly to PMShell
      PMViewLayout a;
      a.setName( i18n( "Default" ) );
      PMViewLayoutEntry p;
      p.setViewType( "treeview" );
      p.setDockPosition( PMDockWidget::DockRight );
      p.setHeight( 50 );
      p.setColumnWidth( 33 );
      a.addEntry( p );
      p.setViewType( "dialogview" );
      p.setDockPosition( PMDockWidget::DockBottom );
      p.setHeight( 50 );
      a.addEntry( p );
      p.setViewType( "glview" );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewPosX ) );
      p.setDockPosition( PMDockWidget::DockRight );
      p.setHeight( 50 );
      p.setColumnWidth( 33 );
      a.addEntry( p );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewNegY ) );
      p.setDockPosition( PMDockWidget::DockBottom );
      p.setHeight( 50 );
      a.addEntry( p );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewPosZ ) );
      p.setDockPosition( PMDockWidget::DockRight );
      p.setHeight( 50 );
      p.setColumnWidth( 33 );
      a.addEntry( p );
      p.setCustomOptions( new PMGLViewOptions( PMGLView::PMViewCamera ) );
      p.setDockPosition( PMDockWidget::DockBottom );
      p.setHeight( 50 );
      a.addEntry( p );

      m_layouts.append( a );
      m_defaultLayout = a.name( );

      return;
   }

   TQFile file( fileName );
   if( !file.open( IO_ReadOnly ) )
   {
      kdError( PMArea ) << i18n( "Could not open the view layouts file." )
                        << endl;
      return;
   }

   TQDomDocument doc( "VIEWLAYOUTS" );
   doc.setContent( &file );

   TQDomElement e = doc.documentElement( );
   m_defaultLayout = e.attribute( "default", "empty" );

   TQDomNode c = e.firstChild( );

   TQString str;

   while( !c.isNull( ) )
   {
      if( c.isElement( ) )
      {
         TQDomElement ce = c.toElement( );
         PMViewLayout v;
         v.loadData( ce );
         m_layouts.append( v );
      }
      c = c.nextSibling( );
   }
}

void PMViewLayoutManager::saveData( )
{
   TQString fileName = locateLocal( "data", "kpovmodeler/viewlayouts.xml" );
   if( fileName.isEmpty( ) )
   {
      kdError( PMArea ) << i18n( "View layouts not found." ) << endl;
      return;
   }
   TQFile file( fileName );
   if( !file.open( IO_WriteOnly ) )
   {
      kdError( PMArea ) << i18n( "Could not open the view layouts file." )
                        << endl;
      return;
   }
   TQDomDocument doc( "VIEWLAYOUTS" );
   TQDomElement e = doc.createElement( "viewlist" );
   e.setAttribute( "default", m_defaultLayout );

   TQValueListIterator<PMViewLayout> it;

   for( it = m_layouts.begin( ); it != m_layouts.end( ); ++it )
   {
      TQDomElement l;

      l = doc.createElement( "viewlayout" );
      ( *it ).saveData( l, doc );
      e.appendChild( l );
   }
   doc.appendChild( e );
   TQTextStream str( &file );
   str.setEncoding( TQTextStream::UnicodeUTF8 );
   str << doc.toString( );
   file.close( );
}

PMViewLayoutManager* PMViewLayoutManager::theManager( )
{
   if( !s_pInstance )
      s_staticDeleter.setObject( s_pInstance, new PMViewLayoutManager( ) );
   return s_pInstance;
}

void PMViewLayoutManager::displayLayout( const TQString& name, PMShell* shell )
{
   PMViewLayout* v_layout = findLayout( name );

   if( v_layout )
   {
      // Destroy the existing dock widgets
      if( m_layoutDisplayed )
      {
         TQWidgetList lst;

         if( shell->centralWidget( ) )
            shell->manager( )->findChildDockWidget( shell->centralWidget( ), lst );
         while( lst.first( ) )
         {
            ( ( PMDockWidget* )lst.first( ) )->undock( );
            ( ( PMDockWidget* )lst.first( ) )->close( );
            lst.remove( );
         }

         TQPtrList<PMDockWidget> flist;
         shell->manager( )->findFloatingWidgets( flist );
         while( flist.first( ) )
         {
            flist.first( )->undock( );
            flist.first( )->close( );
            flist.remove( );
         }
      }
      // Create the new layout
      v_layout->displayLayout( shell );
      m_layoutDisplayed = true;
   }
}

void PMViewLayoutManager::displayDefaultLayout( PMShell* shell )
{
   displayLayout( m_defaultLayout, shell );
}

PMViewLayout* PMViewLayoutManager::findLayout( const TQString& name )
{
   TQValueListIterator<PMViewLayout> it;
   for( it = m_layouts.begin( ); it != m_layouts.end( ) &&
        ( *it ).name( ) != name; ++it );

   if( it == m_layouts.end( ) )
      return 0;
   return &( *it );
}

void PMViewLayoutManager::fillPopupMenu( TDEPopupMenu* pMenu )
{
   TQStringList lst = availableLayouts( );
   TQStringList::ConstIterator it = lst.begin( );

   pMenu->clear( );
   if( it != lst.end( ) )
   {
      for( ; it != lst.end( ); ++it )
         pMenu->insertItem( ( *it ) );
   }
}

void PMViewLayoutManager::addLayout( const TQString& name )
{
   PMViewLayout a;

   if( m_layouts.isEmpty( ) )
      m_defaultLayout = name;
   a.setName( name );
   m_layouts.append( a );
}

void PMViewLayoutManager::removeLayout( const TQString& name )
{
   TQValueListIterator<PMViewLayout> it;
   for( it = m_layouts.begin( ); it != m_layouts.end( ) &&
        ( *it ).name( ) != name; ++it );

   if( it != m_layouts.end( ) )
      m_layouts.remove( it );
}


PMSaveViewLayoutDialog::PMSaveViewLayoutDialog( PMShell* parent,
                                                const char* name )
      : KDialogBase( parent, name, true, i18n( "Save View Layout" ),
                     KDialogBase::Ok | KDialogBase::Cancel )
{
   m_pShell = parent;

   setButtonOK( KStdGuiItem::save() );
   enableButtonOK( false );

   TQWidget* w = new TQWidget( this );
   TQVBoxLayout* vl = new TQVBoxLayout( w, 0, KDialog::spacingHint( ) );

   TQLabel* l = new TQLabel( i18n( "Enter view layout name:" ), w );
   vl->addWidget( l );

   m_pLayoutName = new TQLineEdit( w );
   vl->addWidget( m_pLayoutName );
   connect( m_pLayoutName, TQT_SIGNAL( textChanged( const TQString& ) ),
            TQT_SLOT( slotNameChanged( const TQString& ) ) );

   TQListBox* lb = new TQListBox( w );
   vl->addWidget( lb );
   connect( lb, TQT_SIGNAL( highlighted( const TQString& ) ),
            TQT_SLOT( slotNameSelected( const TQString& ) ) );
   lb->insertStringList( PMViewLayoutManager::theManager( )
                         ->availableLayouts( ) );

   setMainWidget( w );
   setInitialSize( TQSize( 300, 200 ) );
}

PMSaveViewLayoutDialog::~PMSaveViewLayoutDialog( )
{
}

void PMSaveViewLayoutDialog::slotOk( )
{
   TQString name = m_pLayoutName->text( );

   PMViewLayoutManager* m = PMViewLayoutManager::theManager( );
   PMViewLayout* layout = m->findLayout( name );

   PMViewLayout newLayout = PMViewLayout::extractViewLayout( m_pShell );
   newLayout.setName( name );

   if( layout )
      *layout = newLayout;
   else
      m->addLayout( newLayout );

   m->saveData( );

   KDialogBase::slotOk( );
}

void PMSaveViewLayoutDialog::slotNameChanged( const TQString& s )
{
   enableButtonOK( !s.isEmpty( ) );
}

void PMSaveViewLayoutDialog::slotNameSelected( const TQString& s )
{
   m_pLayoutName->setText( s );
}

#include "pmviewlayoutmanager.moc"
