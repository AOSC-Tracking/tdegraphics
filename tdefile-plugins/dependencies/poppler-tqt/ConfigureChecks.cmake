# Poppler
pkg_search_module( POPPLER poppler )
if( POPPLER_FOUND )
  set( HAVE_POPPLER 1 )
else( )
  tde_message_fatal( "poppler is required, but was not found on your system" )
endif( )

string( REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)"
        POPPLER_VERSION_SPLIT ${POPPLER_VERSION} )
set( POPPLER_VERSION_MAJOR ${CMAKE_MATCH_1} )
set( POPPLER_VERSION_MINOR ${CMAKE_MATCH_2} )
set( POPPLER_VERSION_PATCH ${CMAKE_MATCH_3} )
math( EXPR POPPLER_VERSION_C "(${POPPLER_VERSION_MAJOR}*1000000) + (${POPPLER_VERSION_MINOR}*1000) + ${POPPLER_VERSION_PATCH}" )
set( POPPLER_VERSION_C ${POPPLER_VERSION_C} CACHE INTERNAL "Poppler library version as code number" )

if( NOT POPPLER_VERSION_C LESS 21012000 )
  set( POPPLER_CXX_FEATURES cxx_std_17 CACHE INTERNAL "C++ standard required by Poppler" )
endif()
