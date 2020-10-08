################################PYVM Objects###################################
PYTHONSRCDIR := $(PYVM)/Python
include $(PYTHONSRCDIR)/files.mk

OBJECTSRCDIR := $(PYVM)/Objects
include $(OBJECTSRCDIR)/files.mk

PARSERSRCDIR := $(PYVM)/Parser
include $(PARSERSRCDIR)/files.mk

MODULESRCDIR := $(PYVM)/Modules
include $(MODULESRCDIR)/make/files.mk
include $(MODULESRCDIR)/make/filecc.mk
include $(MODULESRCDIR)/make/filecxx.mk
include $(MODULESRCDIR)/make/filecpp.mk
include $(MODULESRCDIR)/make/fileccx.mk
include $(MODULESRCDIR)/make/filexascpp.mk

COVERSRCDIR := $(PYVM)/cover
include $(COVERSRCDIR)/files.mk

PYVM_PYTHON_C := $(SOURCES_PYTHON_C)
PYVM_PARSER_C := $(SOURCES_PARSER_C)
PYVM_OBJECTS_C := $(SOURCES_OBJECTS_C)
PYVM_COVER_C := $(SOURCES_COVER_C)
PYVM_MODULES_C :=$(SOURCES_MODULES_C)
PYVM_C := $(PYVM_PYTHON_C) \
	$(PYVM_PARSER_C) \
	$(PYVM_OBJECTS_C) \
	$(PYVM_COVER_C) \
	$(PYVM_MODULES_C)

PYVM_COBJS := $(PYVM_C:.c=.o) \
	$(SOURCES_MODULES_CXX:.cxx=.o) \
	$(SOURCES_MODULES_CPP:.cpp=.o) \
	$(SOURCES_MODULES_CCX:.c=.o) \
	$(SOURCES_MODULES_CASCPP:.c=.o) \
	$(SOURCES_MODULES_CC:.cc=.o)

PYVM_OBJS := $(PYVM_COBJS:%=$(BUILD_DIR)/%)

#######################################RVM Objects#############################
MAINSRCDIR := $(RVM)/main
include $(MAINSRCDIR)/files.mk

NMATHSRCDIR := $(RVM)/nmath
include $(NMATHSRCDIR)/files.mk

APPLSRCDIR := $(RVM)/appl
include $(APPLSRCDIR)/files.mk

STATSRCDIR := $(RVM)/stats
include $(STATSRCDIR)/files.mk

RFSRCDIR := $(RVM)/randomForest
include $(RFSRCDIR)/files.mk

RVM_MAIN_C := $(SOURCES_MAIN_C)
RVM_APPL_C := $(SOURCES_APPL_C)
RVM_NMATH_C := $(SOURCES_NMATH_C)
RVM_STATS_C := $(SOURCES_STATS_C)
RVM_RF_C := $(SOURCES_RF_C)

RVM_C := $(RVM_MAIN_C) \
	$(RVM_APPL_C) \
	$(RVM_NMATH_C) \
	$(RVM_RF_C) \
	$(RVM_STATS_C)

RVM_COBJS := $(RVM_C:.c=.o)
RVM_OBJS := $(RVM_COBJS:%=$(BUILD_DIR)/%)

##############################Trusted Libraries Objects########################
SECP256K1DIR := $(TRUSTED_LIB)/secp256k1/src

FENVSRCDIR := $(TRUSTED_LIB)/fenv
include $(FENVSRCDIR)/files.mk
FENV_OBJS = $(SOURCES_FENV_C:%.c=$(BUILD_DIR)/%.o)

PNGSRCDIR := $(TRUSTED_LIB)/libpng
include $(PNGSRCDIR)/files.mk
PNG_OBJS = $(SOURCES_PNG_C:%.c=$(BUILD_DIR)/%.o)

ZLIBSRCDIR := $(TRUSTED_LIB)/zlib
include $(ZLIBSRCDIR)/files.mk
ZLIB_OBJS = $(SOURCES_ZLIB_C:%.c=$(BUILD_DIR)/%.o)

FT2SRCDIR := $(TRUSTED_LIB)/ft2
include $(FT2SRCDIR)/files.mk
FT2_OBJS = $(SOURCES_FT2_C:%.c=$(BUILD_DIR)/%.o)

LAPACKSRCDIR := $(TRUSTED_LIB)/lapack
include $(LAPACKSRCDIR)/files.mk
LAPACK_OBJS = $(SOURCES_LAPACK_C:%.c=$(BUILD_DIR)/%.o)

PRNGSRCDIR := $(TRUSTED_LIB)/prng
include $(PRNGSRCDIR)/files.mk
PRNG_OBJS = $(SOURCES_PRNG_C:%.c=$(BUILD_DIR)/%.o)

LIBFFISRCDIR := $(TRUSTED_LIB)/libffi
include $(LIBFFISRCDIR)/files.mk
LIBFFI_OBJS = $(SOURCES_LIBFFI_C:%.c=$(BUILD_DIR)/%.o) \
	$(SOURCES_LIBFFI_S:%.s=$(BUILD_DIR)/%.o)

###################################Enclave Objects#############################
Enclave_Cpp_Objects := $(Enclave_Cpp_Files:%.cpp=$(ENCLAVE_BUILD_DIR)/%.o)
Enclave_C_Objects := $(Enclave_C_Files:%.c=$(ENCLAVE_BUILD_DIR)/%.o)

###################################Enclave List Objects########################
Enclave_Objects := $(ENCLAVE_BUILD_DIR)/gt_enclave_t.o \
	$(Enclave_Cpp_Objects) \
	$(Enclave_C_Objects) \
	$(FENV_OBJS) \
        $(PNG_OBJS) \
	$(LAPACK_OBJS) \
	$(LIBFFI_OBJS) \
	$(ZLIB_OBJS)\
        $(PRNG_OBJS) \
	$(BUILD_DIR)/$(SECP256K1DIR)/secp256k1.o \
	$(FT2_OBJS)

