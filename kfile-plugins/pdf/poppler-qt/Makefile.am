INCLUDES =					\
	-I$(top_srcdir)				\
	-I/usr/include/poppler			\
	$(cairo_includes)			\
	$(QT_INCLUDES)				\
	$(FREETYPE_CFLAGS)			\
	$(FONTCONFIG_CFLAGS)


poppler_includedir = $(includedir)/poppler/qt3
poppler_include_HEADERS =			\
	poppler-qt.h				\
	poppler-page-transition.h               \
	poppler-link-qt3.h

lib_LTLIBRARIES = libpoppler-qt.la
libpoppler_qt_la_SOURCES =			\
	poppler-document.cc			\
	poppler-fontinfo.cc			\
	poppler-link.cc                         \
	poppler-page.cc				\
	poppler-page-transition.cc		\
	poppler-page-transition-private.h	\
	poppler-private.cc			\
	poppler-private.h

libpoppler_qt_la_LIBADD = 			\
	-lpoppler				\
	$(QT_LIBS)				\
	$(FREETYPE_LIBS)			\
	$(FONTCONFIG_LIBS)

# libpoppler_qt_la_LDFLAGS = -version-info 2:0:0 @create_shared_lib@

test_poppler_qt_SOURCES =			\
       test-poppler-qt.cpp

test_poppler_qt_LDADD =				\
	$(top_builddir)/poppler/libpoppler.la	\
	libpoppler-qt.la			\
	$(QT_LIBS)				\
	$(FREETYPE_LIBS)
