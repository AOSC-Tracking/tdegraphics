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

lib_LTLIBRARIES = libpoppler-tqt.la
libpoppler_tqt_la_SOURCES =			\
	poppler-document.cc			\
	poppler-fontinfo.cc			\
	poppler-link.cc                         \
	poppler-page.cc				\
	poppler-page-transition.cc		\
	poppler-page-transition-private.h	\
	poppler-private.cc			\
	poppler-private.h

libpoppler_tqt_la_LIBADD = 			\
	-lpoppler				\
	$(QT_LIBS)				\
	$(FREETYPE_LIBS)			\
	$(FONTCONFIG_LIBS)

test_poppler_tqt_SOURCES =			\
       test-poppler-qt.cpp

test_poppler_tqt_LDADD =			\
	$(top_builddir)/poppler/libpoppler.la	\
	libpoppler-tqt.la			\
	$(QT_LIBS)				\
	$(FREETYPE_LIBS)
