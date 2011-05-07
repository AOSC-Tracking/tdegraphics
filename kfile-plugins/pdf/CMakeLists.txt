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

include( ConfigureChecks.cmake )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${TDE_INCLUDE_DIR}
  ${TQT_INCLUDE_DIRS}
  ${POPPLER_QT_INCLUDE_DIRS}
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


#### other data #################################

install( FILES kfile_pdf.desktop DESTINATION ${SERVICES_INSTALL_DIR} )


#### kfile_pdf (module) #########################

tde_add_kpart( kfile_pdf AUTOMOC
  SOURCES kfile_pdf.cpp
  LINK kio-shared ${POPPLER_QT_LIBRARIES}
  DESTINATION ${PLUGIN_INSTALL_DIR}
)
