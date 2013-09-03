# $Id$
# Makefile for tdebase/tdeioslave/kamera

INCLUDES= -I$(srcdir)/../.. -I$(srcdir)/.. $(all_includes) $(GPHOTO2_INCS)

####### Files

kde_module_LTLIBRARIES = tdeio_kamera.la

tdeio_kamera_la_SOURCES = kamera.cpp
tdeio_kamera_la_LIBADD  = $(LIB_TDEIO) -lgphoto2
tdeio_kamera_la_LDFLAGS = $(all_libraries) $(GPHOTO2_LIBS) -module $(KDE_PLUGIN)

noinst_HEADERS = kamera.h

kde_services_DATA = camera.protocol

