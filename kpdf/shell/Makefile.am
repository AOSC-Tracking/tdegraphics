INCLUDES = -I$(srcdir)/xpdf -I$(srcdir)/xpdf/goo -I$(top_builddir)/kpdf $(all_includes) $(FREETYPE_CFLAGS)

METASOURCES = AUTO

bin_PROGRAMS = kpdf

kpdf_SOURCES = main.cpp shell.cpp
kpdf_LDFLAGS = $(all_libraries) $(KDE_RPATH) $(LIB_TQT) -lDCOP $(LIB_TDECORE) $(LIB_TDEUI) -ltdefx $(LIB_TDEIO) -ltdetexteditor
kpdf_LDADD   = $(LIB_TDEPARTS)

EXTRA_DIST = kpdf.desktop
xdg_apps_DATA = kpdf.desktop

shellrcdir   = $(kde_datadir)/kpdf
shellrc_DATA = shell.rc
