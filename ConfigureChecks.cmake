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

##### check for gcc visibility support #########
# FIXME
# This should check for [T]Qt3 visibility support

if( WITH_GCC_VISIBILITY )
  if( NOT UNIX )
    tde_message_fatal(FATAL_ERROR "\ngcc visibility support was requested, but your system is not *NIX" )
  endif( NOT UNIX )
  set( __KDE_HAVE_GCC_VISIBILITY 1 )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif( )


tde_setup_architecture_flags( )

# freetype2
if( BUILD_KDVI OR BUILD_KPDF OR BUILD_KSVG )
  pkg_search_module( FREETYPE freetype2 )
  if( FREETYPE_FOUND )
    set( HAVE_FREETYPE 1 )
  else( )
    tde_message_fatal( "freetype2 is required, but was not found on your system" )
  endif( )
endif( )


# required stuff
find_package( TQt )
find_package( TDE )
