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

## test for and explicitly include libXext
if( WITH_XEXT )
  pkg_search_module( XEXT xext )
  if( XEXT_FOUND )
    check_include_file( "X11/extensions/shape.h" HAVE_X11_EXTENSIONS_SHAPE_H )
  endif( )
  if( NOT XEXT_FOUND )
    tde_message_fatal( "You requested Xext support, but was not found on your system." )
  endif( )
endif( )
