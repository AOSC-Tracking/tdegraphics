// -*- C++ -*-
#ifndef KVIEWSHELL_H
#define KVIEWSHELL_H

#include <tqstring.h>

#include <kparts/mainwindow.h>

class KRecentFilesAction;
class KURL;
class KViewPart_Iface;


class TQLabel;

class KViewShell : public KParts::MainWindow
{
  Q_OBJECT
  

public:
  KStatusBar *statusbar;
  KStatusBar *action;

  KViewShell(const TQString& defaultMimeType = TQString());
  virtual ~KViewShell();

public slots:
  void openURL(const KURL&);
  void addRecentFile();


protected slots:
  void slotFullScreen();
  void slotQuit();
  void slotConfigureKeys();
  void slotEditToolbar();
  void slotFileClose();
  void slotNewToolbarConfig();

  void slotChangeZoomText(const TQString &);
  void slotChangePageText(const TQString &);
  void slotChangeSizeText(const TQString &);

signals:
  void restoreDocument(const KURL &url, int page);
  void saveDocumentRestoreInfo(TDEConfig* config);

protected:
  void readSettings();
  void writeSettings();

  /**
   * This method is called when it is time for the app to save its
   * properties for session management purposes.
   */
  void saveProperties(TDEConfig*);

  /**
   * This method is called when this app is restored.  The TDEConfig
   * object points to the session management config file that was saved
   * with @ref saveProperties
   */
  void readProperties(TDEConfig*);

  void checkActions();

  void dragEnterEvent(TQDragEnterEvent *event);
  void dropEvent(TQDropEvent *event);
  void keyPressEvent(TQKeyEvent * e);

private:
  KViewPart_Iface *view;

  KRecentFilesAction *recent;
  TQString cwd;

  KAction *closeAction, *reloadAction;
  KToggleAction *fullScreenAction;

  // In the attribute, the status of the statusbar (shown of hidden)
  // is saved when the kviewshell switches to fullscreen mode. The
  // statusbar can then be restored when the application returns to normal mode.
  bool isStatusBarShownInNormalMode;
  // ditto, for the toolbar
  bool isToolBarShownInNormalMode;
};


#endif
