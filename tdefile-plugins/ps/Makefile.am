## Makefile.am for the ps file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = -I$(top_srcdir)/kghostview/dscparse $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_ps.h gscreator.h

kde_module_LTLIBRARIES = tdefile_ps.la gsthumbnail.la

tdefile_ps_la_SOURCES = tdefile_ps.cpp
tdefile_ps_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_ps_la_LIBADD = $(LIB_KIO) ../../kghostview/libdscparse.la

gsthumbnail_la_SOURCES = gscreator.cpp
gsthumbnail_la_LIBADD = $(LIB_TDECORE) ../../kghostview/libdscparse.la
gsthumbnail_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: 
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_ps.pot

services_DATA = tdefile_ps.desktop gsthumbnail.desktop
servicesdir = $(kde_servicesdir)
