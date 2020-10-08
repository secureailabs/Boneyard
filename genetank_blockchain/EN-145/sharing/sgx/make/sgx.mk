ifeq ($(SGX_MODE), HW)
ifeq ($(SGX_DEBUG), 1)
  Build_Mode = HW_DEBUG
else ifeq ($(SGX_PRERELEASE), 1)
  Build_Mode = HW_PRERELEASE
else
  Build_Mode = HW_RELEASE
endif
else
ifeq ($(SGX_DEBUG), 1)
  Build_Mode = SIM_DEBUG
else ifeq ($(SGX_PRERELEASE), 1)
  Build_Mode = SIM_PRERELEASE
else
  Build_Mode = SIM_RELEASE
endif
endif

ifneq ($(SGX_MODE), HW)
        Urts_Library_Name := sgx_urts_sim
else
        Urts_Library_Name := sgx_urts
endif

ifeq ($(shell getconf LONG_BIT), 32)
	SGX_ARCH := x86
else ifeq ($(findstring -m32, $(CXXFLAGS)), -m32)
	SGX_ARCH := x86
endif
ifeq ($(SGX_ARCH), x86)
	SGX_COMMON_CFLAGS := -m32
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x86/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x86/sgx_edger8r
else
	SGX_COMMON_CFLAGS := -m64
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib64
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x64/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x64/sgx_edger8r
endif
ifeq ($(SGX_DEBUG), 1)
ifeq ($(SGX_PRERELEASE), 1)
$(error Cannot set SGX_DEBUG and SGX_PRERELEASE at the same time!!)
endif
endif
ifeq ($(SGX_DEBUG), 1)
        SGX_COMMON_CFLAGS += -O0 -g
else
        SGX_COMMON_CFLAGS += -O2
endif
ifeq ($(SUPPLIED_KEY_DERIVATION), 1)
        SGX_COMMON_CFLAGS += -DSUPPLIED_KEY_DERIVATION
endif

######## Enclave Settings ########
CC_BELOW_4_9 := $(shell expr "`$(CC) -dumpversion`" \< "4.9")
ifeq ($(CC_BELOW_4_9), 1)
	Enclave_C_Flags := $(SGX_COMMON_CFLAGS) -nostdinc -fvisibility=hidden -fpie -ffunction-sections -fdata-sections -fstack-protector
else
	Enclave_C_Flags := $(SGX_COMMON_CFLAGS) -nostdinc -fvisibility=hidden -fpie -ffunction-sections -fdata-sections -fstack-protector-strong
endif
GT_Include_Paths := -I$(Enclave_Dir_abs)/include -I$(App_Dir_abs)/include
Enclave_Include_Paths := -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/libcxx $(GT_Include_Paths)
Enclave_C_Flags += -fPIC $(Enclave_Include_Paths) -Iinclude -I$(SGX_SDK)/include/tlibc
Enclave_Cpp_Flags := $(Enclave_C_Flags) -I$(SGX_SDK)/include/libcxx -std=c++11 -nostdinc++
Enclave_R_Cpp_Flags := $(Enclave_C_Flags) -std=c++11 -nostdinc++

ifneq ($(SGX_MODE), HW)
	Trts_Library_Name := sgx_trts_sim
	Service_Library_Name := sgx_tservice_sim
else
	Trts_Library_Name := sgx_trts
	Service_Library_Name := sgx_tservice
endif
Crypto_Library_Name := sgx_tcrypto

