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

if( NOT SANE_FOUND )
  pkg_search_module( SANE sane-backends )
endif( )

if( NOT SANE_FOUND )
  message( STATUS "checking for 'sane'..." )

  # find sane-config
  find_program( SANE_EXECUTABLE NAMES sane-config )
  if( NOT SANE_EXECUTABLE )
    tde_message_fatal( "sane-config is NOT found.\n sane library is installed?" )
  endif( )

  macro( __run_sane_config  __type __var )
    execute_process(
      COMMAND ${SANE_EXECUTABLE} --${__type}
      OUTPUT_VARIABLE ${__var}
      RESULT_VARIABLE __result
      OUTPUT_STRIP_TRAILING_WHITESPACE )
    if( _result )
      tde_message_fatal( "Unable to run ${SANE_EXECUTABLE}!\n sane library is correctly installed?\n Path to sane-config is corect?" )
    endif( )
  endmacro( )

  __run_sane_config( version SANE_VERSION )
  __run_sane_config( cflags SANE_INCLUDE_DIRS )
  __run_sane_config( libs SANE_LIBRARIES )

  # cleanup
  if( SANE_INCLUDE_DIRS )
    string( REGEX REPLACE "(^| )-I" ";" SANE_INCLUDE_DIRS "${SANE_INCLUDE_DIRS}" )
  else( )
    set( SANE_INCLUDE_DIRS "/usr/include" )
  endif( )
  if( SANE_LIBRARIES )
    ## remove all spaces and replace whitespace with ';'
    string( REGEX REPLACE "[ ]+" ";" SANE_LIBRARIES "${SANE_LIBRARIES}" )
    ## remove all non-library information
    string( REGEX REPLACE ";[-][^l]([^ ;])+" "" SANE_LIBRARIES "${SANE_LIBRARIES}" )
    ## remove multiple ';'
    string( REGEX REPLACE "[;]+" ";" SANE_LIBRARIES "${SANE_LIBRARIES}" )
    ## remove '-l'
    string( REGEX REPLACE "-l" "" SANE_LIBRARIES "${SANE_LIBRARIES}" )
  endif( )
  message( STATUS "  found 'sane', version ${SANE_VERSION}" )

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args( SANE DEFAULT_MSG
                                     SANE_LIBRARIES
                                     SANE_INCLUDE_DIRS )
endif( )
