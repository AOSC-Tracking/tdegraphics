SUBDIRS = dom impl core ecma . plugin test

lib_LTLIBRARIES = libksvg.la
libksvg_la_SOURCES = dummy.cpp
libksvg_la_METASOURCES = AUTO
libksvg_la_LDFLAGS = -version-info 0:1:0 -no-undefined $(all_libraries)
libksvg_la_LIBADD  = dom/libksvgdom.la impl/libksvgdomimpl.la core/libksvgcore.la ecma/libksvgecma.la \
		     $(LCMS_LIBS) impl/libs/xrgbrender/libksvgxrgbrender.la impl/libs/libtext2path/src/libtext2path.la \
			 impl/libs/art_support/libksvgart.la -lkjs $(LIB_TDEHTML) $(LIBART_LIBS) $(FREETYPE_LIBS) $(FONTCONFIG_LIBS)

INCLUDES = -I$(top_srcdir)/ksvg/dom -I$(top_srcdir)/ksvg/impl $(all_includes)

dummy.cpp:
	echo "" > dummy.cpp

# Make it easy for developers :)
hashtables:
	cd $(srcdir); \
	rm -f data/*lut* ; \
	../../tdelibs/kjs/create_hash_table impl/SVGElementImpl.cpp > data/SVGElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGSVGElementImpl.cpp > data/SVGSVGElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGRectElementImpl.cpp > data/SVGRectElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGLineElementImpl.cpp > data/SVGLineElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGCircleElementImpl.cpp > data/SVGCircleElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGImageElementImpl.cpp > data/SVGImageElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGEllipseElementImpl.cpp > data/SVGEllipseElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedPointsImpl.cpp > data/SVGAnimatedPointsImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPointImpl.cpp > data/SVGPointImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGRectImpl.cpp > data/SVGRectImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGNumberImpl.cpp > data/SVGNumberImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedAngleImpl.cpp > data/SVGAnimatedAngleImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedRectImpl.cpp > data/SVGAnimatedRectImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedNumberImpl.cpp > data/SVGAnimatedNumberImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedNumberListImpl.cpp > data/SVGAnimatedNumberListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedIntegerImpl.cpp > data/SVGAnimatedIntegerImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedBooleanImpl.cpp > data/SVGAnimatedBooleanImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedEnumerationImpl.cpp > data/SVGAnimatedEnumerationImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedStringImpl.cpp > data/SVGAnimatedStringImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedLengthImpl.cpp > data/SVGAnimatedLengthImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedLengthListImpl.cpp > data/SVGAnimatedLengthListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedPreserveAspectRatioImpl.cpp > data/SVGAnimatedPreserveAspectRatioImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPreserveAspectRatioImpl.cpp > data/SVGPreserveAspectRatioImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGLengthImpl.cpp > data/SVGLengthImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGMatrixImpl.cpp > data/SVGMatrixImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAngleImpl.cpp > data/SVGAngleImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGLocatableImpl.cpp > data/SVGLocatableImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTestsImpl.cpp > data/SVGTestsImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGLangSpaceImpl.cpp > data/SVGLangSpaceImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGExternalResourcesRequiredImpl.cpp > data/SVGExternalResourcesRequiredImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGURIReferenceImpl.cpp > data/SVGURIReferenceImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPaintImpl.cpp > data/SVGPaintImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGColorImpl.cpp > data/SVGColorImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGICCColorImpl.cpp > data/SVGICCColorImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTextPositioningElementImpl.cpp > data/SVGTextPositioningElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTextContentElementImpl.cpp > data/SVGTextContentElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTransformImpl.cpp > data/SVGTransformImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTransformableImpl.cpp > data/SVGTransformableImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPointListImpl.cpp > data/SVGPointListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGNumberListImpl.cpp > data/SVGNumberListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGLengthListImpl.cpp > data/SVGLengthListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGStringListImpl.cpp > data/SVGStringListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedPathDataImpl.cpp > data/SVGAnimatedPathDataImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegImpl.cpp > data/SVGPathSegImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegArcImpl.cpp > data/SVGPathSegArcImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegMovetoImpl.cpp > data/SVGPathSegMovetoImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegLinetoImpl.cpp > data/SVGPathSegLinetoImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegLinetoHorizontalImpl.cpp > data/SVGPathSegLinetoHorizontalImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegLinetoVerticalImpl.cpp > data/SVGPathSegLinetoVerticalImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegCurvetoCubicImpl.cpp > data/SVGPathSegCurvetoCubicImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegCurvetoCubicSmoothImpl.cpp > data/SVGPathSegCurvetoCubicSmoothImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegCurvetoQuadraticImpl.cpp > data/SVGPathSegCurvetoQuadraticImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegCurvetoQuadraticSmoothImpl.cpp > data/SVGPathSegCurvetoQuadraticSmoothImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathElementImpl.cpp > data/SVGPathElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPathSegListImpl.cpp > data/SVGPathSegListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTransformListImpl.cpp > data/SVGTransformListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimatedTransformListImpl.cpp > data/SVGAnimatedTransformListImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAElementImpl.cpp > data/SVGAElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGUseElementImpl.cpp > data/SVGUseElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGDocumentImpl.cpp > data/SVGDocumentImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGForeignObjectElementImpl.cpp > data/SVGForeignObjectElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGScriptElementImpl.cpp > data/SVGScriptElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGStyleElementImpl.cpp > data/SVGStyleElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGZoomAndPanImpl.cpp > data/SVGZoomAndPanImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGFitToViewBoxImpl.cpp > data/SVGFitToViewBoxImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGStylableImpl.cpp > data/SVGStylableImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGEventImpl.cpp > data/SVGEventImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGEcma.cpp > data/SVGEcma.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGMarkerElementImpl.cpp > data/SVGMarkerElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGViewElementImpl.cpp > data/SVGViewElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTextContentElementImpl.cpp > data/SVGTextContentElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGClipPathElementImpl.cpp > data/SVGClipPathElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGColorProfileElementImpl.cpp > data/SVGColorProfileElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGPatternElementImpl.cpp > data/SVGPatternElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGGradientElementImpl.cpp > data/SVGGradientElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGLinearGradientElementImpl.cpp > data/SVGLinearGradientElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGRadialGradientElementImpl.cpp > data/SVGRadialGradientElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGStopElementImpl.cpp > data/SVGStopElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGAnimationElementImpl.cpp > data/SVGAnimationElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGCursorElementImpl.cpp > data/SVGCursorElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGSymbolElementImpl.cpp > data/SVGSymbolElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGMaskElementImpl.cpp > data/SVGMaskElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGGlyphElementImpl.cpp > data/SVGGlyphElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGMissingGlyphElementImpl.cpp > data/SVGMissingGlyphElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGGlyphRefElementImpl.cpp > data/SVGGlyphRefElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGTextPathElementImpl.cpp > data/SVGTextPathElementImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table impl/SVGZoomEventImpl.cpp > data/SVGZoomEventImpl.lut.h; \
	../../tdelibs/kjs/create_hash_table ecma/ksvg_window.cpp > data/ksvg_window.lut.h;

generatedata:
	cd $(srcdir); \
	rm impl/generateddata.cpp -f; \
	cd impl && ../scripts/generate.pl

redo:
	make hashtables && make generatedata
