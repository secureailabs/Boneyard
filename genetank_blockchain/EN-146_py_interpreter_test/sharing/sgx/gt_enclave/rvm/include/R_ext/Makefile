#
# ${R_HOME}/src/include/R_ext/Makefile


srcdir = .
top_srcdir = ../..

top_builddir = ../..
subdir = include/R_ext

include $(top_builddir)/Makeconf

## Partially included by the API (1) headers in .., but not part of the
## API per se. 
R_EXT_HEADERS = \
  Applic.h Arith.h BLAS.h Boolean.h Callbacks.h Complex.h Connections.h \
  Constants.h Error.h GetX11Image.h \
  GraphicsDevice.h GraphicsEngine.h Itermacros.h \
  Lapack.h Linpack.h MathThreads.h Memory.h QuartzDevice.h \
  Parse.h Print.h PrtUtil.h R-ftp-http.h RS.h Rallocators.h Random.h \
  Rdynload.h Riconv.h RStartup.h Utils.h eventloop.h libextern.h \
  stats_package.h stats_stubs.h Visibility.h

TIMESTAMPS = $(R_EXT_HEADERS:.h=.ts)

CLEANFILES = stamp-R $(TIMESTAMPS)

.SUFFIXES:
.SUFFIXES: .h .ts

.h.ts: 
	@$(INSTALL_DATA) $< $(top_builddir)/include/R_ext/`basename $<`
	@touch $@

all: R

R: stamp-R $(TIMESTAMPS)
stamp-R: 
	@$(MKINSTALLDIRS) $(top_builddir)/include/R_ext
	@touch $@
$(TIMESTAMPS): stamp-R

mostlyclean: clean
clean:
	-@test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

TAGS info dvi check:
