
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


#ifndef __kpselection_h__
#define __kpselection_h__

#include <tqbitmap.h>
#include <tqdatastream.h>
#include <tqobject.h>
#include <tqpixmap.h>
#include <tqpoint.h>
#include <tqpointarray.h>
#include <tqvaluevector.h>
#include <tqrect.h>
#include <tqstring.h>

#include <kpcolor.h>
#include <kppixmapfx.h>
#include <kpselectiontransparency.h>
#include <kptextstyle.h>


class TQSize;


/*
 * Holds a selection - will also be used for the clipboard
 * so that we can retain the border.
 */
class kpSelection : public TQObject
{
Q_OBJECT
  TQ_OBJECT

public:
    enum Type
    {
        Rectangle,
        Ellipse,
        Points,
        Text
    };

    // (for any)
    kpSelection (const kpSelectionTransparency &transparency = kpSelectionTransparency ());

    // (for Rectangle & Ellipse)
    kpSelection (Type type, const TQRect &rect, const TQPixmap &pixmap = TQPixmap (),
                 const kpSelectionTransparency &transparency = kpSelectionTransparency ());
    kpSelection (Type type, const TQRect &rect, const kpSelectionTransparency &transparency);

    // (for Text)
    kpSelection (const TQRect &rect, const TQValueVector <TQString> &textLines_, const kpTextStyle &textStyle_);

    // (for Points)
    kpSelection (const TQPointArray &points, const TQPixmap &pixmap = TQPixmap (),
                 const kpSelectionTransparency &transparency = kpSelectionTransparency ());
    kpSelection (const TQPointArray &points, const kpSelectionTransparency &transparency);

    kpSelection (const kpSelection &rhs);
    kpSelection &operator= (const kpSelection &rhs);
    friend TQDataStream &operator<< (TQDataStream &stream, const kpSelection &selection);
    friend TQDataStream &operator>> (TQDataStream &stream, kpSelection &selection);
    void readFromStream (TQDataStream &stream,
                         const kpPixmapFX::WarnAboutLossInfo &wali =
                             kpPixmapFX::WarnAboutLossInfo ());
    ~kpSelection ();

private:
    void calculatePoints ();

public:
    Type type () const;
    bool isRectangular () const;
    bool isText () const;
    // returns either i18n ("Selection") or i18n ("Text")
    TQString name () const;

    int size () const;

    TQBitmap maskForOwnType (bool nullForRectangular = false) const;

    // synonyms
    TQPoint topLeft () const;
    TQPoint point () const;

    int x () const;
    int y () const;

    void moveBy (int dx, int dy);
    void moveTo (int dx, int dy);
    void moveTo (const TQPoint &topLeftPoint);

    // synonyms
    TQPointArray points () const;
    TQPointArray pointArray () const;

    TQRect boundingRect () const;
    int width () const;
    int height () const;


    // (for non-rectangular selections, may return false even if
    //  kpView::onSelectionResizeHandle())
    bool tqcontains (const TQPoint &point) const;
    bool tqcontains (int x, int y);


    // (Avoid using for text selections since text selection may
    //  require a background for antialiasing purposes - use paint()
    //  instead, else no antialising)
    TQPixmap *pixmap () const;
    void setPixmap (const TQPixmap &pixmap);

    bool usesBackgroundPixmapToPaint () const;

private:
    void paintOpaqueText (TQPixmap *destPixmap, const TQRect &docRect) const;
    TQPixmap transparentForegroundTextPixmap () const;

public:
    // (<docRect> is the document rectangle that <*destPixmap> represents)
    void paint (TQPixmap *destPixmap, const TQRect &docRect) const;

private:
    void calculateTextPixmap ();

public:
    static TQString textForTextLines (const TQValueVector <TQString> &textLines_);
    TQString text () const;  // textLines() as one long string
    TQValueVector <TQString> textLines () const;
    void setTextLines (const TQValueVector <TQString> &textLines_);

    static int textBorderSize ();
    TQRect textAreaRect () const;
    bool pointIsInTextBorderArea (const TQPoint &globalPoint) const;
    bool pointIsInTextArea (const TQPoint &globalPoint) const;


    void textResize (int width, int height);

    // TODO: Enforce in kpSelection, not just in kpToolSelection & when pasting
    //       (in kpMainWindow).
    //       Be more robust when external enforcement fails.
    static int minimumWidthForTextStyle (const kpTextStyle &);
    static int minimumHeightForTextStyle (const kpTextStyle &);
    static TQSize tqminimumSizeForTextStyle (const kpTextStyle &);

    static int preferredMinimumWidthForTextStyle (const kpTextStyle &textStyle);
    static int preferredMinimumHeightForTextStyle (const kpTextStyle &textStyle);
    static TQSize preferredMinimumSizeForTextStyle (const kpTextStyle &textStyle);

    int minimumWidth () const;
    int minimumHeight () const;
    TQSize tqminimumSize () const;

    int textRowForPoint (const TQPoint &globalPoint) const;
    int textColForPoint (const TQPoint &globalPoint) const;
    TQPoint pointForTextRowCol (int row, int col);

    kpTextStyle textStyle () const;
    void setTextStyle (const kpTextStyle &textStyle);

    // TODO: ret val inconstent with pixmap()
    //       - fix when merge with kpTempPixmap
    TQPixmap opaquePixmap () const;  // same as pixmap()

private:
    void calculateTransparencyMask ();

public:
    // Returns opaquePixmap() after applying kpSelectionTransparency
    TQPixmap transparentPixmap () const;

    kpSelectionTransparency transparency () const;
    // Returns whether or not the selection changed due to setting the
    // transparency info.  If <checkTransparentPixmapChanged> is set,
    // it will try harder to return false (although the check is
    // expensive).
    bool setTransparency (const kpSelectionTransparency &transparency,
                          bool checkTransparentPixmapChanged = false);

private:
    void flipPoints (bool horiz, bool vert);

public:
    void flip (bool horiz, bool vert);

signals:
    void changed (const TQRect &docRect);

private:
    // OPT: use implicit sharing

    Type m_type;
    TQRect m_rect;
    TQPointArray m_points;
    TQPixmap *m_pixmap;

    TQValueVector <TQString> m_textLines;
    kpTextStyle m_textStyle;

    kpSelectionTransparency m_transparency;
    TQBitmap m_transparencyMask;

private:
    // There is no need to maintain binary compatibility at this stage.
    // The d-pointer is just so that you can experiment without recompiling
    // the kitchen sink.
    class kpSelectionPrivate *d;
};

#endif  // __kpselection_h__
