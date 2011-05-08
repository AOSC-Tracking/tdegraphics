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

include( CheckIncludeFileCXX )

pkg_search_module( XXF86VM xxf86vm )
if( NOT XXF86VM_FOUND )
  tde_message_fatal( "xxf86vm is required, but was not found on your system" )
endif( )

tde_save( CMAKE_REQUIRED_INCLUDES )
set( CMAKE_REQUIRED_INCLUDES ${TQT_INCLUDE_DIRS} )
check_include_file_cxx( sstream HAVE_SSTREAM )
tde_restore( CMAKE_REQUIRED_INCLUDES )
