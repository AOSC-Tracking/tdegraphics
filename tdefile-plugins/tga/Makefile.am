## Makefile.am for tga file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_tga.h

kde_module_LTLIBRARIES = tdefile_tga.la

tdefile_tga_la_SOURCES = tdefile_tga.cpp 
tdefile_tga_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_tga_la_LIBADD = $(LIB_KSYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) tdefile_tga.cpp -o $(podir)/tdefile_tga.pot

services_DATA = tdefile_tga.desktop
servicesdir = $(kde_servicesdir)
