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
  ${GPHOTO2_INCLUDE_DIRS}
)


link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### kamera (module) ############################

tde_add_kpart( kio_kamera AUTOMOC
  SOURCES kamera.cpp
  LINK kio-shared ${GPHOTO2_LIBRARIES}
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
