## Makefile.am for kuickshow

bin_PROGRAMS =
lib_LTLIBRARIES =
tdeinit_LTLIBRARIES = kuickshow.la

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)
KDE_CXXFLAGS = $(IMLIB_CFLAGS)
METASOURCES = AUTO

kuickshow_la_LDFLAGS = $(all_libraries) -module -avoid-version
kuickshow_la_LIBADD  = $(LIB_TDEPRINT) $(LIB_IMLIB) $(LIB_QT) $(LIB_TDECORE) $(LIB_TDEUI) $(LIB_KIO)
kuickshow_la_SOURCES =	kuickshow.cpp \
	aboutwidget.cpp     generalwidget.cpp  kuickconfigdlg.cpp  main.cpp \
	defaultswidget.cpp  imagewindow.cpp    kuickdata.cpp  \
	imdata.cpp filefinder.cpp kurlwidget.cpp filewidget.cpp \
	kuick.cpp imlibwidget.cpp slideshowwidget.cpp printing.cpp \
	kuickfile.cpp kuickimage.cpp filecache.cpp

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(METASOURCES)

messages:
	$(XGETTEXT) *.cpp *.h -o $(podir)/kuickshow.pot

KDE_ICON = kuickshow

 # this is where the kdelnk file will go 
xdg_apps_DATA = kuickshow.desktop
