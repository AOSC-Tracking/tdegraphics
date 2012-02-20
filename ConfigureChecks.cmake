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
