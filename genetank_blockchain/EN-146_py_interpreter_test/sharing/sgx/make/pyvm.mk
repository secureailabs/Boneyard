PY_BUILD_DIR = $(shell realpath ../../../build)/gt_enclave/pyvm

PY_CC := gcc
PY_CXX := g++

PYVM_INC_FLAGS := -I$(MODULESRCDIR)/numpy/core/include -I$(MODULESRCDIR)/numpy/core/src/common -I$(MODULESRCDIR)/numpy/core/src/multiarray -I$(MODULESRCDIR)/numpy/core/src/npymath -I$(MODULESRCDIR)/numpy/core/src/umath

# Compiler options
OPT = -DNDEBUG -g -fwrapv -O0 -Wall -Wstrict-prototypes -Wno-write-strings
BASECFLAGS = -fno-strict-aliasing
CFLAGS = $(BASECFLAGS) $(OPT) $(PYVM_INC_FLAGS)
PY_DEFS = -DPy_BUILD_CORE

# C flags used for building the interpreter object files
PY_CFLAGS= $(CFLAGS) $(PY_DEFS)
PY_CXXFLAGS= $(PYVM_INC_FLAGS) $(PY_DEFS)

