## Makefile.am for EXR file meta info plugin

KDE_CXXFLAGS = $(USE_EXCEPTIONS)

# set the include path for X, qt and KDE
INCLUDES         = -Drestrict= $(all_includes) $(EXR_FLAGS) 
# INCLUDES         = $(all_includes) $(EXR_FLAGS) 

# these are the headers for your project
noinst_HEADERS   = tdefile_exr.h 

kde_module_LTLIBRARIES = tdefile_exr.la

tdefile_exr_la_SOURCES = tdefile_exr.cpp
tdefile_exr_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_exr_la_LIBADD = $(LIB_TDEIO) $(LIB_EXR)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_exr.pot

services_DATA = tdefile_exr.desktop
servicesdir = $(kde_servicesdir)
