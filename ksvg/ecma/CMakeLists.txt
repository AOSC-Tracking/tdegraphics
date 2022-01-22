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

# FIXME KDE_CXXFLAGS = $(USE_EXCEPTIONS)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../core
  ${CMAKE_CURRENT_SOURCE_DIR}/../dom
  ${CMAKE_CURRENT_SOURCE_DIR}/../data
  ${CMAKE_CURRENT_SOURCE_DIR}/../impl
  ${CMAKE_CURRENT_SOURCE_DIR}/../impl/libs/libtext2path/src
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
)


##### ksvgecma (static) #########################

tde_add_library( ksvgecma STATIC_PIC AUTOMOC
  SOURCES
    ksvg_scriptinterpreter.cpp ksvg_ecma.cpp ksvg_helper.cpp
    ksvg_ecmaeventlistener.cpp ksvg_window.cpp
  CXX_FEATURES cxx_nullptr
)
