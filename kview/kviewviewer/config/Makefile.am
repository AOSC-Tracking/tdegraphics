INCLUDES = $(all_includes)

METASOURCES = AUTO

kde_module_LTLIBRARIES = kcm_kviewviewerpluginsconfig.la

noinst_HEADERS = kviewviewerpluginsconfig.h

kcm_kviewviewerpluginsconfig_la_SOURCES = kviewviewerpluginsconfig.cpp
kcm_kviewviewerpluginsconfig_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
kcm_kviewviewerpluginsconfig_la_LIBADD = $(LIB_TDEUTILS)

kcm_kviewviewerpluginsconfig_DATA = kviewviewerpluginsconfig.desktop
kcm_kviewviewerpluginsconfigdir = $(kde_servicesdir)/tdeconfiguredialog

messages: rc.cpp
	$(XGETTEXT) *.cpp *.h -o $(podir)/kcm_kviewviewerpluginsconfig.pot
