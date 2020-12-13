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
  ${CMAKE_BINARY_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### fofi (static) #############################

tde_add_library( fofi STATIC_PIC
  SOURCES
    FoFiBase.cpp FoFiEncodings.cpp FoFiTrueType.cpp FoFiType1.cpp
    FoFiType1C.cpp
)
