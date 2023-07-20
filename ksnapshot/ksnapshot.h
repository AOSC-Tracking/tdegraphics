#ifndef KSNAPSHOT_H
#define KSNAPSHOT_H
#include "ksnapshotiface.h"

#include <tqbitmap.h>
#include <tqcursor.h>
#include <tqlabel.h>
#include <tqmap.h>
#include <tqpainter.h>
#include <tqpixmap.h>
#include <tqstyle.h>
#include <tqtimer.h>

#include <dcopclient.h>
#include <tdeglobalsettings.h>
#include <kdialogbase.h>
#include <kurl.h>
#include <ktrader.h>

class RegionGrabber;
class KSnapshotWidget;
class KTempFile;
class TDEProcess;

class KSnapshotPreview : public TQLabel
{
    TQ_OBJECT

    public:
        KSnapshotPreview(TQWidget *parent, const char *name = 0)
            : TQLabel(parent, name)
        {
            setAlignment(AlignHCenter | AlignVCenter);
            setCursor(TQCursor(TQt::PointingHandCursor));
        }
        virtual ~KSnapshotPreview() {}

        void setPixmap(const TQPixmap& pm)
        {
            // if this looks convoluted, that's because it is. drawing a PE_SizeGrip
            // does unexpected things when painting directly onto the pixmap
            TQPixmap pixmap(pm);
            TQPixmap handle(15, 15);
            TQBitmap mask(15, 15, true);

            {
                TQPainter p(&mask);
                style().tqdrawPrimitive(TQStyle::PE_SizeGrip, &p, TQRect(0, 0, 15, 15), palette().active());
                p.end();
                handle.setMask(mask);
            }

            {
                TQPainter p(&handle);
                style().tqdrawPrimitive(TQStyle::PE_SizeGrip, &p, TQRect(0, 0, 15, 15), palette().active());
                p.end();
            }

            TQRect rect(pixmap.width() - 16, pixmap.height() - 16, 15, 15);
            TQPainter p(&pixmap);
            p.drawPixmap(rect, handle);
            p.end();
            TQLabel::setPixmap(pixmap);
        }

    signals:
        void startDrag();

    protected:
        void mousePressEvent(TQMouseEvent * e)
        {
            mClickPt = e->pos();
        }

        void mouseMoveEvent(TQMouseEvent * e)
        {
            if (mClickPt != TQPoint(0, 0) &&
                (e->pos() - mClickPt).manhattanLength() > TDEGlobalSettings::dndEventDelay())
            {
                mClickPt = TQPoint(0, 0);
                emit startDrag();
            }
        }

        void mouseReleaseEvent(TQMouseEvent * /*e*/)
        {
            mClickPt = TQPoint(0, 0);
        }

        TQPoint mClickPt;
};

class KSnapshot : public KDialogBase, virtual public KSnapshotIface
{
  TQ_OBJECT

public:
  KSnapshot(TQWidget *parent= 0, const char *name= 0, bool grabCurrent=false);
  ~KSnapshot();

  enum CaptureMode { FullScreen=0, WindowUnderCursor=1, Region=2, ChildWindow=3 };

  bool save( const TQString &filename );
  TQString url() const { return filename.url(); }

protected slots:
  void slotAboutToQuit();
  void slotGrab();
  void slotSave();
  void slotSaveAs();
  void slotCopy();
  void slotPrint();
  void slotOpenWith(int id);
  void slotOpenWithKP();
  void slotExternalAppClosed(TDEProcess *process);
  void slotMovePointer( int x, int y );

  void setTime(int newTime);
  void setURL(const TQString &newURL);
  void setGrabMode( int m );
  void exit();

protected:
    void reject() { close(); }

    virtual void closeEvent( TQCloseEvent * e );
    void resizeEvent(TQResizeEvent*);
    bool eventFilter( TQObject*, TQEvent* );

private slots:
    void grabTimerDone();
    void slotDragSnapshot();
    void updateCaption();
    void updatePreview();
    void slotRegionGrabbed( const TQPixmap & );
    void slotWindowGrabbed( const TQPixmap & );

private:
    bool save( const KURL& url );
    void openWithExternalApp(const KService &service);
    void performGrab();
    void autoincFilename();
    int grabMode();
    int timeout();

    TQPixmap snapshot;
    TQTimer grabTimer;
    TQTimer updateTimer;
    TQWidget* grabber;
    KURL filename;
    KSnapshotWidget *mainWidget;
    RegionGrabber *rgnGrab;
    bool modified;
    TDETrader::OfferList openWithOffers;
    TQMap<TDEProcess*, KTempFile*> m_tmpFiles;
    TQPoint oldWinPos;
};

#endif // KSNAPSHOT_H

