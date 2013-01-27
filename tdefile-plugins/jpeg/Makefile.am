## Makefile.am for jpeg file meta info plugin

KDE_CXXFLAGS = $(USE_EXCEPTIONS)

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = tdefile_jpeg.h exif.h

kde_module_LTLIBRARIES = tdefile_jpeg.la

tdefile_jpeg_la_SOURCES = tdefile_jpeg.cpp exif.cpp tdefile_setcomment.cpp
tdefile_jpeg_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_jpeg_la_LIBADD = $(LIB_KIO)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_jpeg.pot

services_DATA = tdefile_jpeg.desktop
servicesdir = $(kde_servicesdir)
