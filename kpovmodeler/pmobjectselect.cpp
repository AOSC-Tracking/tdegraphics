/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2000-2002 by Andreas Zehender
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

#include "pmobjectselect.h"
#include "pmfactory.h"
#include <klocale.h>
#include <kiconloader.h>

PMListBoxObject::PMListBoxObject( TQListBox* listbox, PMObject* obj,
                                  TQListBoxItem* after )
      : TQListBoxPixmap( listbox,
                      SmallIcon( obj->pixmap( ), PMFactory::instance( ) ),
                      checkName( obj->name( ) ), after )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( TQListBox* listbox, PMObject* obj )
      : TQListBoxPixmap( listbox,
                      SmallIcon( obj->pixmap( ), PMFactory::instance( ) ),
                      checkName( obj->name( ) ) )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( PMObject* obj )
      : TQListBoxPixmap( SmallIcon( obj->pixmap( ), PMFactory::instance( ) ),
                      checkName( obj->name( ) ) )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( TQListBox* listbox, PMObject* obj,
                                  const TQString& text, TQListBoxItem* after )
      : TQListBoxPixmap( listbox,
                      SmallIcon( obj->pixmap( ), PMFactory::instance( ) ),
                      text, after )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( TQListBox* listbox, PMObject* obj,
                                  const TQString& text )
      : TQListBoxPixmap( listbox,
                      SmallIcon( obj->pixmap( ), PMFactory::instance( ) ),
                      text )
{
   m_pObject = obj;
}

PMListBoxObject::PMListBoxObject( PMObject* obj, const TQString& text )
      : TQListBoxPixmap( SmallIcon( obj->pixmap( ), PMFactory::instance( ) ),
                      text )
{
   m_pObject = obj;
}

TQString PMListBoxObject::checkName( const TQString& text )
{
   if( text.isEmpty( ) )
      return i18n( "(unnamed)" );
   return text;
}

PMListBoxObject::~PMListBoxObject( )
{
}


TQSize PMObjectSelect::s_size = TQSize( 200, 300 );


PMObjectSelect::PMObjectSelect( TQWidget* tqparent, const char* name, bool modal )
      : KDialogBase( tqparent, name, modal, i18n( "Choose Object" ), Ok | Cancel )
{
   m_pSelectedObject = 0;
   m_pListBox = new TQListBox( this );
   setMainWidget( m_pListBox );
   setInitialSize( s_size );

   connect( m_pListBox, TQT_SIGNAL( highlighted( TQListBoxItem* ) ),
            TQT_SLOT( slotHighlighted( TQListBoxItem* ) ) );
   connect( m_pListBox, TQT_SIGNAL( selected( TQListBoxItem* ) ),
            TQT_SLOT( slotSelected( TQListBoxItem* ) ) );
   enableButtonOK( false );
}

PMObjectSelect::~PMObjectSelect( )
{
   s_size = size( );
}

void PMObjectSelect::addObject( PMObject* obj )
{
   m_pListBox->insertItem( new PMListBoxObject( obj ) );
}

int PMObjectSelect::selectObject( PMObject* link, const TQString& t,
                                  PMObject* & obj, TQWidget* tqparent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;
   
   do
   {
      scene = last->tqparent( );
      if( scene )
      {
         if( scene->type( ) == "Scene" )
         {
            last = last->prevSibling( );
            stop = true;
            found = true;
         }
         else
            last = last->tqparent( );
      }
      else
         stop = true;
   }
   while( !stop );
   
   if( found )
   {
      PMObjectSelect s( tqparent );

      PMObject* o = scene->firstChild( );
      bool l = false;
      
      while( o && !l && last )
      {
         if( o->type( ) == t )
            s.m_pListBox->insertItem( new PMListBoxObject( o ) );
         
         if( o == last )
            l = true;
         else
            o = o->nextSibling( );
      }
   
      int result = s.exec( );
      if( result == Accepted )
         obj = s.selectedObject( );
      
      return result;
   }
   else
      kdError( PMArea ) << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

int PMObjectSelect::selectObject( PMObject* link,
                                  const TQStringList& t,
                                  PMObject* & obj, TQWidget* tqparent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;
   
   do
   {
      scene = last->tqparent( );
      if( scene )
      {
         if( scene->type( ) == "Scene" )
         {
            last = last->prevSibling( );
            stop = true;
            found = true;
         }
         else
            last = last->tqparent( );
      }
      else
         stop = true;
   }
   while( !stop );
   
   if( found )
   {
      PMObjectSelect s( tqparent );

      PMObject* o = scene->firstChild( );
      bool l = false;
      
      while( o && !l && last )
      {
         if( t.tqfindIndex( o->type( ) ) >= 0 )
            s.m_pListBox->insertItem( new PMListBoxObject( o ) );
         
         if( o == last )
            l = true;
         else
            o = o->nextSibling( );
      }
   
      int result = s.exec( );
      if( result == Accepted )
         obj = s.selectedObject( );
      
      return result;
   }
   else
      kdError( PMArea ) << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

int PMObjectSelect::selectDeclare( PMObject* link, const TQString& declareType,
                                   PMObject* & obj, TQWidget* tqparent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;
   
   do
   {
      scene = last->tqparent( );
      if( scene )
      {
         if( scene->type( ) == "Scene" )
         {
            last = last->prevSibling( );
            stop = true;
            found = true;
         }
         else
            last = last->tqparent( );
      }
      else
         stop = true;
   }
   while( !stop );
   
   if( found )
   {
      PMObjectSelect s( tqparent );

      PMObject* o = scene->firstChild( );
      PMDeclare* decl;
      
      bool l = false;
      
      while( o && !l && last )
      {
         if( o->type( ) == "Declare" )
         {
            decl = ( PMDeclare* ) o;
            if( decl->declareType( ) == declareType )
               s.m_pListBox->insertItem( new PMListBoxObject( o ) );
         }
         
         if( o == last )
            l = true;
         else
            o = o->nextSibling( );
      }
   
      int result = s.exec( );
      if( result == Accepted )
         obj = s.selectedObject( );
      
      return result;
   }
   else
      kdError( PMArea ) << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

int PMObjectSelect::selectDeclare( PMObject* link, const TQStringList& declareTypes,
                                   PMObject* & obj, TQWidget* tqparent )
{
   PMObject* last = link;
   PMObject* scene;
   bool stop = false;
   bool found = false;
   
   do
   {
      scene = last->tqparent( );
      if( scene )
      {
         if( scene->type( ) == "Scene" )
         {
            last = last->prevSibling( );
            stop = true;
            found = true;
         }
         else
            last = last->tqparent( );
      }
      else
         stop = true;
   }
   while( !stop );
   
   if( found )
   {
      PMObjectSelect s( tqparent );

      PMObject* o = scene->firstChild( );
      PMDeclare* decl;
      
      bool l = false;
      
      while( o && !l && last )
      {
         if( o->type( ) == "Declare" )
         {
            decl = ( PMDeclare* ) o;
            if( declareTypes.tqfindIndex( decl->declareType( ) ) >= 0 )
               s.m_pListBox->insertItem( new PMListBoxObject( o ) );
         }
         
         if( o == last )
            l = true;
         else
            o = o->nextSibling( );
      }
   
      int result = s.exec( );
      if( result == Accepted )
         obj = s.selectedObject( );
      
      return result;
   }
   else
      kdError( PMArea ) << "PMObjectSelect: Link does not seem to be correctly inserted in the scene.\n";
   return Rejected;
}

void PMObjectSelect::slotHighlighted( TQListBoxItem* lbi )
{
   m_pSelectedObject = ( ( PMListBoxObject* ) lbi )->object( );
   enableButtonOK( true );
}

void PMObjectSelect::slotSelected( TQListBoxItem* lbi )
{
   m_pSelectedObject = ( ( PMListBoxObject* ) lbi )->object( );
   enableButtonOK( true );
   accept( );
}
#include "pmobjectselect.moc"
