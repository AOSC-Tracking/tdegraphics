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

cmake_minimum_required( VERSION 2.8.12 )


##### general package setup #####################

project( tdegraphics )
set( VERSION R14.1.0 )


##### include essential cmake modules ###########

include( FindPkgConfig )
include( CheckIncludeFile )
include( CheckTypeSize )
include( CheckCSourceCompiles )
include( CheckCXXSourceCompiles )
include( CheckLibraryExists )
include( CheckSymbolExists )


##### include our cmake modules #################

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
include( TDEMacros )


##### setup install paths #######################

include( TDESetupPaths )
tde_setup_paths( )


##### optional stuff ############################

OPTION( WITH_ALL_OPTIONS "Enable all optional support" OFF )

option( WITH_PAPER "Enable libpaper support" ${WITH_ALL_OPTIONS} )
OPTION( WITH_TIFF "Enable tiff support (tdefile-plugins)" ${WITH_ALL_OPTIONS} )
OPTION( WITH_OPENEXR "Enable openexr support (tdefile-plugins)" ${WITH_ALL_OPTIONS} )
OPTION( WITH_PDF "Enable pdf support (tdefile-plugins)" ${WITH_ALL_OPTIONS} )


##### options comments ##########################

# WITH_PAPER   affects kpdf
# WITH_PAPER   description this library is only used to set some default
#                          parameters of paper according to system settings.
# WITH_TIFF    affects tdefile-plugins(tiff)
# WITH_OPENEXR affects tdefile-plugins(exr)
# WITH_PDF     affects tdefile-plugins(pdf dependencies/poppler-tqt)

# NOTE: It seems that libtiff was planned to be used in
#       kviewshell(plugins/djvu) but isn't yet implemented


##### user requested modules ####################

option( BUILD_ALL "Build all" OFF )
option( BUILD_DOC "Build documentation" ${BUILD_ALL} )
option( BUILD_KAMERA "Build kamera" ${BUILD_ALL} )
option( BUILD_KCOLOREDIT "Build kcoloredit" ${BUILD_ALL} )
option( BUILD_KDVI "Build kdvi" ${BUILD_ALL} )
option( BUILD_KFAX "Build kfax" ${BUILD_ALL} )
option( BUILD_KFAXVIEW "Build kfaxview" ${BUILD_ALL} )
option( BUILD_TDEFILE_PLUGINS "Build tdefile-plugins" ${BUILD_ALL} )
option( BUILD_KGAMMA "Build kgamma" ${BUILD_ALL} )
option( BUILD_KGHOSTVIEW "Build kghostview" ${BUILD_ALL} )
option( BUILD_TDEICONEDIT "Build tdeiconedit" ${BUILD_ALL} )
option( BUILD_KOLOURPAINT "Build kolourpaint" ${BUILD_ALL} )
option( BUILD_KOOKA "Build kooka" ${BUILD_ALL} )
option( BUILD_KPDF "Build kpdf" ${BUILD_ALL} )
option( BUILD_KPOVMODELER "Build kpovmodeler" ${BUILD_ALL} )
option( BUILD_KRULER "Build kruler" ${BUILD_ALL} )
option( BUILD_KSNAPSHOT "Build ksnapshot" ${BUILD_ALL} )
option( BUILD_KSVG "Build ksvg" ${BUILD_ALL} )
option( BUILD_KUICKSHOW "Build kuickshow" ${BUILD_ALL} )
option( BUILD_KVIEW "Build kview" ${BUILD_ALL} )
option( BUILD_KVIEWSHELL "Build kviewshell" ${BUILD_ALL} )
option( BUILD_LIBKSCAN "Build libkscan" ${BUILD_ALL} )

# deprecated
option( BUILD_KMRML "Build kmrml" OFF )


##### configure checks ##########################

include( ConfigureChecks.cmake )


###### global compiler settings #################

add_definitions(
  -DHAVE_CONFIG_H
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TQT_CXX_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )
set( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined" )


##### tdegraphics directories ###################

# tdefile-plugins/ps requires dscparse-static library
if( BUILD_TDEFILE_PLUGINS AND NOT BUILD_KGHOSTVIEW )
  add_subdirectory( kghostview/dscparse )
endif( )

# kdvi and kfax require a static library from kviewshell
# see BUG 1593 for details
if( BUILD_KVIEWSHELL OR BUILD_KDVI OR BUILD_KFAX )
  add_subdirectory( kviewshell )
endif( )

tde_conditional_add_subdirectory( BUILD_DOC doc )
tde_conditional_add_subdirectory( BUILD_KAMERA kamera )
tde_conditional_add_subdirectory( BUILD_KCOLOREDIT kcoloredit )
tde_conditional_add_subdirectory( BUILD_KDVI kdvi )
tde_conditional_add_subdirectory( BUILD_KFAX kfax )
tde_conditional_add_subdirectory( BUILD_KFAXVIEW kfaxview )
tde_conditional_add_subdirectory( BUILD_TDEFILE_PLUGINS tdefile-plugins )
tde_conditional_add_subdirectory( BUILD_KGAMMA kgamma )
tde_conditional_add_subdirectory( BUILD_KGHOSTVIEW kghostview )
tde_conditional_add_subdirectory( BUILD_TDEICONEDIT tdeiconedit )
tde_conditional_add_subdirectory( BUILD_KOLOURPAINT kolourpaint )
tde_conditional_add_subdirectory( BUILD_KOOKA kooka )
tde_conditional_add_subdirectory( BUILD_KPDF kpdf )
tde_conditional_add_subdirectory( BUILD_KPOVMODELER kpovmodeler )
tde_conditional_add_subdirectory( BUILD_KRULER kruler )
tde_conditional_add_subdirectory( BUILD_KSNAPSHOT ksnapshot )
tde_conditional_add_subdirectory( BUILD_KSVG ksvg )
tde_conditional_add_subdirectory( BUILD_KUICKSHOW kuickshow )
tde_conditional_add_subdirectory( BUILD_KVIEW kview )
tde_conditional_add_subdirectory( BUILD_LIBKSCAN libkscan )

# deprecated
tde_conditional_add_subdirectory( BUILD_KMRML kmrml )

##### write configure files #####################

configure_file( config.h.cmake config.h @ONLY )
