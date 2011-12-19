/*
    kiconedit - a small graphics drawing program for the KDE
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

#include <tqwhatsthis.h>
#include <tqtooltip.h>

#include <kpopupmenu.h>
#include <kstdaction.h>
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kdebug.h>

#include "kiconedit.h"
#include "palettetoolbar.h"

WindowList KIconEdit::windowList;

KIconEdit::KIconEdit(const TQImage image, const char *name)
 : KMainWindow(0, name)
{
    init();
    img = image;
    img.detach();
    grid->load(&img);
    grid->setModified(true);
}

KIconEdit::KIconEdit(KURL url, const char *name)
 : KMainWindow(0, name)
{
    init();
    icon->open(&img, url);
}

void KIconEdit::init()
{
  setMinimumSize( 600, 540 );

  windowList.append(this);
  setCaption(kapp->caption());

  m_paletteToolBar = 0L;
  statusbar = 0L;

  history = new KCommandHistory( actionCollection(), true );

  gridview = new KGridView(&img, history, this);
  grid = gridview->grid();
  icon = new KIconEditIcon(TQT_TQOBJECT(this), &grid->image());

  setAcceptDrops(true);

  setupActions();
  setupStatusBar();
  setStandardToolBarMenuEnabled( true );

  createGUI();

  connect(this, TQT_SIGNAL(newname(const TQString &)),
           TQT_SLOT( slotUpdateStatusName(const TQString &)));

  connect( icon, TQT_SIGNAL( saved()), TQT_SLOT(slotSaved()));
  connect( icon, TQT_SIGNAL( loaded(TQImage *)), grid, TQT_SLOT(load(TQImage *)));
  connect( icon, TQT_SIGNAL(opennewwin(const TQString &)),
           TQT_SLOT(slotNewWin(const TQString &)));
  connect(icon, TQT_SIGNAL(newname(const TQString &)),
           TQT_SLOT( slotUpdateStatusName(const TQString &)));
  connect(icon, TQT_SIGNAL(newmessage(const TQString &)),
           TQT_SLOT( slotUpdateStatusMessage(const TQString &)));
  connect(icon, TQT_SIGNAL(addrecent(const TQString &)),
           TQT_SLOT( addRecent(const TQString &)));

  connect( m_paletteToolBar, TQT_SIGNAL( newColor(uint)),
     grid, TQT_SLOT(setColorSelection(uint)));

  connect( grid, TQT_SIGNAL( changed(const TQPixmap &) ),
      m_paletteToolBar, TQT_SLOT( previewChanged(const TQPixmap &) ) );
  connect( grid, TQT_SIGNAL( addingcolor(uint) ),
      m_paletteToolBar, TQT_SLOT(addColor(uint)));
  connect( grid, TQT_SIGNAL( colorschanged(uint, uint*) ),
      m_paletteToolBar, TQT_SLOT(addColors(uint, uint*)));

  connect(grid, TQT_SIGNAL(sizechanged(int, int)),
           TQT_SLOT( slotUpdateStatusSize(int, int)));
  connect(grid, TQT_SIGNAL(poschanged(int, int)),
           TQT_SLOT( slotUpdateStatusPos(int, int)));
  connect(grid, TQT_SIGNAL(scalingchanged(int)),
           TQT_SLOT( slotUpdateStatusScaling(int)));
  connect(grid, TQT_SIGNAL(scalingchanged(int)),
           TQT_SLOT( slotUpdateZoom(int)));
  connect( grid, TQT_SIGNAL( addingcolor(uint) ),
           TQT_SLOT(slotUpdateStatusColors(uint)));
  connect(grid, TQT_SIGNAL(colorschanged(uint, uint*)),
           TQT_SLOT( slotUpdateStatusColors(uint, uint*)));
  connect(grid, TQT_SIGNAL(newmessage(const TQString &)),
           TQT_SLOT( slotUpdateStatusMessage(const TQString &)));
  connect(grid, TQT_SIGNAL(clipboarddata(bool)),
           TQT_SLOT( slotUpdatePaste(bool)));
  connect(grid, TQT_SIGNAL(colorSelected(uint)),
           m_paletteToolBar, TQT_SLOT(currentColorChanged(uint)));
  connect(grid, TQT_SIGNAL(modifiedchanged(bool)),
           TQT_SLOT( slotUpdateStatusModified(bool)));
  gridview->checkClipboard(); //Not very elegant, but fixes a buglet

  connect(grid, TQT_SIGNAL(selecteddata(bool)), TQT_SLOT( slotUpdateCopy(bool)));

  kdDebug(4640) << "Updating statusbar" << endl;
  slotUpdateStatusSize(grid->cols(), grid->rows());
  slotUpdateStatusScaling(grid->scaling());

  slotUpdateZoom( grid->scaling() );

  if(icon->url().length())
    slotUpdateStatusName(icon->url());
  else
    slotUpdateStatusName(i18n("Untitled"));

  slotUpdateCopy(false);

  uint *c = 0, n = 0;
  n = grid->getColors(c);
  slotUpdateStatusColors(n, c);

  setCentralWidget(gridview);

  applyMainWindowSettings( kapp->config(), "MainWindowSettings" );
  updateProperties();

  updateAccels();
  show();
  moveDockWindow( m_paletteToolBar, TQt::DockRight, true, 0 );
}

KIconEdit::~KIconEdit()
{
    windowList.remove(this);

    if (windowList.count() < 1)
    {
        kapp->quit();
    }
}

bool KIconEdit::queryClose()
{
    bool cancel = false;
    if (grid->isModified())
    {
        int r = KMessageBox::warningYesNoCancel(this,
	    i18n("The current file has been modified.\nDo you want to save it?"), TQString(), KStdGuiItem::save(), KStdGuiItem::discard());

        switch(r)
        {
            case KMessageBox::Yes:
                if (!icon->save(&grid->image()))
                {
                    cancel = true;
                }
                break;
            case KMessageBox::Cancel:
                cancel = true;
                break;
            case KMessageBox::No:
            default:
                break;
        }
    }

    if(!cancel)
    {
        writeConfig();
    }
    return (!cancel);
}

// this is for exit by request of the session manager
void KIconEdit::saveProperties(KConfig *config )
{
    kdDebug(4640) << "KIconEdit::saveProperties" << endl;

    config->writePathEntry("Name", icon->url());
}

// this is for instances opened by the session manager
void KIconEdit::readProperties(KConfig *config)
{
    kdDebug(4640) << "KIconEdit::readProperties" << endl;

    TQString entry = config->readPathEntry("Name"); // no default
    if (entry.isEmpty())  return;
    icon->open(&grid->image(), KURL::fromPathOrURL( entry ));
}

/*
    this is for normal exits or request from "Options->Save options".
*/
void KIconEdit::writeConfig()
{
    KConfig *config = kapp->config();
    m_actRecent->saveEntries( kapp->config() );

    KIconEditProperties::self()->save();

    saveMainWindowSettings( config, "MainWindowSettings" );
}

TQSize KIconEdit::sizeHint() const
{
    if(gridview)
        return gridview->sizeHint();
    else
        return TQSize(-1, -1);
}

void KIconEdit::setupActions()
{
  kdDebug(4640) << "setupActions" << endl;

  KAction *action;
  KRadioAction *toolAction;
  KShortcut cut;

  // File Menu
  action = new KAction(i18n("New &Window"), "window_new", cut,
      TQT_TQOBJECT(this),  TQT_SLOT(slotNewWin()), actionCollection(), "file_new_window");
  action->setWhatsThis(i18n("New window\n\nOpens a new icon editor window."));

  action = KStdAction::openNew(TQT_TQOBJECT(this), TQT_SLOT(slotNew()), actionCollection());
  action->setWhatsThis(i18n("New\n\nCreate a new icon, either from a"
      " template or by specifying the size"));

  action = KStdAction::open(TQT_TQOBJECT(this), TQT_SLOT(slotOpen()), actionCollection());
  action->setWhatsThis(i18n("Open\n\nOpen an existing icon"));

  m_actRecent = KStdAction::openRecent(TQT_TQOBJECT(this),
      TQT_SLOT(slotOpenRecent(const KURL&)), actionCollection());
  m_actRecent->setMaxItems(15); // FIXME should be configurable!
  m_actRecent->loadEntries(kapp->config());

  action = KStdAction::save(TQT_TQOBJECT(this), TQT_SLOT(slotSave()), actionCollection());
  action->setWhatsThis(i18n("Save\n\nSave the current icon"));

  KStdAction::saveAs(TQT_TQOBJECT(this), TQT_SLOT(slotSaveAs()), actionCollection());

  action = KStdAction::print(TQT_TQOBJECT(this), TQT_SLOT(slotPrint()), actionCollection());
  action->setWhatsThis(i18n("Print\n\nOpens a print dialog to let you print"
      " the current icon."));

  KStdAction::close(TQT_TQOBJECT(this), TQT_SLOT(slotClose()), actionCollection());

  // Edit Menu

  m_actCut = KStdAction::cut(TQT_TQOBJECT(this), TQT_SLOT(slotCut()), actionCollection());
  m_actCut->setWhatsThis(i18n("Cut\n\nCut the current selection out of the"
      " icon.\n\n(Tip: You can make both rectangular and circular selections)"));

  m_actCopy = KStdAction::copy(TQT_TQOBJECT(this), TQT_SLOT(slotCopy()), actionCollection());
  m_actCopy->setWhatsThis(i18n("Copy\n\nCopy the current selection out of the"
      " icon.\n\n(Tip: You can make both rectangular and circular selections)"));

  m_actPaste = KStdAction::paste(TQT_TQOBJECT(this), TQT_SLOT(slotPaste()), actionCollection());
  m_actPaste->setWhatsThis(i18n("Paste\n\n"
       "Paste the contents of the clipboard into the current icon.\n\n"
       "If the contents are larger than the current icon you can paste them"
       " in a new window.\n\n"
       "(Tip: Select \"Paste transparent pixels\" in the configuration dialog"
       " if you also want to paste transparency.)"));

  m_actPasteNew = new KAction( i18n( "Paste as &New" ), cut, TQT_TQOBJECT(grid),
      TQT_SLOT( editPasteAsNew() ), actionCollection(), "edit_paste_as_new" );

  KStdAction::clear(TQT_TQOBJECT(this), TQT_SLOT(slotClear()), actionCollection());
  KStdAction::selectAll(TQT_TQOBJECT(this), TQT_SLOT(slotSelectAll()), actionCollection());

  action = new KAction(i18n("Resi&ze..."), "transform", cut,
      TQT_TQOBJECT(grid),  TQT_SLOT(editResize()), actionCollection(), "edit_resize");
  action->setWhatsThis(i18n("Resize\n\nSmoothly resizes the icon while"
      " trying to preserve the contents"));

  action = new KAction(i18n("&GrayScale"), "grayscale", cut,
      TQT_TQOBJECT(grid),  TQT_SLOT(grayScale()), actionCollection(), "edit_grayscale");
  action->setWhatsThis(i18n("Gray scale\n\nGray scale the current icon.\n"
      "(Warning: The result is likely to contain colors not in the icon"
      " palette"));

  // View Menu
  m_actZoomIn = KStdAction::zoomIn(TQT_TQOBJECT(this), TQT_SLOT(slotZoomIn()),
      actionCollection());
  m_actZoomIn->setWhatsThis(i18n("Zoom in\n\nZoom in by one."));

  m_actZoomOut = KStdAction::zoomOut(TQT_TQOBJECT(this), TQT_SLOT(slotZoomOut()),
      actionCollection());
  m_actZoomOut->setWhatsThis(i18n("Zoom out\n\nZoom out by one."));

  KActionMenu *actMenu = new KActionMenu( i18n( "&Zoom" ), "viewmag",
      actionCollection(), "view_zoom" );

  // xgettext:no-c-format
  action = new KAction( i18n( "100%" ), cut, TQT_TQOBJECT(this), TQT_SLOT( slotZoom1() ),
      actionCollection(), "view_zoom_1" );
  actMenu->insert( action );
  // xgettext:no-c-format
  action = new KAction( i18n( "200%" ), cut, TQT_TQOBJECT(this), TQT_SLOT( slotZoom2() ),
      actionCollection(), "view_zoom_2" );
  actMenu->insert( action );
  // xgettext:no-c-format
  action = new KAction( i18n( "500%" ), cut, TQT_TQOBJECT(this), TQT_SLOT( slotZoom5() ),
      actionCollection(), "view_zoom_5" );
  actMenu->insert( action );
  // xgettext:no-c-format
  action = new KAction( i18n( "1000%" ), cut, TQT_TQOBJECT(this), TQT_SLOT( slotZoom10() ),
      actionCollection(), "view_zoom_10" );
  actMenu->insert( action );

  // Settings Menu
  KStdAction::keyBindings(TQT_TQOBJECT(this), TQT_SLOT(slotConfigureKeys()),
      actionCollection());
  KStdAction::preferences(TQT_TQOBJECT(this), TQT_SLOT(slotConfigureSettings()),
      actionCollection());

  createStandardStatusBarAction();

  KToggleAction *toggle;

  toggle = new KToggleAction( i18n( "Show &Grid" ), "grid",
      cut, TQT_TQOBJECT(this), TQT_SLOT( slotShowGrid() ), actionCollection(),
      "options_show_grid" );
  toggle->setCheckedState(i18n("Hide &Grid"));
  toggle->setWhatsThis( i18n( "Show grid\n\nToggles the grid in the icon"
      " edit grid on/off" ) );
  toggle->setChecked( KIconEditProperties::self()->showGrid() );

  // Tools Menu
  toolAction = new KRadioAction(i18n("Color Picker"), "colorpicker",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolPointer()), actionCollection(),
      "tool_find_pixel");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Color Picker\n\nThe color of the pixel clicked"
      " on will be the current draw color"));

  toolAction = new KRadioAction(i18n("Freehand"), "paintbrush",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolFreehand()), actionCollection(),
      "tool_freehand");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Free hand\n\nDraw non-linear lines"));

  toolAction->setChecked( true );
  grid->setTool(KIconEditGrid::Freehand);

  toolAction = new KRadioAction(i18n("Rectangle"), "rectangle",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolRectangle()), actionCollection(),
      "tool_rectangle");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Rectangle\n\nDraw a rectangle"));

  toolAction = new KRadioAction(i18n("Filled Rectangle"), "filledrectangle",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolFilledRectangle()), actionCollection(),
      "tool_filled_rectangle");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Filled rectangle\n\nDraw a filled rectangle"));

  toolAction = new KRadioAction(i18n("Circle"), "circle",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolCircle()), actionCollection(),
      "tool_circle");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Circle\n\nDraw a circle"));

  toolAction = new KRadioAction(i18n("Filled Circle"), "filledcircle",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolFilledCircle()), actionCollection(),
      "tool_filled_circle");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Filled circle\n\nDraw a filled circle"));

  toolAction = new KRadioAction(i18n("Ellipse"), "ellipse",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolEllipse()), actionCollection(),
      "tool_ellipse");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Ellipse\n\nDraw an ellipse"));

  toolAction = new KRadioAction(i18n("Filled Ellipse"), "filledellipse",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolFilledEllipse()), actionCollection(),
      "tool_filled_ellipse");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Filled ellipse\n\nDraw a filled ellipse"));

  toolAction = new KRadioAction(i18n("Spray"), "airbrush",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolSpray()), actionCollection(),
      "tool_spray");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Spray\n\nDraw scattered pixels in the"
      " current color"));

  toolAction = new KRadioAction(i18n("Flood Fill"), "fill",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolFlood()), actionCollection(),
      "tool_flood_fill");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Flood fill\n\nFill adjoining pixels with"
      " the same color with the current color"));

  toolAction = new KRadioAction(i18n("Line"), "line",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolLine()), actionCollection(),
      "tool_line");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Line\n\nDraw a straight line vertically,"
      " horizontally or at 45 deg. angles"));

  toolAction = new KRadioAction(i18n("Eraser (Transparent)"), "eraser",
      cut, TQT_TQOBJECT(this), TQT_SLOT(slotToolEraser()), actionCollection(),
      "tool_eraser");
  toolAction->setExclusiveGroup("toolActions");
  toolAction->setWhatsThis(i18n("Erase\n\nErase pixels. Set the pixels to"
      " be transparent\n\n(Tip: If you want to draw transparency with a"
      " different tool, first click on \"Erase\" then on the tool you want"
      " to use)"));

  toolAction = new KRadioAction(i18n("Rectangular Selection"),
      "selectrect", cut, TQT_TQOBJECT(this),  TQT_SLOT(slotToolSelectRect()),
      actionCollection(), "edit_select_rectangle");
  toolAction->setExclusiveGroup( "toolActions" );
  toolAction->setWhatsThis(i18n("Select\n\nSelect a rectangular section"
      " of the icon using the mouse."));

  toolAction = new KRadioAction(i18n("Circular Selection"),
      "selectcircle", cut, TQT_TQOBJECT(this),  TQT_SLOT(slotToolSelectCircle()),
      actionCollection(), "edit_select_circle");
  toolAction->setExclusiveGroup( "toolActions" );
  toolAction->setWhatsThis(i18n("Select\n\nSelect a circular section of the"
      " icon using the mouse."));
}

void KIconEdit::updateAccels()
{
    actionCollection()->readShortcutSettings();
}

TQWidget *KIconEdit::createContainer( TQWidget *parent, int index,
        const TQDomElement &element, int &id )
{
    if ( element.attribute( "name" ) == "paletteToolBar" )
    {
        m_paletteToolBar = new PaletteToolBar( this, "paletteToolBar" );
        m_paletteToolBar->setText( i18n( "Palette Toolbar" ) );
        return m_paletteToolBar;
    }

    return KXMLGUIBuilder::createContainer( parent, index, element, id );
}

bool KIconEdit::setupStatusBar()
{
    statusbar = statusBar();

    TQString str = i18n("Statusbar\n\nThe statusbar gives information on"
        " the status of the current icon. The fields are:\n\n"
        "\t- Application messages\n\t- Cursor position\n\t- Size\n\t- Zoom factor\n"
        "\t- Number of colors");
    TQWhatsThis::add(statusBar(), str);

    statusbar->insertFixedItem("99999,99999", 0, true);
    statusbar->insertFixedItem("99999 x 99999", 1, true);
    statusbar->insertFixedItem(" 1:999", 2, true);
    str = i18n("Colors: %1").tqarg(9999999);
    statusbar->insertFixedItem(str, 3, true);
    statusbar->insertItem("", 4);

    statusbar->changeItem( "", 0);
    statusbar->changeItem( "", 1);
    statusbar->changeItem( "", 2);
    statusbar->changeItem( "", 3);

    return true;
}

void KIconEdit::addRecent(const TQString & path)
{
  m_actRecent->addURL(KURL( path ));
}

#include "kiconedit.moc"
