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

pkg_search_module( GPHOTO2 libgphoto2 )
if( NOT GPHOTO2_FOUND )
  tde_message_fatal( "libgphoto2 is required, but was not found on your system" )
endif( )

check_library_exists(gphoto2_port gp_port_info_get_name "" GPHOTO2_5)
if(GPHOTO2_5)
  message(STATUS "  found libgphoto2 2.5")
else(GPHOTO2_5)
  message(STATUS "  assuming libgphoto2 2.4")
endif(GPHOTO2_5)
set(HAVE_GPHOTO2_5 ${GPHOTO2_5} CACHE FILEPATH "")
