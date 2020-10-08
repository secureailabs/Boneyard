LAPACKSRCDIR = .

subdir = lapack
sgxroot = ../..

include $(sgxroot)/make/sgx.mk
include files.mk

#include $(top_builddir)/Makeconf

OBJECTS = $(SOURCES_LAPACK_C:.c=.o)
HEADERS =

SUBDIRS_WITH_NO_BUILD = standalone

all: Pyh

Pyh: $(OBJECTS)

.c.o:
	g++ $(Enclave_C_Flags) -I./ -I../include -Wno-write-strings -c $< -o $@
	
mostlyclean: clean
clean:
	#@(cd standalone; $(MAKE) clean)
	@-rm -rf .libs _libs
	@-rm -f Makedeps *.d *.o *.a *.lo *.la stamp-lo

include filedep.mk
