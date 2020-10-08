NMATHSRCDIR = .

subdir = nmath
SGX_ROOT ?= ../../..
sgxroot = $(shell realpath --relative-to=. $(SGX_ROOT))
R_BUILD = $(sgxroot)/build/gt_enclave/rvm/$(subdir)

include $(sgxroot)/make/defaults.mk
include $(sgxroot)/make/sgx.mk
include $(sgxroot)/make/rvm.mk
include files.mk
include filedep.mk

OBJECTS = $(SOURCES_NMATH_C:%.c=$(R_BUILD)/%.o)
HEADERS = bessel.h dpq.h nmath.h nmath2.h

SUBDIRS_WITH_NO_BUILD = standalone

all: $(OBJECTS)

$(OBJECTS):
	@mkdir -p $(@D)
	$(R_CXX) $(Enclave_Cpp_Flags) -std=c++17 -I$(shell pwd)/../include $(R_DEFS) -Wno-write-strings -c $< -o $@
	@echo "R_CXX   <=  $< $@"

dep_build: dep_files

dep_files: files.mk
	cat files.mk > temp_files.mk
	sed -i 's/\$$(NMATHSRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_NMATH_C:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	$(R_CXX) -MT 'rtemp' -M $(Enclave_Cpp_Flags) -std=c++17 -I$(shell pwd)/../include $(R_DEFS) $$(cat temp_files.mk) | sed 's/rtemp: \(.*\)\.c/\1.o:&/g' | sed 's/rtemp://g' > filedep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' filedep.mk
	sed -i 's/\busr[^ ]*//g' filedep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' filedep.mk
	sed -i 's/\/ //g' filedep.mk
	sed -i '/^ *$$/d' filedep.mk
	sed -i 's/ \.\./ \$$\(NMATHSRCDIR\)\/\.\./g' filedep.mk
	sed -i 's/.*\.o/\$$\(R_BUILD\)\/\$$\(NMATHSRCDIR\)\/&/g' filedep.mk
	sed -i '/^ *\\/d' filedep.mk
	sed -i '/^ \/$$/d' filedep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(R/\n\$$(R/;P;D' filedep.mk > t_filedep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' t_filedep.mk > filedep.mk
	rm -f temp_files.mk t_filedep.mk

