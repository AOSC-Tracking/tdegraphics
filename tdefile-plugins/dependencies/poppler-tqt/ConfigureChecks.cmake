# Poppler
pkg_search_module( POPPLER poppler )
if( POPPLER_FOUND )
  set( HAVE_POPPLER 1 )
else( )
  tde_message_fatal( "poppler is required, but was not found on your system" )
endif( )

tde_save( CMAKE_REQUIRED_INCLUDES CMAKE_REQUIRED_LIBRARIES )
set( CMAKE_REQUIRED_INCLUDES ${DBUS_TQT_INCLUDE_DIRS} ${TQT_INCLUDE_DIRS} ${TQT_INCLUDE_DIRS} ${POPPLER_INCLUDE_DIRS})
set( CMAKE_REQUIRED_LIBRARIES ${DBUS_TQT_LDFLAGS} ${TQT_LDFLAGS} ${QT_LDFLAGS} ${POPPLER_LDFLAGS} )
check_cxx_source_compiles("
  #include <PSOutputDev.h>
  int main(int, char**) { int a; int b; PSOutputDev *psOut = new PSOutputDev(\"test\", (PDFDoc*)0, (XRef*)0, (Catalog*)0, NULL, 0, 0, psModePS, a, b); } "
  HAVE_POPPLER_016 )
check_cxx_source_compiles("
  #include <PSOutputDev.h>
  int main(int, char**) { int a; int b; PSOutputDev *psOut = new PSOutputDev(\"test\", (PDFDoc*)0, NULL, 0, 0, psModePS, a, b); } "
  HAVE_POPPLER_020 )
check_cxx_source_compiles("
  #include <vector>
  #include <PSOutputDev.h>
  int main(int, char**) { int a; int b; std::vector<int> p; PSOutputDev *psOut = new PSOutputDev(\"test\", (PDFDoc*)0, NULL, p, psModePS, a, b); } "
  HAVE_POPPLER_030 )
tde_restore( CMAKE_REQUIRED_INCLUDES CMAKE_REQUIRED_LIBRARIES )

foreach( _poppler_ver 0.58 0.64 0.70 0.71 )
  string( REPLACE "." "" _poppler_str "${_poppler_ver}" )
  if( NOT DEFINED HAVE_POPPLER_${_poppler_str} )
    message( STATUS "Performing Test HAVE_POPPLER_${_poppler_str}" )
    if( NOT POPPLER_VERSION VERSION_LESS "${_poppler_ver}" )
      set( HAVE_POPPLER_${_poppler_str} 1 CACHE INTERNAL "" FORCE )
      message( STATUS "Performing Test HAVE_POPPLER_${_poppler_str} - Success" )
    else( )
      set( HAVE_POPPLER_${_poppler_str} "" CACHE INTERNAL "" FORCE )
      message( STATUS "Performing Test HAVE_POPPLER_${_poppler_str} - Failed" )
    endif( )
  endif( )
endforeach( )
