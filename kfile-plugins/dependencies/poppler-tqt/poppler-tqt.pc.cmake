prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=@LIB_INSTALL_DIR@
includedir=@INCLUDE_INSTALL_DIR@

Name: poppler-tqt
Description: TQt bindings for poppler
Version: @POPPLER_VERSION@
Requires: poppler = @POPPLER_VERSION@

Libs: -L${libdir} -lpoppler-tqt
Cflags: -I${includedir}
