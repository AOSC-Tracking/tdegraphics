#################################################
#
#  (C) 2010-2011 Serghei Amelian
#  serghei (DOT) amelian (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

add_subdirectory( libs )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../core
  ${CMAKE_CURRENT_SOURCE_DIR}/../dom
  ${CMAKE_CURRENT_SOURCE_DIR}/../ecma
  ${CMAKE_CURRENT_SOURCE_DIR}/../data
  ${CMAKE_CURRENT_SOURCE_DIR}/../impl
  ${CMAKE_CURRENT_SOURCE_DIR}/../impl/libs/libtext2path/src
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)


##### ksvgdomimpl (static) ######################

tde_add_library( ksvgdomimpl STATIC_PIC AUTOMOC
  SOURCES
    SVGLengthImpl.cpp SVGNumberImpl.cpp SVGPointImpl.cpp SVGTransformImpl.cpp
    SVGMatrixImpl.cpp SVGRectImpl.cpp SVGAngleImpl.cpp SVGAnimatedLengthImpl.cpp
    SVGAnimatedNumberImpl.cpp SVGAnimatedIntegerImpl.cpp SVGAnimatedBooleanImpl.cpp
    SVGAnimatedEnumerationImpl.cpp SVGAnimatedPreserveAspectRatioImpl.cpp
    SVGAnimatedRectImpl.cpp SVGAnimatedAngleImpl.cpp SVGAnimatedPathDataImpl.cpp
    SVGAnimatedStringImpl.cpp SVGLengthListImpl.cpp SVGNumberListImpl.cpp
    SVGPointListImpl.cpp SVGTransformListImpl.cpp SVGStringListImpl.cpp
    SVGPathSegListImpl.cpp SVGElementInstanceListImpl.cpp
    SVGAnimatedLengthListImpl.cpp SVGAnimatedNumberListImpl.cpp
    SVGAnimatedPointsImpl.cpp SVGAnimatedTransformListImpl.cpp
    SVGShapeImpl.cpp SVGContainerImpl.cpp SVGBBoxTarget.cpp SVGHelperImpl.cpp
    SVGStylableImpl.cpp SVGTransformableImpl.cpp SVGTestsImpl.cpp SVGLangSpaceImpl.cpp
    SVGExternalResourcesRequiredImpl.cpp SVGLocatableImpl.cpp SVGFitToViewBoxImpl.cpp
    SVGPreserveAspectRatioImpl.cpp SVGZoomAndPanImpl.cpp SVGViewSpecImpl.cpp
    SVGElementImpl.cpp SVGElementInstanceImpl.cpp SVGDocumentImpl.cpp
    SVGSVGElementImpl.cpp SVGWindowImpl.cpp SVGDefsElementImpl.cpp
    SVGUseElementImpl.cpp SVGDescElementImpl.cpp SVGTitleElementImpl.cpp
    SVGGElementImpl.cpp SVGSwitchElementImpl.cpp SVGSymbolElementImpl.cpp
    SVGImageElementImpl.cpp SVGURIReferenceImpl.cpp SVGStyleElementImpl.cpp
    SVGCSSRuleImpl.cpp SVGPathElementImpl.cpp SVGPathSegImpl.cpp
    SVGPathSegClosePathImpl.cpp SVGPathSegArcImpl.cpp SVGPathSegMovetoImpl.cpp
    SVGPathSegCurvetoQuadraticImpl.cpp SVGPathSegCurvetoQuadraticSmoothImpl.cpp
    SVGPathSegCurvetoCubicImpl.cpp SVGPathSegCurvetoCubicSmoothImpl.cpp
    SVGPathSegLinetoImpl.cpp SVGPathSegLinetoHorizontalImpl.cpp
    SVGPathSegLinetoVerticalImpl.cpp SVGRectElementImpl.cpp SVGCircleElementImpl.cpp
    SVGEllipseElementImpl.cpp SVGLineElementImpl.cpp SVGPolyElementImpl.cpp
    SVGPolylineElementImpl.cpp SVGPolygonElementImpl.cpp SVGTextElementImpl.cpp
    SVGTSpanElementImpl.cpp SVGTRefElementImpl.cpp SVGTextPositioningElementImpl.cpp
    SVGTextContentElementImpl.cpp SVGTextPathElementImpl.cpp SVGPaintImpl.cpp
    SVGMarkerElementImpl.cpp SVGColorImpl.cpp SVGICCColorImpl.cpp
    SVGColorProfileElementImpl.cpp SVGColorProfileRuleImpl.cpp SVGPaintServerImpl.cpp
    SVGGradientElementImpl.cpp SVGStopElementImpl.cpp SVGLinearGradientElementImpl.cpp
    SVGRadialGradientElementImpl.cpp SVGPatternElementImpl.cpp
    SVGClipPathElementImpl.cpp SVGMaskElementImpl.cpp SVGFilterElementImpl.cpp
    SVGFilterPrimitiveStandardAttributesImpl.cpp SVGFEBlendElementImpl.cpp
    SVGFEColorMatrixElementImpl.cpp SVGFEComponentTransferElementImpl.cpp
    SVGComponentTransferFunctionElementImpl.cpp SVGFEFuncAElementImpl.cpp
    SVGFEFuncBElementImpl.cpp SVGFEFuncGElementImpl.cpp SVGFEFuncRElementImpl.cpp
    SVGFECompositeElementImpl.cpp SVGFEConvolveMatrixElementImpl.cpp
    SVGFEFloodElementImpl.cpp SVGFEGaussianBlurElementImpl.cpp
    SVGFEDiffuseLightingElementImpl.cpp SVGFEDistantLightElementImpl.cpp
    SVGFEPointLightElementImpl.cpp SVGFESpotLightElementImpl.cpp
    SVGFEDisplacementMapElementImpl.cpp SVGFEMergeElementImpl.cpp
    SVGFEMergeNodeElementImpl.cpp SVGFEImageElementImpl.cpp
    SVGFEMorphologyElementImpl.cpp SVGFEOffsetElementImpl.cpp
    SVGFESpecularLightingElementImpl.cpp SVGFETileElementImpl.cpp
    SVGFETurbulenceElementImpl.cpp SVGCursorElementImpl.cpp
    SVGAElementImpl.cpp SVGViewElementImpl.cpp SVGScriptElementImpl.cpp
    SVGEventImpl.cpp SVGZoomEventImpl.cpp SVGEcma.cpp generateddata.cpp
    SVGAnimationElementImpl.cpp SVGAnimateElementImpl.cpp SVGSetElementImpl.cpp
    SVGAnimateMotionElementImpl.cpp SVGAnimateColorElementImpl.cpp
    SVGAnimateTransformElementImpl.cpp SVGMPathElementImpl.cpp SVGTimeScheduler.cpp
    SVGFontElementImpl.cpp SVGAltGlyphElementImpl.cpp SVGAltGlyphDefElementImpl.cpp
    SVGGlyphRefElementImpl.cpp SVGGlyphElementImpl.cpp SVGMissingGlyphElementImpl.cpp
    SVGFontFaceElementImpl.cpp SVGFontFaceFormatElementImpl.cpp
    SVGFontFaceNameElementImpl.cpp SVGFontFaceSrcElementImpl.cpp
    SVGFontFaceUriElementImpl.cpp SVGDefinitionSrcElementImpl.cpp
    SVGHKernElementImpl.cpp SVGVKernElementImpl.cpp SVGMetadataElementImpl.cpp
    SVGForeignObjectElementImpl.cpp svgpathparser.cpp
  LINK kjs-shared
)
