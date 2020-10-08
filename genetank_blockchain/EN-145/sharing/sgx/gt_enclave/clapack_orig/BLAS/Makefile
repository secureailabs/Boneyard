CLABLASDIR = .
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

R: $(ALLOBJ)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

include filedep.mk
