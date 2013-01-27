## Makefile.am for the pdf file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes) -I$(top_srcdir)/tdefile-plugins/dependencies/poppler-tqt

# these are the headers for your project
noinst_HEADERS   = tdefile_pdf.h

kde_module_LTLIBRARIES = tdefile_pdf.la

tdefile_pdf_la_SOURCES = tdefile_pdf.cpp
tdefile_pdf_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
tdefile_pdf_la_LIBADD = $(LIB_KIO) -L$(top_builddir)/tdefile-plugins/dependencies/poppler-tqt/ -lpoppler-tqt

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: 
	$(XGETTEXT) *.cpp -o $(podir)/tdefile_pdf.pot

services_DATA = tdefile_pdf.desktop
servicesdir = $(kde_servicesdir)
