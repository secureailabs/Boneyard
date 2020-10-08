#
# ${R_HOME}/src/include/Makefile


srcdir = .
top_srcdir = ..

top_builddir = ..
subdir = include

include $(top_builddir)/Makeconf


## API(1): for .C() and .Call() writers _or_ for alternative front ends.
SRC_HEADERS = R.h S.h Rdefines.h Rembedded.h Rinternals.h Rinterface.h
## API(2) {these are built, system-dependently}:
OBJ_HEADERS = Rconfig.h Rmath.h Rversion.h
## Non-API internal ones:
INT_HEADERS = Defn.h Errormsg.h Fileio.h Graphics.h GraphicsBase.h \
   IOStuff.h Internal.h Parse.h Print.h Rconnections.h \
   Rdynpriv.h Rgraphics.h Rinlinedfuns.h Startup.h rlocale.h

SUBDIRS = R_ext
SUBDIRS_WITH_NO_BUILD = Rmodules vg

TIMESTAMPS = $(SRC_HEADERS:.h=.ts) $(OBJ_HEADERS:.h=.tsa)

CLEANFILES = $(OBJ_HEADERS) stamp-R $(TIMESTAMPS) libintl.h
#DISTCLEANFILES = Makefile Rmath.h0 config.h stamp-h
DISTCLEANFILES = Rmath.h0 config.h stamp-h

.SUFFIXES:
.SUFFIXES: .h .ts .tsa

.h.ts:
	@$(INSTALL_DATA) $< $(top_builddir)/include/`basename $<`
	@touch $@
.h.tsa:
	@$(INSTALL_DATA) $< $(top_builddir)/include/`basename $<`
	@touch $@


all: R

R: stamp-R $(TIMESTAMPS)
	@for d in $(SUBDIRS); do \
	  (cd $${d} && $(MAKE) $@) || exit 1; \
	done
stamp-R:
	@$(MKINSTALLDIRS) $(top_builddir)/include
	@touch $@
$(TIMESTAMPS): stamp-R

config.h: stamp-h
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	@cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/config.h
	@$(ECHO) timestamp > $@ 2> /dev/null
$(srcdir)/config.h.in: # $(srcdir)/stamp-h.in
$(srcdir)/stamp-h.in: # $(top_srcdir)/configure.ac $(top_builddir)/aclocal.m4
	@TBD=`cd $(top_builddir); pwd`; \
	  cd $(top_srcdir) && $(AUTOHEADER) -I $${TBD}
	@$(ECHO) timestamp > $@ 2> /dev/null
Rconfig.h: config.h $(top_srcdir)/tools/GETCONFIG
	@$(SHELL) $(top_srcdir)/tools/GETCONFIG > $@
Rversion.h: $(top_srcdir)/VERSION $(top_srcdir)/tools/GETVERSION $(top_builddir)/SVN-REVISION
	@$(SHELL) $(top_srcdir)/tools/GETVERSION > $@

## <NOTE>
## we don't use AC_CONFIG_HEADERS on Rmath.h.in because
## a) that would comment out #undef statements in Rmath.h.in and
## b) Rmath.h should be a self-contained file for standalone Rmath use.
## </NOTE>
Rmath.h: Rmath.h0
	@$(SHELL) $(top_srcdir)/tools/copy-if-change Rmath.h0 $@
Rmath.h0: $(srcdir)/Rmath.h0.in $(top_builddir)/config.status
	@cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@

mostlyclean: clean
clean:
	@for d in $(SUBDIRS); do (cd $${d} && $(MAKE) $@); done
	-@test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

TAGS info dvi check:

