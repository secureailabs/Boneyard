
CLASRCDIR = .
sgxroot = ../../../..

include $(sgxroot)/make/sgx.mk
include $(sgxroot)/make/rvm.mk

CC        = $(R_CC)
# if no wrapping of the blas library is needed, uncomment next line
#CC        = gcc -DNO_BLAS_WRAP
#CFLAGS    = -O3 -fPIC
CFLAGS    = $(Enclave_C_Flags) -I$(shell pwd)/../INCLUDE $(R_DEFS) -Wno-write-strings
#LOADER   = gcc -fPIC
LOADER    = $(R_CC) $(Enclave_C_Flags) -I$(shell pwd)/../INCLUDE $(R_DEFS) -Wno-write-strings
LOADOPTS  =
#NOOPT    = -O0 -fPIC
NOOPT     = $(Enclave_C_Flags) -I$(shell pwd)/../INCLUDE $(R_DEFS) -Wno-write-strings
DRVCFLAGS = $(CFLAGS)
F2CCFLAGS = $(CFLAGS)

include file.mk

all: R

R: $(ALLSRCOBJ)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

slaruv.o: slaruv.c ; $(CC) $(NOOPT)  -c $< -o $@
dlaruv.o: dlaruv.c ; $(CC) $(NOOPT)  -c $< -o $@
sla_wwaddw.o: sla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@
dla_wwaddw.o: dla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@
cla_wwaddw.o: cla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@
zla_wwaddw.o: zla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@

clean:
	rm -f *.o

include filedep.mk
