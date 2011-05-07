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

add_subdirectory( dvi )
add_subdirectory( png )
add_subdirectory( ps )
add_subdirectory( jpeg )
add_subdirectory( xbm )
add_subdirectory( xpm )
add_subdirectory( bmp )
add_subdirectory( tga )
add_subdirectory( rgb )
add_subdirectory( ico )
add_subdirectory( pcx )
add_subdirectory( pnm )
add_subdirectory( dds )
add_subdirectory( gif )
add_subdirectory( raw )

tde_conditional_add_subdirectory( WITH_TIFF tiff )
tde_conditional_add_subdirectory( WITH_OPENEXR exr )
tde_conditional_add_subdirectory( WITH_PDF pdf )
