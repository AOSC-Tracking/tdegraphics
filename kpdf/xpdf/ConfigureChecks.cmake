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

pkg_search_module( FREETYPE freetype2 )
if( FREETYPE_FOUND )
  set( HAVE_FREETYPE 1 CACHE INTERNAL "" FORCE )
else( )
  tde_message_fatal( "freetype2 is required, but was not found on your system" )
endif( )


pkg_search_module( XFT xft )
if( NOT XFT_FOUND )
    tde_message_fatal( "Xft is required, but was not found on your system" )
endif( )


find_package( JPEG )
if( NOT JPEG_FOUND )
  tde_message_fatal( "jpeg is required, but was not found on your system" )
endif( )


# check for t1lib
if( WITH_T1LIB )
  check_include_file( t1lib.h HAVE_T1LIB_H )
  if( HAVE_T1LIB_H )
    check_library_exists( t1 T1_InitLib "" HAVE_T1LIB )
  endif( )
  if( HAVE_T1LIB_H AND HAVE_T1LIB )
    set( T1_LIBRARY t1 CACHE INTERNAL "" FORCE )
  else( )
    tde_message_fatal( "t1lib is required, but was not found on your system" )
  endif( )
endif( )


# check for libpaper
if( WITH_PAPER )
  check_include_file( paper.h HAVE_PAPER_H )
  if( HAVE_PAPER_H )
    check_library_exists( paper paperinit "" HAVE_PAPER )
  endif( )
  if( HAVE_PAPER_H AND HAVE_PAPER )
    set( PAPER_LIBRARY paper CACHE INTERNAL "" FORCE )
  else( )
    tde_message_fatal( "libpaper is required, but was not found on your system" )
  endif( )
endif( )
