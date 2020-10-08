sgxroot = ../../../..
CLAF2CDIR = .

include $(sgxroot)/make/sgx.mk
include $(sgxroot)/make/rvm.mk

CC        = $(R_CXX)
# if no wrapping of the blas library is needed, uncomment next line
#CC        = gcc -DNO_BLAS_WRAP
#CFLAGS    = -O3 -fPIC
CFLAGS    = $(Enclave_Cpp_Flags) -I$(shell pwd)/../INCLUDE $(R_DEFS) -Wno-write-strings
#LOADER   = gcc -fPIC
LOADER    = $(R_CC) $(Enclave_Cpp_Flags) -I$(shell pwd)/../INCLUDE $(R_DEFS) -Wno-write-strings
LOADOPTS  =
#NOOPT    = -O0 -fPIC
NOOPT     = $(Enclave_Cpp_Flags) -I$(shell pwd)/../INCLUDE $(R_DEFS) -Wno-write-strings
DRVCFLAGS = $(CFLAGS)
F2CCFLAGS = $(CFLAGS)

# Unix makefile: see README.
# For C++, first "make hadd".
# If your compiler does not recognize ANSI C, add
#	-DKR_headers
# to the CFLAGS = line below.
# On Sun and other BSD systems that do not provide an ANSI sprintf, add
#	-DUSE_STRLEN
# to the CFLAGS = line below.
# On Linux systems, add
#	-DNON_UNIX_STDIO
# to the CFLAGS = line below.  For libf2c.so under Linux, also add
#	-fPIC
# to the CFLAGS = line below.

include file.mk

all: R
R: $(OFILES)

.SUFFIXES: .c .o

# compile, then strip unnecessary symbols
.c.o:
	$(CC) -c -DSkip_f2c_Undefs $(CFLAGS) $*.c
	ld -r -x -o $*.xxx $*.o
	mv $*.xxx $*.o
## Under Solaris (and other systems that do not understand ld -x),
## omit -x in the ld line above.
## If your system does not have the ld command, comment out
## or remove both the ld and mv lines above.

## Shared-library variant: the following rule works on Linux
## systems.  Details are system-dependent.  Under Linux, -fPIC
## must appear in the CFLAGS assignment when making libf2c.so.
## Under Solaris, use -Kpic in CFLAGS and use "ld -G" instead
## of "cc -shared".



### If your system lacks ranlib, you don't need it; see README.

#f77vers.o: f77vers.c
#	$(CC) -c f77vers.c

#i77vers.o: i77vers.c
#	$(CC) -c i77vers.c


# If your system lacks onexit() and you are not using an
# ANSI C compiler, then you should uncomment the following
# two lines (for compiling main.o):
#main.o: main.c
#	$(CC) -c -DNO_ONEXIT -DSkip_f2c_Undefs main.c
# On at least some Sun systems, it is more appropriate to
# uncomment the following two lines:
#main.o: main.c
#	$(CC) -c -Donexit=on_exit -DSkip_f2c_Undefs main.c

clean:
	rm -f *.o

include filedep.mk
