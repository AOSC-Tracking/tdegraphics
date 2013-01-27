## Makefile.am for ico file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_ico.h

kde_module_LTLIBRARIES = tdefile_ico.la

tdefile_ico_la_SOURCES = tdefile_ico.cpp 
tdefile_ico_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_ico_la_LIBADD = $(LIB_KSYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) tdefile_ico.cpp -o $(podir)/tdefile_ico.pot

services_DATA = tdefile_ico.desktop
servicesdir = $(kde_servicesdir)
