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
  ${CMAKE_BINARY_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)


##### splash (static) ###########################

tde_add_library( splash STATIC_PIC
  SOURCES
    Splash.cpp SplashBitmap.cpp SplashClip.cpp SplashFTFont.cpp
    SplashFTFontEngine.cpp SplashFTFontFile.cpp SplashFont.cpp
    SplashFontEngine.cpp SplashFontFile.cpp SplashFontFileID.cpp
    SplashPath.cpp SplashPattern.cpp SplashScreen.cpp SplashState.cpp
    SplashT1Font.cpp SplashT1FontEngine.cpp SplashT1FontFile.cpp
    SplashXPath.cpp SplashXPathScanner.cpp
  LINK ${FREETYPE_LIBRARIES} ${FONTCONFIG_LIBRARIES}
)
