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
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)


##### generatorkimgio (static) ##################

tde_add_library( generatorkimgio STATIC_PIC
  SOURCES generator_kimgio.cpp
)
