COVERSRCDIR = .

subdir = cover
SGX_ROOT ?= ../../..
sgxroot = $(shell realpath --relative-to=. $(SGX_ROOT))
PY_BUILD = $(sgxroot)/build/gt_enclave/pyvm/$(subdir)

include $(sgxroot)/make/defaults.mk
include $(sgxroot)/make/sgx.mk
include $(sgxroot)/make/pyvm.mk
include files.mk
include filedep.mk

OBJECTS = $(SOURCES_COVER_C:%.c=$(PY_BUILD)/%.o)
HEADERS =

SUBDIRS_WITH_NO_BUILD = standalone
COVER_INC_PATH = -I. -I$(shell pwd)/../Include -I$(sgxroot)/gt_app/include

all: $(OBJECTS)

$(OBJECTS):
	@mkdir -p $(@D)
	$(PY_CC) $(Enclave_C_Flags) $(PY_CFLAGS) $(COVER_INC_PATH) -c $< -o $@
	@echo "PYTHON_CC   <=  $< $@"

dep_build: dep_files

dep_files: files.mk
	cat files.mk > temp_files.mk
	sed -i 's/\$$(COVERSRCDIR)\///g' temp_files.mk
	sed -i 's/SOURCES_COVER_C:=//g' temp_files.mk
	sed -i 's/ \\//g' temp_files.mk
	$(PY_CC) -MT 'pytemp' -M $(Enclave_C_Flags) $(PY_CFLAGS) $(COVER_INC_PATH) $$(cat temp_files.mk) | sed 's/pytemp: \(.*\)\.c/\1.o:&/g' | sed 's/pytemp://g' > filedep.mk
	sed -i 's/$(shell pwd | sed 's/\//\\\//g')\///g' filedep.mk
	sed -i 's/\busr[^ ]*//g' filedep.mk
	sed -i 's/[^ ]*sgxsdk\/include[^ ]*//g' filedep.mk
	sed -i 's/\/ //g' filedep.mk
	sed -i '/^ *$$/d' filedep.mk
	sed -i 's/ \.\./ \$$\(COVERSRCDIR\)\/\.\./g' filedep.mk
	sed -i 's/.*\.o/\$$\(PY_BUILD\)\/\$$\(COVERSRCDIR\)\/&/g' filedep.mk
	sed -i '/^ *\\/d' filedep.mk
	sed -i '/^ \/$$/d' filedep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n\$$(P/\n\$$(P/;P;D' filedep.mk > t_filedep.mk
	sed -e :n -e '$$!N;/\n.*\n/!{$$!bn};  s/\\\n \///;P;D' t_filedep.mk > filedep.mk
	rm -f temp_files.mk t_filedep.mk   

