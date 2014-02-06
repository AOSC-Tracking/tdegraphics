INCLUDES = $(all_includes)
SUBDIRS = pics

bin_PROGRAMS = tdeiconedit

tdeiconedit_SOURCES = utils.cpp main.cpp tdeiconedit.cpp tdeicongrid.cpp \
    tdeiconcolors.cpp kcolorgrid.cpp palettetoolbar.cpp\
    tdeicon.cpp kresize.cpp knew.cpp properties.cpp \
    tdeiconeditslots.cpp tdeiconconfig.cpp 

tdeiconedit_METASOURCES = AUTO

# the library search path. 
tdeiconedit_LDFLAGS = $(all_libraries) $(KDE_RPATH) $(LIB_QT) -lDCOP $(LIB_TDECORE) $(LIB_TDEUI) -ltdefx $(LIB_TDEIO) -ltdetexteditor
tdeiconedit_LDADD = $(LIB_TDEFILE) -ltdeprint

DISTCLEANFILES = *~ .deps/* .libs/*

rcdir = $(kde_datadir)/tdeiconedit
rc_DATA = tdeiconeditui.rc

xdg_apps_DATA = tdeiconedit.desktop

messages: rc.cpp
	$(XGETTEXT) rc.cpp $(tdeiconedit_SOURCES) -o $(podir)/tdeiconedit.pot

