INCLUDES = -I$(srcdir)/.. -I$(srcdir)/../fofi -I$(srcdir)/../splash -I$(srcdir)/../goo $(LIBFREETYPE_CFLAGS) $(XFT_CFLAGS) $(all_includes)

libxpdf_la_LDFLAGS = $(all_libraries)
libxpdf_la_LIBADD = $(LIB_X11) $(LIBFREETYPE_LIBS) $(LIBPAPER_LIBS) $(XFT_LIBS) $(LIBJPEG) ../goo/libgoo.la ../fofi/libfofi.la ../splash/libsplash.la 
libxpdf_la_SOURCES = Annot.cpp Array.cpp BuiltinFont.cpp BuiltinFontTables.cpp \
     Catalog.cpp CharCodeToUnicode.cpp CMap.cpp Decrypt.cpp Dict.cpp \
     FontEncodingTables.cpp Function.cpp Gfx.cpp \
     GfxFont.cpp GfxState.cpp GlobalParams.cpp JArithmeticDecoder.cpp \
     JBIG2Stream.cpp Lexer.cpp Link.cpp NameToCharCode.cpp Object.cpp Outline.cpp \
     OutputDev.cpp PDFDoc.cpp PDFDocEncoding.cpp PreScanOutputDev.cpp PSTokenizer.cpp \
     Page.cpp Parser.cpp PSOutputDev.cpp SecurityHandler.cpp SplashOutputDev.cpp Stream.cpp JPXStream.cpp \
     TextOutputDev.cpp UnicodeMap.cpp UnicodeTypeTable.cpp XRef.cpp

noinst_LTLIBRARIES = libxpdf.la

# This fixes crash in Bug 109015 which i assume is a compiler bug
# as adding some correctly placed printf in SplashOutputDev::convertPath() makes this 
# option unneeded
KDE_CXXFLAGS=$(NOREGMOVE)
