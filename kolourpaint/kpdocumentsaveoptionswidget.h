
/*
   Copyright (c) 2003,2004,2005 Clarence Dang <dang@kde.org>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef KP_DOCUMENT_SAVE_OPTIONS_WIDGET_H
#define KP_DOCUMENT_SAVE_OPTIONS_WIDGET_H


#include <tqsize.h>

#include <tqwidget.h>


class TQPixmap;
class TQLabel;

class kpResizeSignallingLabel;


class kpDocumentSaveOptionsPreviewDialog : public TQWidget
{
Q_OBJECT
  TQ_OBJECT

public:
    kpDocumentSaveOptionsPreviewDialog (TQWidget *parent, const char *name = 0);
    virtual ~kpDocumentSaveOptionsPreviewDialog ();

    TQSize preferredMinimumSize () const;

protected:
    static const TQSize s_pixmapLabelMinimumSize;

signals:
    void moved ();
    void resized ();
    void finished ();

public slots:
    void setFilePixmapAndSize (const TQPixmap &filePixmap, int fileSize);
    void updatePixmapPreview ();

protected:
    virtual void closeEvent (TQCloseEvent *e);
    virtual void moveEvent (TQMoveEvent *e);
    virtual void resizeEvent (TQResizeEvent *e);

protected:
    TQPixmap *m_filePixmap;
    int m_fileSize;

    kpResizeSignallingLabel *m_filePixmapLabel;
    TQLabel *m_fileSizeLabel;
};


#include <tqrect.h>
#include <tqwidget.h>

#include <kpdocumentmetainfo.h>
#include <kpdocumentsaveoptions.h>


class TQLabel;
class TQTimer;

class KComboBox;
class KIntNumInput;
class KPushButton;


class kpDocumentSaveOptionsWidget : public TQWidget
{
Q_OBJECT
  TQ_OBJECT

public:
    kpDocumentSaveOptionsWidget (const TQPixmap &docPixmap,
                                 const kpDocumentSaveOptions &saveOptions,
                                 const kpDocumentMetaInfo &metaInfo,
                                 TQWidget *parent, const char *name = 0);
    kpDocumentSaveOptionsWidget (TQWidget *parent, const char *name = 0);
private:
    void init ();
public:
    virtual ~kpDocumentSaveOptionsWidget ();


    // <visualParent> is usually the filedialog
    void setVisualParent (TQWidget *visualParent);


protected:
    bool mimeTypeHasConfigurableColorDepth () const;
    bool mimeTypeHasConfigurableQuality () const;

public:
    TQString mimeType () const;
public slots:
    void setMimeType (const TQString &string);

public:
    int colorDepth () const;
    bool dither () const;
protected:
    static int colorDepthComboItemFromColorDepthAndDither (int depth, bool dither);
public slots:
    void setColorDepthDither (int depth,
                              bool dither = kpDocumentSaveOptions::initialDither ());
protected slots:
    void slotColorDepthSelected ();

public:
    int quality () const;
public slots:
    void setQuality (int newQuality);

public:
    kpDocumentSaveOptions documentSaveOptions () const;
public slots:
    void setDocumentSaveOptions (const kpDocumentSaveOptions &saveOptions);


public:
    void setDocumentPixmap (const TQPixmap &documentPixmap);
    void setDocumentMetaInfo (const kpDocumentMetaInfo &metaInfo);


protected:
    enum Mode
    {
        // (mutually exclusive)
        None, ColorDepth, Quality
    };

    Mode mode () const;
    void setMode (Mode mode);

protected slots:
    void repaintLabels ();


protected slots:
    void showPreview (bool yes = true);
    void hidePreview ();
    void updatePreviewDelayed ();
    void updatePreview ();
    void updatePreviewDialogLastRelativeGeometry ();


protected:
    TQWidget *m_visualParent;

    Mode m_mode;

    TQPixmap *m_documentPixmap;

    kpDocumentSaveOptions m_baseDocumentSaveOptions;
    kpDocumentMetaInfo m_documentMetaInfo;

    TQLabel *m_colorDepthLabel;
    KComboBox *m_colorDepthCombo;
    int m_colorDepthComboLastSelectedItem;
    TQWidget *m_colorDepthSpaceWidget;

    TQLabel *m_qualityLabel;
    KIntNumInput *m_qualityInput;

    KPushButton *m_previewButton;
    kpDocumentSaveOptionsPreviewDialog *m_previewDialog;
    TQRect m_previewDialogLastRelativeGeometry;
    TQTimer *m_updatePreviewTimer;
    int m_updatePreviewDelay;
    TQTimer *m_updatePreviewDialogLastRelativeGeometryTimer;
};


#endif  // KP_DOCUMENT_SAVE_OPTIONS_WIDGET_H
