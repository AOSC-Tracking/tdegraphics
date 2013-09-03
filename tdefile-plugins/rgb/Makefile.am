## Makefile.am for rgb file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_rgb.h

kde_module_LTLIBRARIES = tdefile_rgb.la

tdefile_rgb_la_SOURCES = tdefile_rgb.cpp
tdefile_rgb_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_rgb_la_LIBADD = $(LIB_TDESYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) tdefile_rgb.cpp -o $(podir)/tdefile_rgb.pot

services_DATA = tdefile_rgb.desktop
servicesdir = $(kde_servicesdir)
