INCLUDES =					\
	-I$(top_srcdir)				\
	-I/usr/include/poppler			\
	$(cairo_includes)			\
	$(TQT_INCLUDES)				\
	$(FREETYPE_CFLAGS)			\
	$(FONTCONFIG_CFLAGS)


poppler_includedir = $(includedir)/
poppler_include_HEADERS =			\
	poppler-qt.h				\
	poppler-page-transition.h               \
	poppler-link-qt3.h

lib_LTLIBRARIES = libpoppler-tqt.la
libpoppler_tqt_la_SOURCES =			\
	poppler-document.cpp			\
	poppler-fontinfo.cpp			\
	poppler-link.cpp                         \
	poppler-page.cpp				\
	poppler-page-transition.cpp		\
	poppler-page-transition-private.h	\
	poppler-private.cpp			\
	poppler-private.h

libpoppler_tqt_la_LIBADD = 			\
	-lpoppler				\
	$(TQT_LIBS)				\
	$(FREETYPE_LIBS)			\
	$(FONTCONFIG_LIBS)

test_poppler_tqt_SOURCES =			\
       test-poppler-qt.cpp

test_poppler_tqt_LDADD =			\
	$(top_builddir)/poppler/libpoppler.la	\
	libpoppler-tqt.la			\
	$(TQT_LIBS)				\
	$(FREETYPE_LIBS)
