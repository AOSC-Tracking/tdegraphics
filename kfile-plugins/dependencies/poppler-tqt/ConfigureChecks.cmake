# Poppler
if( BUILD_KPDF )
  pkg_search_module( POPPLER poppler )
  if( POPPLER_FOUND )
    set( HAVE_POPPLER 1 )
  else( )
    tde_message_fatal( "poppler is required, but was not found on your system" )
  endif( )
endif( )

tde_save( CMAKE_REQUIRED_INCLUDES CMAKE_REQUIRED_LIBRARIES )
set( CMAKE_REQUIRED_INCLUDES ${DBUS_TQT_INCLUDE_DIRS} ${TQT_INCLUDE_DIRS} ${QT_INCLUDE_DIRS} ${POPPLER_INCLUDE_DIRS})
set( CMAKE_REQUIRED_LIBRARIES ${DBUS_TQT_LDFLAGS} ${TQT_LDFLAGS} ${QT_LDFLAGS} ${POPPLER_LDFLAGS} )
check_cxx_source_compiles("
  #include <poppler/PSOutputDev.h>
  int main(int, char**) { int a; int b; PSOutputDev *psOut = new PSOutputDev(\"test\", (PDFDoc*)0, (XRef*)0, (Catalog*)0, NULL, 0, 0, psModePS, a, b); } "
  HAVE_POPPLER_016 )
check_cxx_source_compiles("
  #include <poppler/PSOutputDev.h>
  int main(int, char**) { int a; int b; PSOutputDev *psOut = new PSOutputDev(\"test\", (PDFDoc*)0, NULL, 0, 0, psModePS, a, b); } "
  HAVE_POPPLER_020 )
tde_restore( CMAKE_REQUIRED_INCLUDES CMAKE_REQUIRED_LIBRARIES )

