## Makefile.am for xpm file meta info plugin

INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_xpm.h

kde_module_LTLIBRARIES = tdefile_xpm.la

tdefile_xpm_la_SOURCES = tdefile_xpm.cpp
tdefile_xpm_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_xpm_la_LIBADD = $(LIB_TDEIO)
tdefile_xpm_la_METASOURCES = tdefile_xpm.moc

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

kde_services_DATA = tdefile_xpm.desktop

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_xpm.pot
