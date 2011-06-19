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


#include "pmdialogeditbase.h"

#include <tqlayout.h>
#include <tqcheckbox.h>
#include <tqpushbutton.h>
#include <tqlabel.h>
#include <tqvbox.h>
#include <tqbuffer.h>
#include <tqptrdict.h>
#include <kdialog.h>
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "pmpart.h"
#include "pmdefaults.h"
#include "pmpovrayrenderwidget.h"
#include "pmpovrayoutputwidget.h"
#include "pmpovray31format.h"
#include "pmserializer.h"
#include "pmcolor.h"
#include "pmrecursiveobjectiterator.h"
#include "pmdeclare.h"

#include "pmdebug.h"

int PMDialogEditBase::s_previewSize = c_defaultTPSize;
bool PMDialogEditBase::s_showSphere = c_defaultTPShowSphere;
bool PMDialogEditBase::s_showCylinder = c_defaultTPShowCylinder;
bool PMDialogEditBase::s_showBox = c_defaultTPShowBox;
bool PMDialogEditBase::s_previewAA = c_defaultTPAA;
int PMDialogEditBase::s_previewAADepth = c_defaultTPAADepth;
double PMDialogEditBase::s_previewAAThreshold = c_defaultTPAAThreshold;
bool PMDialogEditBase::s_showFloor = c_defaultTPShowFloor;
bool PMDialogEditBase::s_showWall = c_defaultTPShowWall;
TQColor PMDialogEditBase::s_wallColor1 = c_defaultTPWallColor1;
TQColor PMDialogEditBase::s_wallColor2 = c_defaultTPWallColor2;
TQColor PMDialogEditBase::s_floorColor1 = c_defaultTPFloorColor1;
TQColor PMDialogEditBase::s_floorColor2 = c_defaultTPFloorColor2;
bool PMDialogEditBase::s_previewLocal = false;
double PMDialogEditBase::s_previewGamma = c_defaultTPGamma;


/** Constants for texture preview */
const TQString c_wallCode = TQString(
   "plane { <0, 0, -1>, -2\n"
   "  pigment { checker %1 %2\n"
   "    scale <0.5, 0.5, 0.5>\n"
   "    translate <0.5, 0, 0>\n"
   "  }\n"
   "}\n" );
const TQString c_floorCode = TQString(
   "plane { <0, 1, 0>, 0\n"
   "  pigment { checker %1 %2\n"
   "    scale <0.5, 0.5, 0.5>\n"
   "  }\n"
   "}\n" );
const TQString c_lightCode = TQString(
   "light_source { <-2.5, 3, -1.5>, rgb <1, 1, 1> }\n"
   "light_source { <3, 3, -3>, rgb <0.6, 0.6, 0.6> shadowless }\n" );


const TQString c_cameraCode[3] = {
   TQString( "camera { location <-1, 1.25, -2> right <1, 0, 0>\n"
            "  look_at <0.0, 0.5, 0> angle 45 }\n" ),
   TQString( "camera { location <-1, 2, -3> right <1, 0, 0>\n"
            "  look_at <0.0, 1, 0> angle 45 }\n" ),
   TQString( "camera { location <-2, 2.5, -4> right <1, 0, 0>\n"
            "  look_at <0.0, 1.5, 0> angle 45 }\n" )
};
const TQString c_sphereCode = TQString(
   "sphere { <0, 0.5, 0>, 0.5 translate <0, %1, 0> }\n" );
const TQString c_cylinderCode = TQString(
   "cylinder { <0, 0, 0>, <0, 1, 0>, 0.5 translate <0, %1, 0> }\n" );
const TQString c_boxCode = TQString(
   "box { <-0.5, 0, -0.5>, <0.5, 1, 0.5> translate <0, %1, 0> }\n" );
const TQString c_globalSettingsCode = TQString(
   "global_settings { assumed_gamma %1 }\n" );

PMDialogEditBase::PMDialogEditBase( TQWidget* tqparent, const char* name )
      : TQWidget( tqparent, name )
{
   m_pDisplayedObject = 0;
   m_pPart = 0;

   m_pTexturePreviewWidget = 0;
   m_pOutputWidget = 0;
   m_pRenderWidget = 0;
   m_pRenderFrame = 0;
   m_pPreviewLocalBox = 0;
   m_pPreviewButton = 0;
   m_pOutputButton = 0;
}

PMDialogEditBase::~PMDialogEditBase( )
{
   if( m_pOutputWidget )
      delete m_pOutputWidget;
}

void PMDialogEditBase::createWidgets( )
{
   m_pTopLayout = new TQBoxLayout( this, TQBoxLayout::TopToBottom,
                                  KDialog::spacingHint( ) );

   createTopWidgets( );
   createBottomWidgets( );

   m_pTopLayout->addStretch( );
   m_pTopLayout->activate( );
}

void PMDialogEditBase::createBottomWidgets( )
{
   m_pTexturePreviewWidget = new TQWidget( this );
   m_pTexturePreviewWidget->hide( );
   m_pTopLayout->addWidget( m_pTexturePreviewWidget );
}

bool PMDialogEditBase::saveData( )
{
   if( isDataValid( ) )
   {
      saveContents( );
      return true;
   }
   return false;
}

void PMDialogEditBase::saveContents( )
{
}

void PMDialogEditBase::displayObject( PMObject* o )
{
   bool newObject = ( m_pDisplayedObject != o );
   m_pDisplayedObject = o;
   PMObject* global;
   PMObject* local;

   // Is object a full texture or inside a full texture
   findTextures( global, local );
   if( global )
   {
      // is the preview widget created?
      if( !m_pRenderWidget )
      {
         TQVBoxLayout* vl = new TQVBoxLayout( m_pTexturePreviewWidget, 0,
                                            KDialog::spacingHint( ) );
         vl->addSpacing( 10 );
         TQFrame* hline = new TQFrame( m_pTexturePreviewWidget );
         hline->setFrameStyle( TQFrame::HLine | TQFrame::Plain );
         hline->setLineWidth( 1 );
         vl->addWidget( hline );
         vl->addWidget( new TQLabel( i18n( "Texture preview:" ), m_pTexturePreviewWidget ) );
         m_pRenderFrame = new TQVBox( m_pTexturePreviewWidget );
         m_pRenderFrame->setFrameStyle( TQFrame::StyledPanel | TQFrame::Sunken );
         m_pRenderFrame->setLineWidth( 2 );
         m_pRenderFrame->hide( );
         vl->addWidget( m_pRenderFrame );

         m_pRenderWidget = new PMPovrayRenderWidget( m_pRenderFrame );
         connect( m_pRenderWidget, TQT_SIGNAL( finished( int ) ),
                  TQT_SLOT( slotPreviewFinished( int ) ) );
         m_pPreviewLocalBox = new TQCheckBox( i18n( "local" ), m_pTexturePreviewWidget );
         m_pPreviewLocalBox->setChecked( s_previewLocal );
         connect( m_pPreviewLocalBox, TQT_SIGNAL( toggled( bool ) ),
                  TQT_SLOT( slotPreviewLocal( bool ) ) );
         vl->addWidget( m_pPreviewLocalBox );

         TQHBoxLayout* hl = new TQHBoxLayout( vl );
         m_pPreviewButton = new TQPushButton( i18n( "&Preview" ), m_pTexturePreviewWidget );
         hl->addWidget( m_pPreviewButton );
         connect( m_pPreviewButton, TQT_SIGNAL( clicked( ) ),
                  TQT_SLOT( slotTexturePreview( ) ) );
         m_pOutputButton = new TQPushButton( i18n( "Povray Output" ), m_pTexturePreviewWidget );
         hl->addWidget( m_pOutputButton );
         connect( m_pOutputButton, TQT_SIGNAL( clicked( ) ),
                  TQT_SLOT( slotPovrayOutput( ) ) );
         hl->addStretch( 1 );

         m_pOutputWidget = new PMPovrayOutputWidget( );
         connect( m_pRenderWidget, TQT_SIGNAL( povrayMessage( const TQString& ) ),
                  m_pOutputWidget, TQT_SLOT( slotText( const TQString& ) ) );
      }

      m_pTexturePreviewWidget->show( );
      if( newObject )
         m_pRenderFrame->hide( );
      m_pPreviewLocalBox->setEnabled( local != global );
      m_pPreviewButton->setEnabled( true );
   }
   else
   {
      // is the preview widget created?
      if( m_pPreviewButton )
         m_pPreviewButton->setEnabled( false );
      m_pTexturePreviewWidget->hide( );
   }
}

void PMDialogEditBase::setHelp( const TQString& anchor )
{
   m_helpTopic = anchor;
}

void PMDialogEditBase::redisplay( )
{
   if( m_pDisplayedObject )
      displayObject( m_pDisplayedObject );
}

void PMDialogEditBase::setCheckBox( TQCheckBox* box, PMThreeState state )
{
   switch( state )
   {
      case PMTrue:
         box->setChecked( true );
         break;
      case PMFalse:
         box->setChecked( false );
         break;
      case PMUnspecified:
         box->setNoChange( );
         break;
   }
}

PMThreeState PMDialogEditBase::checkBoxState( TQCheckBox* box )
{
   PMThreeState st = PMTrue;
   switch( box->state( ) )
   {
      case TQCheckBox::On:
         st = PMTrue;
         break;
      case TQCheckBox::Off:
         st = PMFalse;
         break;
      case TQCheckBox::NoChange:
         st = PMUnspecified;
         break;
   }
   return st;
}

void PMDialogEditBase::setPreviewSize( int size )
{
   if( ( size >= 10 ) && ( size <= 400 ) )
      s_previewSize = size;
}

void PMDialogEditBase::setPreviewAADepth( int d )
{
   if( ( d >= 1 ) && ( d <= 9 ) )
      s_previewAADepth = d;
}

void PMDialogEditBase::saveConfig( KConfig* cfg )
{
   cfg->setGroup( "TexturePreview" );
   cfg->writeEntry( "Size", s_previewSize );
   cfg->writeEntry( "showSphere", s_showSphere );
   cfg->writeEntry( "showCylinder", s_showCylinder );
   cfg->writeEntry( "showBox", s_showBox );
   cfg->writeEntry( "AA", s_previewAA );
   cfg->writeEntry( "AADepth", s_previewAADepth );
   cfg->writeEntry( "AAThreshold", s_previewAAThreshold );
   cfg->writeEntry( "showWall", s_showWall );
   cfg->writeEntry( "showFloor", s_showFloor );
   cfg->writeEntry( "WallColor1", s_wallColor1 );
   cfg->writeEntry( "WallColor2", s_wallColor2 );
   cfg->writeEntry( "FloorColor1", s_floorColor1 );
   cfg->writeEntry( "FloorColor2", s_floorColor2 );
   cfg->writeEntry( "Gamma", s_previewGamma );
}

void PMDialogEditBase::restoreConfig( KConfig* cfg )
{
   cfg->setGroup( "TexturePreview" );
   setPreviewSize( cfg->readNumEntry( "Size", s_previewSize ) );
   s_showSphere = cfg->readBoolEntry( "showSphere", s_showSphere );
   s_showCylinder = cfg->readBoolEntry( "showCylinder", s_showCylinder );
   s_showBox = cfg->readBoolEntry( "showBox", s_showBox );
   s_previewAA = cfg->readBoolEntry( "AA", s_previewAA );
   setPreviewAADepth( cfg->readNumEntry( "AADepth", s_previewAADepth ) );
   s_previewAAThreshold = cfg->readDoubleNumEntry( "AAThreshold", s_previewAAThreshold );
   s_showWall = cfg->readBoolEntry( "showWall", s_showWall );
   s_showFloor = cfg->readBoolEntry( "showFloor", s_showFloor );
   s_wallColor1 = cfg->readColorEntry( "WallColor1", &s_wallColor1 );
   s_wallColor2 = cfg->readColorEntry( "WallColor2", &s_wallColor2 );
   s_floorColor1 = cfg->readColorEntry( "FloorColor1", &s_floorColor1 );
   s_floorColor2 = cfg->readColorEntry( "FloorColor2", &s_floorColor2 );
   s_previewGamma = cfg->readDoubleNumEntry( "Gamma", s_previewGamma );
}

void PMDialogEditBase::findTextures( PMObject*& global, PMObject*& local ) const
{
   PMObject* o;
   global = 0;
   local = 0;

   for( o = m_pDisplayedObject; o; o = o->tqparent( ) )
   {
      if( o->type( ) == "Material" || o->type( ) == "Interior" ||
          o->type( ) == "Texture" || o->type( ) == "Pigment" ||
          o->type( ) == "InteriorTexture" )
      {
         if( !local )
            local = o;
         global = o;
      }
      else if( o->type( ) == "Declare" )
      {
         PMDeclare* d = ( PMDeclare* ) o;
         if( d->declareType( ) == "Interior" ||
             d->declareType( ) == "Pigment" ||
             d->declareType( ) == "Material" )
         {
            if( !local || ( local == global ) )
               local = o;
            global = o;
         }
         else if( d->declareType( ) == "Texture" ||
                  d->declareType( ) == "InteriorTexture" )
         {
            if( o->countChildren( ) > 1 )
            {
               if( !local )
                  local = o;
            }
            else
               if( !local || ( local == global ) )
                  local = o;
            global = o;
         }
      }
   }
}

void PMDialogEditBase::slotTexturePreview( )
{
   PMObject* o;
   PMObject* local = 0;
   PMObject* global = 0;

   if( !m_pRenderWidget )
      return;

   findTextures( global, local );
   if( local )
   {
      emit aboutToRender( );

      if( global == local )
         o = global;
      else
      {
         if( m_pPreviewLocalBox->isChecked( ) )
            o = local;
         else
            o = global;
      }

      // ensure at least one object will be rendered.
      if( !( s_showSphere || s_showCylinder || s_showBox ) )
         s_showSphere = true;

      int numObjects = 0;
      TQByteArray scene;
      TQBuffer buffer( scene );
      buffer.open( IO_WriteOnly );
      TQTextStream str( &buffer );
      PMPovray31Format format;
      PMSerializer* dev = format.newSerializer( TQT_TQIODEVICE(&buffer) );
      PMRenderMode mode;
      PMObjectList neededDeclares, objectsToCheck;
      TQPtrDict<PMObject> objectsToSerialize( 101 );
      PMObject* link;

      // find needed declares
      objectsToCheck.append( o );
      do
      {
         PMObjectListIterator it( objectsToCheck );
         for( ; it.current( ); ++it )
         {
            PMRecursiveObjectIterator rit( it.current( ) );
            for( ; rit.current( ); ++rit )
            {
               link = rit.current( )->linkedObject( );
               if( link )
                  if( !neededDeclares.tqcontainsRef( link ) )
                     if( !objectsToSerialize.tqfind( link ) )
                        neededDeclares.append( link );
            }
            objectsToSerialize.insert( it.current( ), it.current( ) );
         }

         objectsToCheck = neededDeclares;
         neededDeclares.clear( );
      }
      while( objectsToCheck.count( ) > 0 );

      // serialize all needed declares in the right order
      int numDeclares = objectsToSerialize.count( );
      if( numDeclares > 0 )
      {
         PMObject* otr = o;

         // find the scene
         while( otr->tqparent( ) ) otr = otr->tqparent( );

         for( otr = otr->firstChild( ); otr && ( numDeclares > 0 );
              otr = otr->nextSibling( ) )
         {
            if( otr->type( ) == "Declare" )
            {
               if( objectsToSerialize.tqfind( otr ) )
               {
                  dev->serialize( otr );
                  numDeclares--;
               }
            }
         }
         str << "\n\n";
      }
      // if the previewed texture is a declare, serialize it
      if( o->type( ) == "Declare" )
          dev->serialize( o );

      // build the preview scene
      str << "union {\n";
      if( s_showBox )
      {
         str << c_boxCode.tqarg( numObjects );
         numObjects++;
      }
      if( s_showCylinder )
      {
         str << c_cylinderCode.tqarg( numObjects );
         numObjects++;
      }
      if( s_showSphere )
      {
         str << c_sphereCode.tqarg( numObjects );
         numObjects++;
      }

      // serialize the texture
      if( o->type( ) == "Declare" )
      {
         PMDeclare* dec = ( PMDeclare* ) o;
         if( dec->declareType( ) == "Interior" )
            str << "interior { ";
         else if( dec->declareType( ) == "Texture" )
            str << "texture { ";
         else if( dec->declareType( ) == "Pigment" )
            str << "pigment { ";
         else if( dec->declareType( ) == "InteriorTexture" )
            str << "interior_texture { ";
         else if( dec->declareType( ) == "Material" )
            str << "material { ";
         else
            kdError( PMArea ) << "Unhandled declare type in "
               "PMDialogEditBase::slotTexturePreview\n";

         str << dec->id( );
         str << " }\n";
      }
      else
         dev->serialize( o );
      str << "}\n";

      PMColor c1, c2;
      if( s_showWall )
      {
         c1 = PMColor( s_wallColor1 );
         c2 = PMColor( s_wallColor2 );
         str << c_wallCode.tqarg( c1.serialize( true ) ).tqarg( c2.serialize( true ) );
      }
      if( s_showFloor )
      {
         c1 = PMColor( s_floorColor1 );
         c2 = PMColor( s_floorColor2 );
         str << c_floorCode.tqarg( c1.serialize( true ) ).tqarg( c2.serialize( true ) );
      }

      str << c_lightCode;
      str << c_cameraCode[numObjects-1];
      str << c_globalSettingsCode.tqarg( s_previewGamma );

      // Set the render mode
      mode.setHeight( s_previewSize );
      mode.setWidth( s_previewSize );
      mode.setAntialiasing( s_previewAA );
      mode.setAntialiasingThreshold( s_previewAAThreshold );
      mode.setAntialiasingDepth( s_previewAADepth );

      m_pRenderFrame->show( );
      m_pRenderFrame->setFixedSize( s_previewSize + m_pRenderFrame->frameWidth( ) * 2,
                                    s_previewSize + m_pRenderFrame->frameWidth( ) * 2 );
      m_pRenderFrame->updateGeometry( );
      m_pTexturePreviewWidget->tqlayout( )->activate( );
      emit sizeChanged( );
      m_pOutputWidget->slotClear( );
      m_pRenderWidget->render( scene, mode, m_pPart->url( ) );

      delete dev;
   }
}

void PMDialogEditBase::slotPreviewLocal( bool on )
{
   s_previewLocal = on;
}

void PMDialogEditBase::slotPreviewFinished( int exitStatus )
{
   if( exitStatus != 0 )
   {
      KMessageBox::error( this, i18n( "Povray exited abnormally with "
                                      "exit code %1.\n"
                                      "See the povray output for details." )
                          .tqarg( exitStatus ) );
   }
   else if( m_pRenderWidget->povrayOutput( ).tqcontains( "ERROR" ) )
   {
      KMessageBox::error( this, i18n( "There were errors while rendering.\n"
                                      "See the povray output for details." ) );
   }
}

void PMDialogEditBase::slotPovrayOutput( )
{
   if( m_pOutputWidget )
      m_pOutputWidget->show( );
}

#include "pmdialogeditbase.moc"
