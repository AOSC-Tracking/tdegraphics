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

# libtiff
check_include_file( tiff.h HAVE_TIFF_H )
if( HAVE_TIFF_H )
  check_library_exists( tiff TIFFOpen "" HAVE_TIFF )
endif( )
if( HAVE_TIFF_H AND HAVE_TIFF )
  set( TIFF_LIBRARY tiff )
else( )
  tde_message_fatal( "libtiff is required, but was not found on your system" )
endif( )
