## Makefile.am for dds file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_dds.h

kde_module_LTLIBRARIES = tdefile_dds.la

tdefile_dds_la_SOURCES = tdefile_dds.cpp 
tdefile_dds_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_dds_la_LIBADD = $(LIB_KSYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) tdefile_dds.cpp -o $(podir)/tdefile_dds.pot

services_DATA = tdefile_dds.desktop
servicesdir = $(kde_servicesdir)
