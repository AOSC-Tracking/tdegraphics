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
  ${CMAKE_BINARY_DIR}
  ${TQT_INCLUDE_DIRS}
)


#### xf86gammacfg (executable) ##################

tde_add_executable( xf86gammacfg
  SOURCES xf86gammacfg.cpp
  DESTINATION ${BIN_INSTALL_DIR}
)
