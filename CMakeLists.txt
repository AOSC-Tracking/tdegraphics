#################################################
#
#  (C) 2010 Serghei Amelian
#  serghei (DOT) amelian (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#################################################

cmake_minimum_required( VERSION 2.6 )


##### general package setup #####################

project( kdegraphics )


##### include essential cmake modules ###########

include( FindPkgConfig )
include( CheckIncludeFile )
include( CheckTypeSize )
# include( CheckCSourceRuns )
include( CheckCSourceCompiles )
include( CheckCXXSourceCompiles )
include( CheckLibraryExists )
# include( CheckFunctionExists )
# include( CheckStructHasMember )


##### include our cmake modules #################

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
include( TDEMacros )


##### setup install paths #######################

include( TDESetupPaths )
tde_setup_paths( )


##### optional stuff ############################

# option( WITH_ALSA "Enable ALSA support" ON )


##### user requested modules ####################

option( BUILD_ALL "Build all" OFF )
option( BUILD_KVIEW "Build kview" ${BUILD_ALL} )


##### configure checks ##########################

include( ConfigureChecks.cmake )


###### global compiler settings #################

add_definitions(
  -DHAVE_CONFIG_H
  ${TQT_CFLAGS_OTHER}
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include tqt.h" )
set( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined" )
set( CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined" )


##### activate dependencies #####################

# if( BUILD_LIBKCAL OR BUILD_LIBKDEPIM OR BUILD_LIBKPIMIDENTITIES OR BUILD_KIOSLAVES OR BUILD_KMAIL )
#   add_subdirectory( libemailfunctions )
# endif()


##### kdemultimedia directories ########################

tde_conditional_add_subdirectory( BUILD_KVIEW kview )


##### write configure files #####################

configure_file( config.h.cmake config.h @ONLY )
