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

# poppler-qt
pkg_search_module( POPPLER_QT poppler-qt )
if( NOT POPPLER_QT_FOUND )
  tde_message_fatal( "poppler-qt is required, but was not found on your system" )
endif( )
