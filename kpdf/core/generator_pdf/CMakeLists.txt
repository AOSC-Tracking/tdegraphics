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
  ${CMAKE_CURRENT_BINARY_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../xpdf
  ${CMAKE_CURRENT_SOURCE_DIR}/../../xpdf/goo
  ${CMAKE_CURRENT_SOURCE_DIR}/../../xpdf/splash
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### generatorpdf (static) #####################

tde_add_library( generatorpdf STATIC_PIC
  SOURCES
    generator_pdf.cpp gp_outputdev.cpp
  DEPENDENCIES kpdfconf-static
)
