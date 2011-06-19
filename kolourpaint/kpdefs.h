
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


#ifndef __kp_defs_h__
#define __kp_defs_h__


#include <limits.h>

#include <tqglobal.h>
#include <tqpoint.h>
#include <tqsize.h>
#include <tqstring.h>

#include <kdeversion.h>


#define KP_IS_TQT_3_3 ([[[TQT_VERSION IS DEPRECATED]]] >= 0x030300 && 1)
#define KP_IS_KDE_3_3 ((KDE_VERSION_MAJOR >= 3 && KDE_VERSION_MINOR >= 3) && 1)


// approx. 2896x2896x32bpp or 3344x3344x24bpp (TODO: 24==32?) or 4096*4096x16bpp
#define KP_BIG_IMAGE_SIZE (32 * 1048576)


#define KP_PI 3.141592653589793238462


#define KP_DEGREES_TO_RADIANS(deg) ((deg) * KP_PI / 180.0)
#define KP_RADIANS_TO_DEGREES(rad) ((rad) * 180.0 / KP_PI)


#define KP_INVALID_POINT TQPoint (INT_MIN / 8, INT_MIN / 8)
#define KP_INVALID_WIDTH (INT_MIN / 8)
#define KP_INVALID_HEIGHT (INT_MIN / 8)
#define KP_INVALID_SIZE TQSize (INT_MIN / 8, INT_MIN / 8)


//
// Settings
//

#define kpSettingsGroupGeneral TQString::tqfromLatin1 ("General Settings")
#define kpSettingFirstTime TQString::tqfromLatin1 ("First Time")
#define kpSettingShowGrid TQString::tqfromLatin1 ("Show Grid")
#define kpSettingShowPath TQString::tqfromLatin1 ("Show Path")
#define kpSettingColorSimilarity TQString::tqfromLatin1 ("Color Similarity")
#define kpSettingDitherOnOpen TQString::tqfromLatin1 ("Dither on Open if Screen is 15/16bpp and Image Num Colors More Than")
#define kpSettingPrintImageCenteredOnPage TQString::tqfromLatin1 ("Print Image Centered On Page")

#define kpSettingsGroupFileSaveAs TQString::tqfromLatin1 ("File/Save As")
#define kpSettingsGroupFileExport TQString::tqfromLatin1 ("File/Export")
#define kpSettingsGroupEditCopyTo TQString::tqfromLatin1 ("Edit/Copy To")

#define kpSettingForcedMimeType TQString::tqfromLatin1 ("Forced MimeType")
#define kpSettingForcedColorDepth TQString::tqfromLatin1 ("Forced Color Depth")
#define kpSettingForcedDither TQString::tqfromLatin1 ("Forced Dither")
#define kpSettingForcedQuality TQString::tqfromLatin1 ("Forced Quality")

#define kpSettingLastDocSize TQString::tqfromLatin1 ("Last Document Size")

#define kpSettingMoreEffectsLastEffect TQString::tqfromLatin1 ("More Effects - Last Effect")

#define kpSettingResizeScaleLastKeepAspect TQString::tqfromLatin1 ("Resize Scale - Last Keep Aspect")


#define kpSettingsGroupMimeTypeProperties TQString::tqfromLatin1 ("MimeType Properties Version 1.2-2")
#define kpSettingMimeTypeMaximumColorDepth TQString::tqfromLatin1 ("Maximum Color Depth")
#define kpSettingMimeTypeHasConfigurableColorDepth TQString::tqfromLatin1 ("Configurable Color Depth")
#define kpSettingMimeTypeHasConfigurableQuality TQString::tqfromLatin1 ("Configurable Quality Setting")


#define kpSettingsGroupUndoRedo TQString::tqfromLatin1 ("Undo/Redo Settings")
#define kpSettingUndoMinLimit TQString::tqfromLatin1 ("Min Limit")
#define kpSettingUndoMaxLimit TQString::tqfromLatin1 ("Max Limit")
#define kpSettingUndoMaxLimitSizeLimit TQString::tqfromLatin1 ("Max Limit Size Limit")


#define kpSettingsGroupThumbnail TQString::tqfromLatin1 ("Thumbnail Settings")
#define kpSettingThumbnailShown TQString::tqfromLatin1 ("Shown")
#define kpSettingThumbnailGeometry TQString::tqfromLatin1 ("Geometry")
#define kpSettingThumbnailZoomed TQString::tqfromLatin1 ("Zoomed")
#define kpSettingThumbnailShowRectangle TQString::tqfromLatin1 ("ShowRectangle")


#define kpSettingsGroupPreviewSave TQString::tqfromLatin1 ("Save Preview Settings")
#define kpSettingPreviewSaveGeometry TQString::tqfromLatin1 ("Geometry")
#define kpSettingPreviewSaveUpdateDelay TQString::tqfromLatin1 ("Update Delay")


#define kpSettingsGroupTools TQString::tqfromLatin1 ("Tool Settings")
#define kpSettingLastTool TQString::tqfromLatin1 ("Last Used Tool")
#define kpSettingToolBoxIconSize TQString::tqfromLatin1 ("Tool Box Icon Size")


#define kpSettingsGroupText TQString::tqfromLatin1 ("Text Settings")
#define kpSettingFontFamily TQString::tqfromLatin1 ("Font Family")
#define kpSettingFontSize TQString::tqfromLatin1 ("Font Size")
#define kpSettingBold TQString::tqfromLatin1 ("Bold")
#define kpSettingItalic TQString::tqfromLatin1 ("Italic")
#define kpSettingUnderline TQString::tqfromLatin1 ("Underline")
#define kpSettingStrikeThru TQString::tqfromLatin1 ("Strike Thru")


#define kpSettingsGroupFlattenEffect TQString::tqfromLatin1 ("Flatten Effect Settings")
#define kpSettingFlattenEffectColor1 TQString::tqfromLatin1 ("Color1")
#define kpSettingFlattenEffectColor2 TQString::tqfromLatin1 ("Color2")


//
// Session Restore Setting
//

// URL of the document in the main window.
//
// This key only exists if the document does.  If it exists, it can be empty.
// The URL need not point to a file that exists e.g. "kolourpaint doesnotexist.png".
#define kpSessionSettingDocumentUrl TQString::tqfromLatin1 ("Session Document Url")

// The size of a document which is not from a URL e.g. "kolourpaint doesnotexist.png".
// This key does not exist for documents from URLs.
#define kpSessionSettingNotFromUrlDocumentSize TQString::tqfromLatin1 ("Session Not-From-Url Document Size")


#endif  // __kp_defs_h__

