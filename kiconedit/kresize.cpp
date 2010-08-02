/*
    KDE Draw - a small graphics drawing program for the KDE
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)
    Copyright (C) 2002  Nadeem Hasan ( nhasan@kde.org )

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <tqlabel.h>
#include <tqlayout.h>
#include <tqgroupbox.h>

#include <klocale.h>
#include <knuminput.h>

#include "kresize.h"

KResizeWidget::KResizeWidget( TQWidget* parent, const char* name, 
    const TQSize& size ) : TQWidget( parent, name )
{
  TQHBoxLayout* genLayout = new TQHBoxLayout( this );

  TQGroupBox* group = new TQGroupBox( i18n( "Size" ), this );
  group->setColumnLayout( 0, Qt::Horizontal );
  genLayout->addWidget( group );

  TQHBoxLayout* layout = new TQHBoxLayout( group->layout(), 6 );

  m_width = new KIntSpinBox( 1, 200, 1, 1, 10, group );
  m_width->setValue( size.width() );
  layout->addWidget( m_width, 1 );

  TQLabel* label = new TQLabel( "X", group );
  layout->addWidget( label );

  m_height = new KIntSpinBox( 1, 200, 1, 1, 10, group);
  m_height->setValue( size.height() );
  layout->addWidget( m_height, 1 );

  setMinimumSize( 200, 100 );
}

KResizeWidget::~KResizeWidget()
{
}

const TQSize KResizeWidget::getSize()
{
  return TQSize( m_width->value(), m_height->value() );
}

KResizeDialog::KResizeDialog( TQWidget* parent, const char* name, 
    const TQSize size )
    : KDialogBase( parent, name, true, i18n( "Select Size" ), Ok|Cancel )
{
  m_resize = new KResizeWidget( this, "resize widget", size );

  setMainWidget( m_resize );
}

KResizeDialog::~KResizeDialog()
{
}

const TQSize KResizeDialog::getSize()
{
  return m_resize->getSize();
}

#include "kresize.moc"
