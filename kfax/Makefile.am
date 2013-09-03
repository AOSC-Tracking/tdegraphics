AM_LDFLAGS = $(all_libraries) $(KDE_RPATH) $(LIB_QT) -lDCOP $(LIB_TDECORE) $(LIB_TDEUI) -ltdefx $(LIB_TDEIO) -ltdetexteditor
LDADD      = $(LIB_TDEFILE) $(LIB_TDEPRINT) -lm
INCLUDES   = $(all_includes)

####### Files
noinst_HEADERS = kfax.h faxexpand.h options.h

kfax_SOURCES   = options.cpp kfax.cpp faxexpand.cpp faxinit.cpp faxinput.cpp \
		 viewfax.cpp kfax_printsettings.cpp

METASOURCES    = AUTO

bin_PROGRAMS   = kfax

xdg_apps_DATA = kfax.desktop

picsdir = $(kde_datadir)/kfax/pics
pics_DATA = kfax.tif kfaxlogo.xpm

rcdir = $(kde_datadir)/kfax
rc_DATA = kfaxui.rc

KDE_ICON = kfax

EXTRA_DIST = $(xdg_apps_DATA) $(pics_DATA) 

messages: rc.cpp
	$(XGETTEXT) $(kfax_SOURCES) -o $(podir)/kfax.pot

####### Explicit dependencies


