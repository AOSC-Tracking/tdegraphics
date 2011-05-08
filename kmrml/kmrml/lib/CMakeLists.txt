#################################################
#
#  (C) 2010-2011 Calvin Morrison
#  mutantturkey@gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


#### kcmkgamma (static) #########################

tde_add_library( kmrmlstuff STATIC_PIC AUTOMOC
  SOURCES
    kmrml_config.cpp mrml_shared.cpp
    mrml_utils.cpp watcher_stub.cpp
)
