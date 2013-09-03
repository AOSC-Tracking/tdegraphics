## Makefile.am for PCX file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

noinst_HEADERS   = tdefile_pcx.h

kde_module_LTLIBRARIES = tdefile_pcx.la

tdefile_pcx_la_SOURCES = tdefile_pcx.cpp
tdefile_pcx_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_pcx_la_LIBADD = $(LIB_TDEIO) $(LIBTIFF)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_pcx.pot

services_DATA = tdefile_pcx.desktop
servicesdir = $(kde_servicesdir)
