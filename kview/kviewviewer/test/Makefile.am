INCLUDES = -I$(top_srcdir)/kview $(all_includes)

METASOURCES = AUTO

check_PROGRAMS = test

test_SOURCES = main.cpp test.cpp
test_LDFLAGS = $(all_libraries) $(KDE_RPATH) $(LIB_QT) -lDCOP $(LIB_TDECORE) $(LIB_TDEUI) -ltdefx $(LIB_KIO) -lktexteditor
test_LDADD   = $(LIB_KPARTS)
