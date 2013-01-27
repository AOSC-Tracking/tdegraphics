## Makefile.am for png file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_png.h 

kde_module_LTLIBRARIES = tdefile_png.la

tdefile_png_la_SOURCES = tdefile_png.cpp
tdefile_png_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_png_la_LIBADD = $(LIB_KIO)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_png.pot

services_DATA = tdefile_png.desktop
servicesdir = $(kde_servicesdir)
