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

# required stuff

tde_setup_architecture_flags( )

include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)

tde_setup_largefiles( )

find_package( TQt )
find_package( TDE )


##### check for gcc visibility support #########

if( WITH_GCC_VISIBILITY )
  tde_setup_gcc_visibility( )
endif( )


# freetype2
##### check for fontconfig ######################
if( BUILD_KDVI OR BUILD_KPDF OR BUILD_KSVG )
  pkg_search_module( FREETYPE freetype2 )
  if( FREETYPE_FOUND )
    set( HAVE_FREETYPE 1 )
  else( )
    tde_message_fatal( "freetype2 is required, but was not found on your system" )
  endif( )
endif( )


##### check for fontconfig ######################
if( BUILD_KPDF OR BUILD_KSVG )
	pkg_search_module( FONTCONFIG fontconfig )
	if( FONTCONFIG_FOUND )
	  set( HAVE_FONTCONFIG 1 CACHE INTERNAL "" FORCE )
	else( )
		message(FATAL_ERROR "fontconfig is required, but not found on your system" )
	endif( )
endif( )
