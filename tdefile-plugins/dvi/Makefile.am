## Makefile.am for the dvi file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_dvi.h

kde_module_LTLIBRARIES = tdefile_dvi.la

tdefile_dvi_la_SOURCES = tdefile_dvi.cpp
tdefile_dvi_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_dvi_la_LIBADD = $(LIB_TDEIO)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: 
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_dvi.pot

services_DATA = tdefile_dvi.desktop
servicesdir = $(kde_servicesdir)
