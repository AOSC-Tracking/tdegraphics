/*
    TDE Icon Editor - a small graphics drawing program for the TDE
    Copyright (C) 1998  Thomas Tanghus (tanghus@kde.org)

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

#include <tqpaintdevicemetrics.h>
#include <tqpainter.h>

#include <kkeydialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kprinter.h>
#include <kurldrag.h>

#include "kiconedit.h"
#include "kiconcolors.h"
#include "palettetoolbar.h"

#ifndef PICS_INCLUDED
#define PICS_INCLUDED
#include "pics/logo.xpm"
#endif

void TDEIconEdit::updateProperties()
{
    TDEIconEditProperties *props = TDEIconEditProperties::self();
    gridview->setShowRulers(props->showRulers());
    if(props->bgMode() == TQWidget::FixedPixmap)
    {
        TQPixmap pix(props->bgPixmap());
        if(pix.isNull())
        {
            TQPixmap pmlogo((const char**)logo);
            pix = pmlogo;
        }
        gridview->viewPortWidget()->viewport()->setBackgroundPixmap(pix);
        m_paletteToolBar->setPreviewBackground(pix);
    }
    else
    {
        gridview->viewPortWidget()->viewport()
            ->setBackgroundColor(props->bgColor());
        m_paletteToolBar->setPreviewBackground(props->bgColor());
    }
    grid->setTransparencyDisplayType(props->transparencyDisplayType());
    grid->setTransparencySolidColor(props->transparencySolidColor());
    grid->setCheckerboardColor1(props->checkerboardColor1());
    grid->setCheckerboardColor2(props->checkerboardColor2());
    grid->setCheckerboardSize(props->checkerboardSize());
    grid->update();
}

void TDEIconEdit::slotNewWin(const TQString & url)
{
    //kdDebug(4640) << "TDEIconEdit::openNewWin() - " << url << endl;

    TDEIconEdit *w = new TDEIconEdit(KURL(url), "kiconedit");
    TQ_CHECK_PTR(w);
}


void TDEIconEdit::slotNew()
{
    bool cancel = false;
    if (grid->isModified())
    {
        int r = KMessageBox::warningYesNoCancel(this,
        i18n("The current file has been modified.\nDo you want to save it?"), TQString(), KStdGuiItem::save(), KStdGuiItem::discard());

        switch(r)
        {
            case KMessageBox::Yes:
                if(!icon->save(&grid->image()))
                {
                    cancel = true;
                }
                break;

            case KMessageBox::No:
                break;

            case KMessageBox::Cancel:
                cancel = true;
                break;

            default:
                break;
        }
    }
    if(!cancel)
    {
        KNewIcon newicon(this);
        if(newicon.exec())
        {
            int r = newicon.openStyle();
            if(r == KNewIcon::Blank)
            {
                grid->editClear();
                const TQSize s = newicon.templateSize();
                //kdDebug(4640) << "Size: " << s.width() << " x " << s.height() << endl;
                grid->setSize(s);
                grid->setModified(false);
            }
            else if(r == KNewIcon::Template)
            {
                TQString str = newicon.templatePath();
                icon->open(&grid->image(), KURL( str ));
            }
            icon->setUrl("");
            emit newname(i18n("Untitled"));
        }
    }
}


void TDEIconEdit::slotOpen()
{
    bool cancel = false;

    if( grid->isModified() )
    {
        int r = KMessageBox::warningYesNoCancel(this,
      i18n("The current file has been modified.\nDo you want to save it?"),TQString(), KStdGuiItem::save(), KStdGuiItem::discard());

        switch( r )
        {
            case KMessageBox::Yes:
                if(!icon->save( &grid->image() ))
                {
                    cancel = true;
                }
                break;

            case KMessageBox::No:
                break;

            case KMessageBox::Cancel:
                cancel = true;
                break;

            default:
                break;
        }
    }

    if( !cancel )
    {
        if (icon->promptForFile( &grid->image() ))
        {
            grid->setModified(false);
        }
    }
}

/*
    close only the current window
*/
void TDEIconEdit::slotClose()
{
    //kdDebug(4640) << "TDEIconEdit: Closing " <<  endl;
    close();
}

void TDEIconEdit::slotSave()
{
    //kdDebug(4640) << "TDEIconEdit: slotSave() " << endl;
    icon->save(&grid->image());
}


void TDEIconEdit::slotSaveAs()
{
    //kdDebug(4640) << "TDEIconEdit: slotSaveAs() " << endl;
    icon->saveAs(&grid->image());
}


void TDEIconEdit::slotPrint()
{
    KPrinter printer;

    if ( printer.setup(this, i18n("Print %1").arg(icon->url().section('/', -1))) )
    {
        int margin = 10, yPos = 0;
        printer.setCreator("TDE Icon Editor");

        TQPainter p;
        p.begin( &printer );
        TQFontMetrics fm = p.fontMetrics();
        // need width/height
        TQPaintDeviceMetrics metrics( &printer );

        p.drawText( margin, margin + yPos, metrics.width(), fm.lineSpacing(),
                        ExpandTabs | DontClip, icon->url() );
        yPos = yPos + fm.lineSpacing();
        p.drawPixmap( margin, margin + yPos, grid->pixmap() );
        p.end();
  }
}

void TDEIconEdit::slotZoomIn()
{
    grid->zoom(DirIn);
}

void TDEIconEdit::slotZoomOut()
{
    grid->zoom(DirOut);
}

void TDEIconEdit::slotZoom1()
{
    grid->zoomTo(1);
}

void TDEIconEdit::slotZoom2()
{
    grid->zoomTo(2);
}

void TDEIconEdit::slotZoom5()
{
    grid->zoomTo(5);
}

void TDEIconEdit::slotZoom10()
{
    grid->zoomTo(10);
}

void TDEIconEdit::slotCopy()
{
    grid->editCopy();
}

void TDEIconEdit::slotCut()
{
    grid->editCopy(true);
}

void TDEIconEdit::slotPaste()
{
    static_cast<TDERadioAction*>(actionCollection()
        ->action("tool_find_pixel"))->setChecked(true);
    grid->setTool(TDEIconEditGrid::Find);
    grid->editPaste();
}

void TDEIconEdit::slotClear()
{
    grid->editClear();
}

void TDEIconEdit::slotSelectAll()
{
    grid->setTool(TDEIconEditGrid::SelectRect);
    grid->editSelectAll();
}

void TDEIconEdit::slotOpenRecent(const KURL& iconFile)
{
    bool cancel = false;

    if( grid->isModified() )
    {
        int r = KMessageBox::warningYesNoCancel(this,
      i18n("The current file has been modified.\nDo you want to save it?"),TQString(), KStdGuiItem::save(), KStdGuiItem::discard());

        switch( r )
        {
            case KMessageBox::Yes:
                if (!icon->save( &grid->image() ))
                {
                    cancel = true;
                }
                break;

            case KMessageBox::No:
                break;

            case KMessageBox::Cancel:
                cancel = true;
                break;

            default:
                break;
        }
    }

    if( !cancel )
    {
        if(icon->open(&grid->image(), iconFile))
        {
            grid->setModified(false);
        }
    }
}

void TDEIconEdit::slotConfigureSettings()
{
    TDEIconConfig* c = new TDEIconConfig(this);
    c->exec();
    delete c;
}

void TDEIconEdit::slotConfigureKeys()
{
    KKeyDialog::configure(actionCollection());

    TDEIconEdit *ki = 0L;
    for (ki = windowList.first(); ki; ki = windowList.next())
    {
        if (ki != this)
        {
            ki->updateAccels();
        }
    }
}

void TDEIconEdit::slotShowGrid()
{
  bool b = TDEIconEditProperties::self()->showGrid();
  grid->setGrid( !b );
  TDEIconEditProperties::self()->setShowGrid( !b );
}

void TDEIconEdit::slotToolPointer()
{
    grid->setTool(TDEIconEditGrid::Find);
}

void TDEIconEdit::slotToolFreehand()
{
    grid->setTool(TDEIconEditGrid::Freehand);
}

void TDEIconEdit::slotToolRectangle()
{
    grid->setTool(TDEIconEditGrid::Rect);
}

void TDEIconEdit::slotToolFilledRectangle()
{
    grid->setTool(TDEIconEditGrid::FilledRect);
}

void TDEIconEdit::slotToolCircle()
{
    grid->setTool(TDEIconEditGrid::Circle);
}

void TDEIconEdit::slotToolFilledCircle()
{
    grid->setTool(TDEIconEditGrid::FilledCircle);
}

void TDEIconEdit::slotToolEllipse()
{
    grid->setTool(TDEIconEditGrid::Ellipse);
}

void TDEIconEdit::slotToolFilledEllipse()
{
    grid->setTool(TDEIconEditGrid::FilledEllipse);
}

void TDEIconEdit::slotToolSpray()
{
    grid->setTool(TDEIconEditGrid::Spray);
}

void TDEIconEdit::slotToolFlood()
{
    grid->setTool(TDEIconEditGrid::FloodFill);
}

void TDEIconEdit::slotToolLine()
{
    grid->setTool(TDEIconEditGrid::Line);
}

void TDEIconEdit::slotToolEraser()
{
    grid->setTool(TDEIconEditGrid::Eraser);
}

void TDEIconEdit::slotToolSelectRect()
{
    grid->setTool(TDEIconEditGrid::SelectRect);
}

void TDEIconEdit::slotToolSelectCircle()
{
    grid->setTool(TDEIconEditGrid::SelectCircle);
}

void TDEIconEdit::slotSaved()
{
    grid->setModified(false);
}

void TDEIconEdit::slotUpdateZoom( int s )
{
    m_actZoomOut->setEnabled( s>1 );
}

void TDEIconEdit::slotUpdateStatusPos(int x, int y)
{
    TQString str = i18n("Status Position", "%1, %2").arg(x).arg(y);
    statusbar->changeItem( str, 0);
}

void TDEIconEdit::slotUpdateStatusSize(int x, int y)
{
    TQString str = i18n("Status Size", "%1 x %2").arg(x).arg(y);
    statusbar->changeItem( str, 1);
}

void TDEIconEdit::slotUpdateStatusScaling(int s)
{
    TDEIconEditProperties::self()->setGridScale( s );
    TQString str;

    str.sprintf("1:%d", s);
    statusbar->changeItem( str, 2);
}

void TDEIconEdit::slotUpdateStatusColors(uint)
{
    TQString str = i18n("Colors: %1").arg(grid->numColors());
    statusbar->changeItem( str, 3);
}

void TDEIconEdit::slotUpdateStatusColors(uint n, uint *)
{
    TQString str = i18n("Colors: %1").arg(n);
    statusbar->changeItem( str, 3);
}


void TDEIconEdit::slotUpdateStatusMessage(const TQString & msg)
{
    statusbar->changeItem( msg, 4);
}


void TDEIconEdit::slotUpdateStatusName(const TQString & name)
{
    m_name = name;

    TQString text = m_name;

    if(grid->isModified())
    {
        text += " ["+i18n("modified")+"]";
    }

    setCaption(text);
}


void TDEIconEdit::slotUpdateStatusModified(bool)
{
    slotUpdateStatusName(m_name);
}

void TDEIconEdit::slotUpdatePaste(bool state)
{
    m_actPaste->setEnabled(state);
    m_actPasteNew->setEnabled(state);
}


void TDEIconEdit::slotUpdateCopy(bool state)
{
    m_actCopy->setEnabled(state);
    m_actCut->setEnabled(state);
}


void TDEIconEdit::slotOpenBlank(const TQSize s)
{
    grid->loadBlank( s.width(), s.height());
}


void TDEIconEdit::dragEnterEvent(TQDragEnterEvent* e)
{
  e->accept(KURLDrag::canDecode(e));
}


/*
    accept drop of a file - opens file in current window
    old code to drop image, as image, should be removed
*/
void TDEIconEdit::dropEvent( TQDropEvent *e )
{
    //kdDebug(4640) << "Got TQDropEvent!" << endl;

    KURL::List fileList;
    bool loadedinthis = false;

    if(KURLDrag::decode(e, fileList))
    {
        for(KURL::List::ConstIterator it = fileList.begin(); 
            it != fileList.end(); ++it)
        {
            //kdDebug(4640) << "In dropEvent for "  <<  (*it).prettyURL() << endl;
            const KURL &url = *it;
            if(url.isValid())
            {
                if (!grid->isModified() && !loadedinthis)
                {
                    icon->open(&grid->image(), url);
                    loadedinthis = true;
                }
                else
                {
                    slotNewWin(url.url());
                }
            }
        }
    }
}


