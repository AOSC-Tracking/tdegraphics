## Makefile.am for Tiff file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

noinst_HEADERS   = tdefile_tiff.h

kde_module_LTLIBRARIES = tdefile_tiff.la

tdefile_tiff_la_SOURCES = tdefile_tiff.cpp
tdefile_tiff_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_tiff_la_LIBADD = $(LIB_KIO) $(LIBTIFF)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_tiff.pot

services_DATA = tdefile_tiff.desktop
servicesdir = $(kde_servicesdir)
