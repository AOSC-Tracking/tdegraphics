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
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### kolourpaintcursors (static) ###############

tde_add_library( kolourpaintcursors STATIC_PIC
  SOURCES kpcursorlightcross.cpp kpcursorprovider.cpp
)
