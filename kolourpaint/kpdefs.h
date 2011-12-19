
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

#include <tdeversion.h>


#define KP_IS_TQT_3_3 ([[[TQT_VERSION IS DEPRECATED]]] >= 0x030300 && 1)
#define KP_IS_KDE_3_3 ((TDE_VERSION_MAJOR >= 3 && TDE_VERSION_MINOR >= 3) && 1)


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

#define kpSettingsGroupGeneral TQString::fromLatin1 ("General Settings")
#define kpSettingFirstTime TQString::fromLatin1 ("First Time")
#define kpSettingShowGrid TQString::fromLatin1 ("Show Grid")
#define kpSettingShowPath TQString::fromLatin1 ("Show Path")
#define kpSettingColorSimilarity TQString::fromLatin1 ("Color Similarity")
#define kpSettingDitherOnOpen TQString::fromLatin1 ("Dither on Open if Screen is 15/16bpp and Image Num Colors More Than")
#define kpSettingPrintImageCenteredOnPage TQString::fromLatin1 ("Print Image Centered On Page")

#define kpSettingsGroupFileSaveAs TQString::fromLatin1 ("File/Save As")
#define kpSettingsGroupFileExport TQString::fromLatin1 ("File/Export")
#define kpSettingsGroupEditCopyTo TQString::fromLatin1 ("Edit/Copy To")

#define kpSettingForcedMimeType TQString::fromLatin1 ("Forced MimeType")
#define kpSettingForcedColorDepth TQString::fromLatin1 ("Forced Color Depth")
#define kpSettingForcedDither TQString::fromLatin1 ("Forced Dither")
#define kpSettingForcedQuality TQString::fromLatin1 ("Forced Quality")

#define kpSettingLastDocSize TQString::fromLatin1 ("Last Document Size")

#define kpSettingMoreEffectsLastEffect TQString::fromLatin1 ("More Effects - Last Effect")

#define kpSettingResizeScaleLastKeepAspect TQString::fromLatin1 ("Resize Scale - Last Keep Aspect")


#define kpSettingsGroupMimeTypeProperties TQString::fromLatin1 ("MimeType Properties Version 1.2-2")
#define kpSettingMimeTypeMaximumColorDepth TQString::fromLatin1 ("Maximum Color Depth")
#define kpSettingMimeTypeHasConfigurableColorDepth TQString::fromLatin1 ("Configurable Color Depth")
#define kpSettingMimeTypeHasConfigurableQuality TQString::fromLatin1 ("Configurable Quality Setting")


#define kpSettingsGroupUndoRedo TQString::fromLatin1 ("Undo/Redo Settings")
#define kpSettingUndoMinLimit TQString::fromLatin1 ("Min Limit")
#define kpSettingUndoMaxLimit TQString::fromLatin1 ("Max Limit")
#define kpSettingUndoMaxLimitSizeLimit TQString::fromLatin1 ("Max Limit Size Limit")


#define kpSettingsGroupThumbnail TQString::fromLatin1 ("Thumbnail Settings")
#define kpSettingThumbnailShown TQString::fromLatin1 ("Shown")
#define kpSettingThumbnailGeometry TQString::fromLatin1 ("Geometry")
#define kpSettingThumbnailZoomed TQString::fromLatin1 ("Zoomed")
#define kpSettingThumbnailShowRectangle TQString::fromLatin1 ("ShowRectangle")


#define kpSettingsGroupPreviewSave TQString::fromLatin1 ("Save Preview Settings")
#define kpSettingPreviewSaveGeometry TQString::fromLatin1 ("Geometry")
#define kpSettingPreviewSaveUpdateDelay TQString::fromLatin1 ("Update Delay")


#define kpSettingsGroupTools TQString::fromLatin1 ("Tool Settings")
#define kpSettingLastTool TQString::fromLatin1 ("Last Used Tool")
#define kpSettingToolBoxIconSize TQString::fromLatin1 ("Tool Box Icon Size")


#define kpSettingsGroupText TQString::fromLatin1 ("Text Settings")
#define kpSettingFontFamily TQString::fromLatin1 ("Font Family")
#define kpSettingFontSize TQString::fromLatin1 ("Font Size")
#define kpSettingBold TQString::fromLatin1 ("Bold")
#define kpSettingItalic TQString::fromLatin1 ("Italic")
#define kpSettingUnderline TQString::fromLatin1 ("Underline")
#define kpSettingStrikeThru TQString::fromLatin1 ("Strike Thru")


#define kpSettingsGroupFlattenEffect TQString::fromLatin1 ("Flatten Effect Settings")
#define kpSettingFlattenEffectColor1 TQString::fromLatin1 ("Color1")
#define kpSettingFlattenEffectColor2 TQString::fromLatin1 ("Color2")


//
// Session Restore Setting
//

// URL of the document in the main window.
//
// This key only exists if the document does.  If it exists, it can be empty.
// The URL need not point to a file that exists e.g. "kolourpaint doesnotexist.png".
#define kpSessionSettingDocumentUrl TQString::fromLatin1 ("Session Document Url")

// The size of a document which is not from a URL e.g. "kolourpaint doesnotexist.png".
// This key does not exist for documents from URLs.
#define kpSessionSettingNotFromUrlDocumentSize TQString::fromLatin1 ("Session Not-From-Url Document Size")


#endif  // __kp_defs_h__

