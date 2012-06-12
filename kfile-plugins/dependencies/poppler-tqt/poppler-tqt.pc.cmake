prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: poppler-tqt
Description: TQt bindings for poppler
Version: @POPPLER_VERSION@
Requires: poppler = @POPPLER_VERSION@

Libs: -L${libdir} -lpoppler-tqt
Cflags: -I${includedir}
