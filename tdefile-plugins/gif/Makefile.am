## Makefile.am for gif file meta info plugin

KDE_CXXFLAGS = $(USE_EXCEPTIONS)

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_gif.h

kde_module_LTLIBRARIES = tdefile_gif.la

tdefile_gif_la_SOURCES = tdefile_gif.cpp
tdefile_gif_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_gif_la_LIBADD = $(LIB_TDEIO)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_gif.pot

services_DATA = tdefile_gif.desktop
servicesdir = $(kde_servicesdir)
