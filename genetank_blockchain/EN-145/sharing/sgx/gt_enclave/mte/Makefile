MTESRCDIR = .

subdir = mte
sgxroot = ../..

include $(sgxroot)/make/sgx.mk
include files.mk

OBJECTS = $(SOURCES_MTE_CPP:.cpp=.o)

SUBDIRS_WITH_NO_BUILD = standalone

all: Pyh

Pyh: $(OBJECTS)

%.o: %.cpp
	g++ -std=c++11 -I./ -Wno-write-strings -c $< -o $@
	
mostlyclean: clean
clean:
	#@(cd standalone; $(MAKE) clean)
	@-rm -rf .libs _libs
	@-rm -f Makedeps *.d *.o *.a *.lo *.la stamp-lo

include filedep.mk
