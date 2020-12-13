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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../goo
  ${CMAKE_CURRENT_SOURCE_DIR}/../fofi
  ${CMAKE_CURRENT_SOURCE_DIR}/../splash
  ${CMAKE_BINARY_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)


##### xpdf (static) #############################

tde_add_library( xpdf STATIC_PIC
  SOURCES
    Annot.cpp Array.cpp BuiltinFont.cpp BuiltinFontTables.cpp
    Catalog.cpp CharCodeToUnicode.cpp CMap.cpp Decrypt.cpp Dict.cpp
    FontEncodingTables.cpp Function.cpp Gfx.cpp GfxFont.cpp GfxState.cpp
    GlobalParams.cpp JArithmeticDecoder.cpp JBIG2Stream.cpp Lexer.cpp Link.cpp
    NameToCharCode.cpp Object.cpp Outline.cpp OutputDev.cpp PDFDoc.cpp
    PDFDocEncoding.cpp PreScanOutputDev.cpp PSTokenizer.cpp Page.cpp
    Parser.cpp PSOutputDev.cpp SecurityHandler.cpp SplashOutputDev.cpp
    Stream.cpp JPXStream.cpp TextOutputDev.cpp UnicodeMap.cpp
    UnicodeTypeTable.cpp XRef.cpp
  LINK ${FONTCONFIG_LIBRARIES}
)
