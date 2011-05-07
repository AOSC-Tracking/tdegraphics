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

cmake_minimum_required( VERSION 2.8 )


##### general package setup #####################

project( kdegraphics )
set( VERSION 3.5.13 )


##### include essential cmake modules ###########

include( FindPkgConfig )
include( CheckIncludeFile )
include( CheckTypeSize )
include( CheckCSourceCompiles )
include( CheckCXXSourceCompiles )
include( CheckLibraryExists )


##### include our cmake modules #################

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
include( TDEMacros )


##### setup install paths #######################

include( TDESetupPaths )
tde_setup_paths( )


##### optional stuff ############################

option( WITH_T1LIB "Enable t1lib support" OFF )
option( WITH_LIBPAPER "Enable libpaper support" OFF )


##### user requested modules ####################


option( BUILD_ALL "Build all" OFF )
option( BUILD_DOC "Build documentation" ${BUILD_ALL} )
option( BUILD_KAMERA "Build kamera" ${BUILD_ALL} )
option( BUILD_KCOLOREDIT "Build kcoloredit" ${BUILD_ALL} )
option( BUILD_KDVI "Build kdvi" ${BUILD_ALL} )
option( BUILD_KVIEW "Build kview" ${BUILD_ALL} )
option( BUILD_KRULER "Build kruler" ${BUILD_ALL} )
option( BUILD_KPDF "Build kpdf" ${BUILD_ALL} )
option( BUILD_KSVG "Build ksvg" ${BUILD_ALL} )
option( BUILD_KVIEWSHELL "Build kviewshell" ${BUILD_ALL} )


##### configure checks ##########################

include( ConfigureChecks.cmake )


###### global compiler settings #################

add_definitions(
  -DHAVE_CONFIG_H
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TQT_CXX_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined" )
set( CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined" )


##### kdegraphics directories ###################

tde_conditional_add_subdirectory( BUILD_DOC doc )
tde_conditional_add_subdirectory( BUILD_KAMERA kamera )
tde_conditional_add_subdirectory( BUILD_KCOLOREDIT kcoloredit )
tde_conditional_add_subdirectory( BUILD_KDVI kdvi )
tde_conditional_add_subdirectory( BUILD_KVIEW kview )
tde_conditional_add_subdirectory( BUILD_KRULER kruler )
tde_conditional_add_subdirectory( BUILD_KPDF kpdf )
tde_conditional_add_subdirectory( BUILD_KSVG ksvg )
tde_conditional_add_subdirectory( BUILD_KVIEWSHELL kviewshell )


##### write configure files #####################

configure_file( config.h.cmake config.h @ONLY )
