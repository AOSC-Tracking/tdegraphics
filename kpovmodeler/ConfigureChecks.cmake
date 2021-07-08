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

# glu
set( OpenGL_GL_PREFERENCE LEGACY )
find_package( OpenGL )
if( NOT OPENGL_FOUND )
  check_include_file ( GL/glu.h HAVE_GLU )
  if( NOT HAVE_GLU )
    tde_message_fatal( "OpenGL (glu) is required, but was not found on your system" )
  endif ( )
  set( OPENGL_LIBRARIES "-lGL -lGLU" )
endif( )


# xmu
pkg_search_module( XMU xmu )
if( NOT XMU_FOUND )
  tde_message_fatal( "xmu is required, but was not found on your system" )
endif( )
