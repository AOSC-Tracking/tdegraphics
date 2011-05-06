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

include( CheckFunctionExists )

check_type_size( wchar_t WCHAR_T )

if( HAVE_WCHAR_T )
  tde_save( CMAKE_EXTRA_INCLUDE_FILES )
  set( CMAKE_EXTRA_INCLUDE_FILES wchar.h )
  check_type_size( mbstate_t MBSTATE_T )
  tde_restore( CMAKE_EXTRA_INCLUDE_FILES )
  check_function_exists( iswspace HAVE_ISWSPACE )
  if( HAVE_MBSTATE_T )
    check_function_exists( wcrtomb HAVE_WCRTOMB )
  endif( )
endif( )
