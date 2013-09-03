## Makefile.am for pnm file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_pnm.h

kde_module_LTLIBRARIES = tdefile_pnm.la

tdefile_pnm_la_SOURCES = tdefile_pnm.cpp
tdefile_pnm_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_pnm_la_LIBADD = $(LIB_TDEIO)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_pnm.pot

services_DATA = tdefile_pnm.desktop
servicesdir = $(kde_servicesdir)
