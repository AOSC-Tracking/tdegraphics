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
  ${CMAKE_CURRENT_BINARY_DIR}
  #${CMAKE_CURRENT_SOURCE_DIR}
  #${CMAKE_BINARY_DIR}
  #${CMAKE_SOURCE_DIR}
  #${CMAKE_SOURCE_DIR}
  #${CMAKE_SOURCE_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


#### dscparse (static) ##########################

tde_add_library( dscparse STATIC_PIC
  SOURCES dscparse.cpp dscparse_adapter.cpp
	CXX_FEATURES cxx_nullptr
)
