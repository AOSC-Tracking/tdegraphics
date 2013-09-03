## Makefile.am for bmp file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_bmp.h

kde_module_LTLIBRARIES = tdefile_bmp.la

tdefile_bmp_la_SOURCES = tdefile_bmp.cpp 
tdefile_bmp_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_bmp_la_LIBADD = $(LIB_TDESYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) tdefile_bmp.cpp -o $(podir)/tdefile_bmp.pot

services_DATA = tdefile_bmp.desktop
servicesdir = $(kde_servicesdir)
