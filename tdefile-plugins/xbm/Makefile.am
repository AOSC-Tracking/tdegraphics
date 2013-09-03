## Makefile.am for xbm file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_xbm.h

kde_module_LTLIBRARIES = tdefile_xbm.la

tdefile_xbm_la_SOURCES = tdefile_xbm.cpp 
tdefile_xbm_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_xbm_la_LIBADD = $(LIB_TDESYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) tdefile_xbm.cpp -o $(podir)/tdefile_xbm.pot

services_DATA = tdefile_xbm.desktop
servicesdir = $(kde_servicesdir)
