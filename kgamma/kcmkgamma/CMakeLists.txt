#################################################
#
#  (C) 2010-2011 Calvin Morrison
#  mutantturkey@gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

add_subdirectory( pics )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

install( FILES kgamma.desktop DESTINATION ${APPS_INSTALL_DIR}/.hidden )


#### kcmkgamma (module) #########################

if( HAVE_SSTREAM )
  set_source_files_properties( xvidextwrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_SSTREAM" )
endif( )

tde_add_kpart( kcm_kgamma AUTOMOC
  SOURCES
    xf86configpath.cpp xvidextwrap.cpp
    displaynumber.cpp gammactrl.cpp kgamma.cpp
  LINK tdeparts-shared ${XXF86VM_LIBRARIES}
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
