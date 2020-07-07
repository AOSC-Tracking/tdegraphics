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

add_subdirectory( lib )
add_subdirectory( kcontrol )
add_subdirectory( server )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

install( FILES mrml.protocol DESTINATION ${SERVICES_INSTALL_DIR} )

tde_create_translated_desktop(
  SOURCE mrml_part.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  PO_DIR kmrml-desktops
)

tde_create_translated_desktop(
  SOURCE mrml.desktop
  DESTINATION ${MIME_INSTALL_DIR}/text
  PO_DIR kmrml-desktops
)

tde_create_translated_desktop(
  SOURCE mrml-servicemenu.desktop
  DESTINATION ${DATA_INSTALL_DIR}/konqueror/servicemenus
  PO_DIR kmrml-desktops
)


#### tdeio_mrml (module) ##########################

tde_add_kpart( tdeio_mrml AUTOMOC
  SOURCES mrml.cpp
  LINK kmrmlstuff-static tdeio-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### libkmrmlpart (module) #######################

tde_add_kpart( libkmrmlpart AUTOMOC
  SOURCES
    mrml_part.cpp mrml_view.cpp loader.cpp mrml_elements.cpp
    mrml_creator.cpp browser.cpp algorithmdialog.cpp collectioncombo.cpp
    algorithmcombo.cpp propertysheet.cpp
  LINK kmrmlstuff-static tdeparts-shared
  DESTINATION ${PLUGIN_INSTALL_DIR}
)


##### mrmlsearch (tdeinit) ######################

tde_add_tdeinit_executable( mrmlsearch AUTOMOC
  SOURCES mrmlsearch.cpp
  LINK kmrmlstuff-static tdecore-shared
)
