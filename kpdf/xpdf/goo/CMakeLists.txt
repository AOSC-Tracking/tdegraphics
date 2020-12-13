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

include( ConfigureChecks.cmake )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_BINARY_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### goo (static) ##############################

tde_add_library( goo STATIC_PIC
  SOURCES
    GHash.cpp GList.cpp GString.cpp gfile.cpp gmem.cpp gmempp.cpp
)
