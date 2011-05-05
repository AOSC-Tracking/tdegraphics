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
  ${CMAKE_BINARY_DIR}
  ${ART_INCLUDE_DIRS}
)


##### ksvgart (static) ##########################

tde_add_library( ksvgart STATIC_PIC
  SOURCES
    art_render_misc.c art_rgba_svp.c art_misc.c
)
