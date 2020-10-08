#
#  Top Level Makefile for LAPACK
#  Version 3.2.1
#  June 2009
#

all: R
 
R: blaslib lapacklib f2clib #install 

#install:
#	( cd INSTALL; $(MAKE))

blaslib:
	( cd BLAS; $(MAKE))

lapacklib:
	( cd SRC; $(MAKE))

f2clib:
	( cd F2CLIBS; $(MAKE))

clean:
#	( cd INSTALL; $(MAKE) clean )
	( cd BLAS; $(MAKE) clean )
	( cd SRC; $(MAKE) clean )
	( cd F2CLIBS; $(MAKE) clean )
