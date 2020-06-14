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

# lcms
check_include_file( lcms2.h HAVE_LCMS2_H )
if( HAVE_LCMS2_H )
  set( LCMS_HEADER "lcms2.h" CACHE INTERNAL "" FORCE )
  set( LCMS_LIBRARY lcms2 )
else( HAVE_LCMS2_H )
  check_include_file( lcms.h HAVE_LCMS_H )
  if( HAVE_LCMS_H )
    set( LCMS_HEADER "lcms.h" CACHE INTERNAL "" FORCE )
    set( LCMS_LIBRARY lcms )
  endif( HAVE_LCMS_H )
endif( HAVE_LCMS2_H )
if( LCMS_HEADER )
  check_library_exists( ${LCMS_LIBRARY} cmsOpenProfileFromFile "" HAVE_LCMS )
endif( )
if( NOT HAVE_LCMS )
  tde_message_fatal( "lcms is required, but was not found on your system" )
endif( )


# libart-2.0
pkg_search_module( ART libart-2.0 )
if( NOT ART_FOUND )
  tde_message_fatal( "libart-2.0 is required, but was not found on your system" )
endif( )


# freetype2 2.2.x
if( NOT ${FREETYPE_VERSION} VERSION_LESS "2.2" )
  set( HAVE_FREETYPE_2_2_x 1 CACHE INTERNAL "" FORCE )
endif( )



# fribidi
pkg_search_module( FRIBIDI fribidi )
if( FRIBIDI_FOUND )
  tde_save_and_set( CMAKE_REQUIRED_INCLUDES ${FRIBIDI_INCLUDE_DIRS} )
  check_include_file( fribidi/fribidi-types.h FRIBIDI_NEW_FILENAME )
  tde_restore( CMAKE_REQUIRED_INCLUDES  )
else( )
  tde_message_fatal( "fribidi is required, but was not found on your system" )
endif( )

