
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


#ifndef KP_PIXMAP_FX_H
#define KP_PIXMAP_FX_H


#include <tqstring.h>


class TQBitmap;
class TQColor;
class TQImage;
class TQPointArray;
class TQPixmap;
class TQPoint;
class TQRect;
class TQString;
class TQWidget;
class TQWMatrix;

class kpColor;
class kpSelection;


class kpPixmapFX
{
public:

    //
    // Overflow Resistant Arithmetic:
    //
    // Returns INT_MAX if <lhs> or <rhs> < 0 or if would overflow.
    static int addDimensions (int lhs, int rhs);
    static int multiplyDimensions (int lhs, int rhs);


    //
    // TQPixmap Statistics
    //

    // Returns the width * height.
    static int pixmapArea (const TQPixmap &pixmap);
    static int pixmapArea (const TQPixmap *pixmap);
    static int pixmapArea (int width, int height);

    // Returns the estimated size of <pixmap> in pixmap memory.
    static int pixmapSize (const TQPixmap &pixmap);
    static int pixmapSize (const TQPixmap *pixmap);
    static int pixmapSize (int width, int height, int depth);

    static int imageSize (const TQImage &image);
    static int imageSize (const TQImage *image);
    static int imageSize (int width, int height, int depth);

    static int selectionSize (const kpSelection &sel);
    static int selectionSize (const kpSelection *sel);

    static int stringSize (const TQString &string);

    static int pointArraySize (const TQPointArray &points);


    //
    // TQPixmap/TQImage Conversion Functions
    //

    //
    // Converts <pixmap> to a TQImage and returns it.
    //
    // WARNING: On an 8-bit screen:
    //
    //              TQPixmap result = convertToPixmap (convertToImage (pixmap));
    //
    //          <result> is slightly differently colored to <pixmap>.
    //
    //          KolourPaint needs to convert to TQImage occasionally as
    //          TQImage allows KolourPaint to read pixels and because the TQImage
    //          methods give reliable results and pixel-identical results on
    //          all platforms.  The TQPixmap paint engine has no such guarantee
    //          and even depends on the quality of the video driver.
    //
    //          As a result, KolourPaint should not be used on an 8-bit screen.
    //          HITODO: Add warning on startup, like in KolourPaint/KDE4.
    //
    //          This bug will be fixed when KolourPaint gets a proper image library,
    //          where TQPixmap -> TQImage -> TQPixmap transitions will be not be needed.
    static TQImage convertToImage (const TQPixmap &pixmap);

    //
    // Dialog info for warning about data loss with convertToPixmap().
    //
    struct WarnAboutLossInfo
    {
        // <moreColorsThanDisplayAndHasAlphaChannelMessage>:
        //
        //     i18n ("The (image \"example.jpg\"|image from the clipboard)"
        //           " may have more colors than the current screen mode."
        //           " In order to display it, some colors may be changed."
        //           " Try increasing your screen depth to at least %1bpp."
        //
        //           "\nIt also"
        //
        //           " contains translucency which is not fully"
        //           " supported. The translucency data will be"
        //           " approximated with a 1-bit transparency tqmask.")
        //
        // <moreColorsThanDisplayMessage>:
        //     i18n ("The (image \"example.jpg\"|image from the clipboard)"
        //           " may have more colors than the current screen mode."
        //           " In order to display it, some colors may be changed."
        //           " Try increasing your screen depth to at least %1bpp.")
        //
        // <hasAlphaChannelMessage>:
        //     i18n ("The (image \"example.jpg\"|image from the clipboard)"
        //           " contains translucency which is not fully"
        //           " supported. The translucency data will be"
        //           " approximated with a 1-bit transparency tqmask.")
        //
        // <dontAskAgainPrefix>:
        //
        //     Don'tAskAgain ID for dialog.
        //
        // <tqparent>:
        //
        //     Dialog tqparent
        //
        WarnAboutLossInfo (const TQString &moreColorsThanDisplayAndHasAlphaChannelMessage,
                const TQString &moreColorsThanDisplayMessage,
                const TQString &hasAlphaChannelMessage,
                const TQString &dontAskAgainPrefix,
                TQWidget *tqparent)
            :
                m_moreColorsThanDisplayAndHasAlphaChannelMessage (
                    moreColorsThanDisplayAndHasAlphaChannelMessage),
                m_moreColorsThanDisplayMessage (
                    moreColorsThanDisplayMessage),
                m_hasAlphaChannelMessage (
                    hasAlphaChannelMessage),
                m_dontAskAgainPrefix (
                    dontAskAgainPrefix),
                m_parent (tqparent),
                m_isValid (true)
        {
        }

        WarnAboutLossInfo ()
            : m_parent (0),
              m_isValid (false)
        {
        }

        ~WarnAboutLossInfo ()
        {
        }


        bool isValid () const { return m_isValid; }


        TQString m_moreColorsThanDisplayAndHasAlphaChannelMessage,
                m_moreColorsThanDisplayMessage,
                m_hasAlphaChannelMessage;
        TQString m_dontAskAgainPrefix;
        TQWidget *m_parent;
        bool m_isValid;
    };

    //
    // Converts <image> to a TQPixmap of the current display's depth and
    // returns it.
    //
    // If the flag <pretty> is set, it will dither the image making the
    // returned pixmap look better but if the image has few colours
    // (less than the screen can handle), this will be at the expense of
    // exactness of conversion.
    //
    // This will automatically call ensureNoAlphaChannel().
    //
    // Never use a foreign TQPixmap that is offered to you - always get the
    // foreign TQImage and use this function to convert it to a sane TQPixmap.
    //
    // <wali>, if specified, describes parameters for the dialog that comes
    // up warning the user of data loss if the <image> contains translucency
    // and/or more colors than the current display.
    //
    static TQPixmap convertToPixmap (const TQImage &image, bool pretty = false,
                                    const WarnAboutLossInfo &wali = WarnAboutLossInfo ());

    // Same as convertToPixmap() but tries as hard as possible to make the
    // pixmap look like the original <image> - when in doubt, reads the
    // config to see whether or not to dither (default: on).
    //
    // If you know for sure that <image> can be displayed losslessly on
    // the screen, you should call convertToPixmap() with <pretty> = false
    // instead.  If you know for sure that <image> cannot be displayed
    // losslessly, then call convertToPixmap() with <pretty> = true.
    //
    static TQPixmap convertToPixmapAsLosslessAsPossible (const TQImage &image,
        const WarnAboutLossInfo &wali = WarnAboutLossInfo ());


    // Sets the RGB values of the pixels where <pixmap> is transparent to
    // <transparentColor>.  This has visually no effect on the <pixmap>
    // unless the tqmask is lost.
    static TQPixmap pixmapWithDefinedTransparentPixels (const TQPixmap &pixmap,
        const TQColor &transparentColor);


    //
    // Get/Set Parts of Pixmap
    //


    //
    // Returns the pixel and tqmask data found at the <rect> in <pm>.
    //
    static TQPixmap getPixmapAt (const TQPixmap &pm, const TQRect &rect);

    //
    // Sets the pixel and tqmask data at <destRect> in <*destPixmapPtr>
    // to <srcPixmap>.
    //
    static void setPixmapAt (TQPixmap *destPixmapPtr, const TQRect &destRect,
                             const TQPixmap &srcPixmap);

    //
    // Sets the pixel and tqmask data at the rectangle in <*destPixmapPtr>,
    // with the top-left <destAt> and dimensions <srcPixmap.rect()>,
    // to <srcPixmap>.
    //
    static void setPixmapAt (TQPixmap *destPixmapPtr, const TQPoint &destAt,
                             const TQPixmap &srcPixmap);
    static void setPixmapAt (TQPixmap *destPixmapPtr, int destX, int destY,
                             const TQPixmap &srcPixmap);

    //
    // Draws <srcPixmap> on top of <*destPixmapPtr> at <destAt>.
    // The tqmask of <*destPixmapPtr> is adjusted so that all opaque
    // pixels in <srcPixmap> will be opaque in <*destPixmapPtr>.
    //
    static void paintPixmapAt (TQPixmap *destPixmapPtr, const TQPoint &destAt,
                               const TQPixmap &srcPixmap);
    static void paintPixmapAt (TQPixmap *destPixmapPtr, int destX, int destY,
                               const TQPixmap &srcPixmap);

    //
    // Returns the colour of the pixel at <at> in <pm>.
    // If the pixel is transparent, a value is returned such that
    // kpTool::isColorTransparent(<return_value>) will return true.
    //
    static kpColor getColorAtPixel (const TQPixmap &pm, const TQPoint &at);
    static kpColor getColorAtPixel (const TQPixmap &pm, int x, int y);

    //
    // Returns the color of the pixel at <at> in <img>.
    // If the pixel is transparent, a value is returned such that
    // kpTool::isColorTransparent(<return_value>) will return true.
    //
    static kpColor getColorAtPixel (const TQImage &img, const TQPoint &at);
    static kpColor getColorAtPixel (const TQImage &img, int x, int y);


    //
    // Mask Operations
    //


    //
    // Removes <*destPixmapPtr>'s Alpha Channel and attempts to convert it
    // to a tqmask.  KolourPaint - and TQPixmap to a great extent - does not
    // support Alpha Channels - only tqmasks.  Call this whenever you get
    // a pixmap from a foreign source; else all KolourPaint code will
    // exhibit "undefined behaviour".
    //
    static void ensureNoAlphaChannel (TQPixmap *destPixmapPtr);

    //
    // Returns <pm>'s tqmask or a fully opaque tqmask (with <pm>'s dimensions)
    // if <pm> does not have a tqmask.
    //
    static TQBitmap getNonNullMask (const TQPixmap &pm);

    //
    // Ensures that <*destPixmapPtr> is transparent at <rect>.
    //
    static void ensureTransparentAt (TQPixmap *destPixmapPtr, const TQRect &destRect);

    //
    // Sets the tqmask of <*destPixmapPtr> at the rectangle, with the
    // top-left <destAt> and dimensions <srcMaskBitmap.rect()>,
    // to transparent where <brushBitmap> is opaque.
    //
    // <brushPixmap> must be a TQPixmap of depth 1 (or a TQBitmap).
    //
    static void paintMaskTransparentWithBrush (TQPixmap *destPixmapPtr, const TQPoint &destAt,
                                               const TQPixmap &brushBitmap);
    static void paintMaskTransparentWithBrush (TQPixmap *destPixmapPtr, int destX, int destY,
                                               const TQPixmap &brushBitmap);

    //
    // Ensures that <*destPixmapPtr> is opaque at <rect>.
    //
    static void ensureOpaqueAt (TQPixmap *destPixmapPtr, const TQRect &destRect);

    //
    // Ensures that <srcPixmap>'s opaque pixels will be opaque if
    // painted onto <*destPixmapPtr> at <destAt>.
    //
    static void ensureOpaqueAt (TQPixmap *destPixmapPtr, const TQPoint &destAt,
                                const TQPixmap &srcPixmap);
    static void ensureOpaqueAt (TQPixmap *destPixmapPtr, int destX, int destY,
                                const TQPixmap &srcPixmap);


    //
    // Effects
    //


    //
    // Converts the image to grayscale.
    //
    static void convertToGrayscale (TQPixmap *destPixmapPtr);
    static TQPixmap convertToGrayscale (const TQPixmap &pm);
    static void convertToGrayscale (TQImage *destImagePtr);
    static TQImage convertToGrayscale (const TQImage &img);

    //
    // Fills an image in the given color.
    //
    static void fill (TQPixmap *destPixmapPtr, const kpColor &color);
    static TQPixmap fill (const TQPixmap &pm, const kpColor &color);

    //
    // Resizes an image to the given width and height,
    // filling any new areas with <backgroundColor> if <fillNewAreas> is set.
    //
    static void resize (TQPixmap *destPixmapPtr, int w, int h,
                        const kpColor &backgroundColor, bool fillNewAreas = true);
    static TQPixmap resize (const TQPixmap &pm, int w, int h,
                           const kpColor &backgroundColor, bool fillNewAreas = true);

    //
    // Scales an image to the given width and height.
    // If <pretty> is true, a smooth scale will be used.
    //
    static void scale (TQPixmap *destPixmapPtr, int w, int h, bool pretty = false);
    static TQPixmap scale (const TQPixmap &pm, int w, int h, bool pretty = false);


    // The minimum difference between 2 angles (in degrees) such that they are
    // considered different.  This gives you at least enough precision to
    // rotate an image whose width <= 10000 such that its height increases
    // by just 1 (and similarly with height <= 10000 and width).
    //
    // Currently used for skew & rotate operations.
    static double AngleInDegreesEpsilon;


    //
    // Skews an image.
    //
    // <hangle>             horizontal angle clockwise (-90 < x < 90)
    // <vangle>             vertical angle clockwise (-90 < x < 90)
    // <backgroundColor>    color to fill new areas with
    // <targetWidth>        if > 0, the desired width of the resultant pixmap
    // <targetHeight>       if > 0, the desired height of the resultant pixmap
    //
    // Using <targetWidth> & <targetHeight> to generate preview pixmaps is
    // significantly more efficient than skewing and then scaling yourself.
    //
    static TQWMatrix skewMatrix (int width, int height, double hangle, double vangle);
    static TQWMatrix skewMatrix (const TQPixmap &pixmap, double hangle, double vangle);

    static void skew (TQPixmap *destPixmapPtr, double hangle, double vangle,
                      const kpColor &backgroundColor,
                      int targetWidth = -1, int targetHeight = -1);
    static TQPixmap skew (const TQPixmap &pm, double hangle, double vangle,
                         const kpColor &backgroundColor,
                         int targetWidth = -1, int targetHeight = -1);

    //
    // Rotates an image.
    //
    // <angle>              clockwise angle to rotate by
    // <backgroundColor>    color to fill new areas with
    // <targetWidth>        if > 0, the desired width of the resultant pixmap
    // <targetHeight>       if > 0, the desired height of the resultant pixmap
    //
    // Using <targetWidth> & <targetHeight> to generate preview pixmaps is
    // significantly more efficient than rotating and then scaling yourself.
    //
    static TQWMatrix rotateMatrix (int width, int height, double angle);
    static TQWMatrix rotateMatrix (const TQPixmap &pixmap, double angle);

    static bool isLosslessRotation (double angle);

    static void rotate (TQPixmap *destPixmapPtr, double angle,
                        const kpColor &backgroundColor,
                        int targetWidth = -1, int targetHeight = -1);
    static TQPixmap rotate (const TQPixmap &pm, double angle,
                           const kpColor &backgroundColor,
                           int targetWidth = -1, int targetHeight = -1);


    //
    // Flips an image in the given directions.
    //
    static TQWMatrix flipMatrix (int width, int height, bool horz, bool vert);
    static TQWMatrix flipMatrix (const TQPixmap &pixmap, bool horz, bool vert);

    // TODO: this kind of overloading is error prone
    //       e.g. TQPixmap pixmap;
    //            kpPixmapFX::flip (pixmap, false, true);
    //       looks like it will flip vertically but does absolutely nothing!
    //       (should be &pixmap)
    static void flip (TQPixmap *destPixmapPtr, bool horz, bool vert);
    static TQPixmap flip (const TQPixmap &pm, bool horz, bool vert);
    static void flip (TQImage *destImagePtr, bool horz, bool vert);
    static TQImage flip (const TQImage &img, bool horz, bool vert);
};


#endif  // KP_PIXMAP_FX_H
