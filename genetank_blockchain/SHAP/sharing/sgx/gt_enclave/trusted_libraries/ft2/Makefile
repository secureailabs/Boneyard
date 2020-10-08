FT2SRCDIR = .

subdir = ft2
sgxroot = ../..

include $(sgxroot)/make/sgx.mk
include files.mk

OBJECTS = $(SOURCES_FT2_C:.c=.o)

SUBDIRS_WITH_NO_BUILD = standalone

all: Pyh

Pyh: $(OBJECTS)

.c.o:
	gcc $(Enclave_C_Flags) -I$(sgxroot)/gt_enclave/trusted_libraries -I../include -Wno-write-strings -c $< -o $@
	
mostlyclean: clean
clean:
	#@(cd standalone; $(MAKE) clean)
	@-rm -rf .libs _libs
	@-rm -f Makedeps *.d *.o *.a *.lo *.la stamp-lo

include filedep.mk
