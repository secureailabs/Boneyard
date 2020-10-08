MODULESRCDIR = .

subdir = Modules
SGX_ROOT ?= ../../..
sgxroot = $(shell realpath --relative-to=. $(SGX_ROOT))
PY_BUILD = $(sgxroot)/build/gt_enclave/pyvm/$(subdir)

include $(sgxroot)/make/defaults.mk
include $(sgxroot)/make/sgx.mk
include $(sgxroot)/make/pyvm.mk
include make/files.mk
include make/filecc.mk
include make/filecxx.mk
include make/filecpp.mk
include make/fileccx.mk
include make/filexascpp.mk

include make/filedep.mk
include make/fileccdep.mk
include make/filecxxdep.mk
include make/filecppdep.mk
include make/fileccxdep.mk
include make/filexascppdep.mk

OBJECTS_C = $(SOURCES_MODULES_C:%.c=$(PY_BUILD)/%.o) 
OBJECTS_CXX = $(SOURCES_MODULES_CXX:%.cxx=$(PY_BUILD)/%.o)
OBJECTS_CPP = $(SOURCES_MODULES_CPP:%.cpp=$(PY_BUILD)/%.o) 
OBJECTS_CCX = $(SOURCES_MODULES_CCX:%.c=$(PY_BUILD)/%.o) 
OBJECTS_CASCPP = $(SOURCES_MODULES_CASCPP:%.c=$(PY_BUILD)/%.o) 
OBJECTS_CC = $(SOURCES_MODULES_CC:%.cc=$(PY_BUILD)/%.o)
HEADERS =

OBJECTS = $(OBJECTS_C) $(OBJECTS_CXX) $(OBJECTS_CPP) $(OBJECTS_CCX) $(OBJECTS_CASCPP) $(OBJECTS_CC)
SUBDIRS_WITH_NO_BUILD = standalone

MODULE_INC_PATH := -I. -I../../include -I./numpy/f2py/src -I./pandas/_libs/src -I./pandas/_libs/tslibs -I./pandas/_libs/src/klib -I./matplotlib/extern/agg24-svn/include/ -I./matplotlib/extern/ -I./matplotlib -I$(shell pwd)/../Include -I./xgboost/dmlc-core/include -I$(sgxroot)/gt_enclave/trusted_libraries  

all: $(OBJECTS)

$(OBJECTS_C):
	@mkdir -p $(@D)
	$(PY_CC) $(Enclave_C_Flags) $(PY_CFLAGS) $(MODULE_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

$(OBJECTS_CXX):
	@mkdir -p $(@D)
	$(PY_CXX) $(Enclave_Cpp_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

$(OBJECTS_CC):
	@mkdir -p $(@D)
	$(PY_CXX) $(Enclave_Cpp_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

$(OBJECTS_CPP):
	@mkdir -p $(@D)
	$(PY_CXX) $(Enclave_Cpp_Flags) $(PY_CXXFLAGS) -std=c++11 $(MODULE_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

$(OBJECTS_CCX):
	@mkdir -p $(@D)
	$(PY_CC) $(Enclave_C_Flags) $(PY_CFLAGS) $(MODULE_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

$(OBJECTS_CASCPP):
	@mkdir -p $(@D)
	$(PY_CXX) $(Enclave_C_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

########################################Generate Makedep#######################
dep_build: dep_files dep_filecc dep_filecxx dep_filecpp dep_fileccx dep_filexascpp

dep_files: make/files.mk
	cat make/files.mk > temp_files.mk
	sed -i 's/\$$(MODULESRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_MODULES_C:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	gcc -MT 'pytemp' -M $(Enclave_C_Flags) $(PY_CFLAGS) $(MODULE_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > make/filedep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' make/filedep.mk
	sed -i 's/\busr[^ ]*//g' make/filedep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g'  make/filedep.mk
	sed -i 's/\/ //g' make/filedep.mk
	sed -i '/^ *$$/d' make/filedep.mk
	sed -i 's/ \.\./ \$$\(MODULESRCDIR\)\/\.\./g' make/filedep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(MODULESRCDIR\)\/&/g' make/filedep.mk
	sed -i '/^ *\\/d' make/filedep.mk
	sed -i '/^ \/$$/d' make/filedep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' make/filedep.mk > make/t_filedep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' make/t_filedep.mk > make/filedep.mk
	rm -f temp_files.mk make/t_filedep.mk	

dep_filecc: make/filecc.mk
	cat make/filecc.mk > temp_files.mk
	sed -i 's/\$$(MODULESRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_MODULES_CC:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	$(PY_CXX) -MT 'pytemp' -M $(Enclave_Cpp_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > make/t_fileccdep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' make/fileccdep.mk
	sed -i 's/\busr[^ ]*//g' make/t_fileccdep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' make/t_fileccdep.mk
	sed -i 's/\/ //g' make/t_fileccdep.mk
	sed -i '/^ *$$/d' make/t_fileccdep.mk
	sed -i 's/ \.\./ \$$\(MODULESRCDIR\)\/\.\./g' make/t_fileccdep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(MODULESRCDIR\)\/&/g' make/t_fileccdep.mk
	sed -i '/^ *\\/d' make/t_fileccdep.mk
	sed -i '/^ \/$$/d' make/t_fileccdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' make/fileccdep.mk > make/t_fileccdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' make/t_fileccdep.mk > make/fileccdep.mk
	rm -f temp_files.mk make/t_fileccdep.mk

dep_filecxx: make/filecxx.mk
	cat make/filecxx.mk > temp_files.mk
	sed -i 's/\$$(MODULESRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_MODULES_CXX:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	$(PY_CXX) -MT 'pytemp' -M $(Enclave_Cpp_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > make/filecxxdep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' make/filecxxdep.mk
	sed -i 's/\busr[^ ]*//g' make/filecxxdep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' make/filecxxdep.mk
	sed -i 's/\/ //g' make/filecxxdep.mk
	sed -i '/^ *$$/d' make/filecxxdep.mk
	sed -i 's/ \.\./ \$$\(MODULESRCDIR\)\/\.\./g' make/filecxxdep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(MODULESRCDIR\)\/&/g' make/filecxxdep.mk
	sed -i '/^ *\\/d' make/filecxxdep.mk
	sed -i '/^ \/$$/d' make/filecxxdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' make/filecxxdep.mk > make/t_filecxxdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' make/t_filecxxdep.mk > make/filecxxdep.mk
	rm -f temp_files.mki make/t_filecxxdep.mk	

dep_filecpp: make/filecpp.mk
	cat make/filecpp.mk > temp_files.mk
	sed -i 's/\$$(MODULESRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_MODULES_CPP:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	$(PY_CXX) -MT 'pytemp' -M $(Enclave_Cpp_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > make/filecppdep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' make/filecppdep.mk
	sed -i 's/\busr[^ ]*//g' make/filecppdep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' make/filecppdep.mk
	sed -i 's/\/ //g' make/filecppdep.mk
	sed -i '/^ *$$/d' make/filecppdep.mk
	sed -i 's/ \.\./ \$$\(MODULESRCDIR\)\/\.\./g' make/filecppdep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(MODULESRCDIR\)\/&/g' make/filecppdep.mk
	sed -i '/^ *\\/d' make/filecppdep.mk
	sed -i '/^ \/$$/d' make/filecppdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' make/filecppdep.mk > make/t_filecppdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' make/t_filecppdep.mk > make/filecppdep.mk
	rm -f temp_files.mk make/t_filecppdep.mk	

dep_fileccx: make/fileccx.mk
	cat make/fileccx.mk > temp_files.mk
	sed -i 's/\$$(MODULESRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_MODULES_CCX:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	gcc -MT 'pytemp' -M $(Enclave_C_Flags) $(PY_CFLAGS) $(MODULE_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > make/fileccxdep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' make/fileccxdep.mk
	sed -i 's/\busr[^ ]*//g' make/fileccxdep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' make/fileccxdep.mk
	sed -i 's/\/ //g' make/fileccxdep.mk
	sed -i '/^ *$$/d' make/fileccxdep.mk
	sed -i 's/ \.\./ \$$\(MODULESRCDIR\)\/\.\./g' make/fileccxdep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(MODULESRCDIR\)\/&/g' make/fileccxdep.mk
	sed -i '/^ *\\/d' make/fileccxdep.mk
	sed -i '/^ \/$$/d' make/fileccxdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' make/fileccxdep.mk > make/t_fileccxdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' make/t_fileccxdep.mk > make/fileccxdep.mk
	rm -f temp_files.mk make/t_fileccxdep.mk	

dep_filexascpp: make/filexascpp.mk
	cat make/filexascpp.mk > temp_files.mk
	sed -i 's/\$$(MODULESRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_MODULES_CASCPP:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	$(PY_CXX) -MT 'pytemp' -M $(Enclave_C_Flags) $(PY_CXXFLAGS) $(MODULE_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > make/filexascppdep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' make/filexascppdep.mk
	sed -i 's/\busr[^ ]*//g' make/filexascppdep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' make/filexascppdep.mk
	sed -i 's/\/ //g' make/filexascppdep.mk
	sed -i '/^ *$$/d' make/filexascppdep.mk
	sed -i 's/ \.\./ \$$\(MODULESRCDIR\)\/\.\./g' make/filexascppdep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(MODULESRCDIR\)\/&/g' make/filexascppdep.mk
	sed -i '/^ *\\/d' make/filexascppdep.mk
	sed -i '/^ \/$$/d' make/filexascppdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' make/filexascppdep.mk > make/t_filexascppdep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' make/t_filexascppdep.mk > make/filexascppdep.mk
	rm -f temp_files.mk make/t_filexascppdep.mk	


