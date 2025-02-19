#
# Copyright (C) 2011-2018 Intel Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#   * Neither the name of Intel Corporation nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#

######## SGX SDK Settings ########

#SGX_SDK ?= /opt/intel/sgxsdk
SGX_SDK ?= /home/shifa/sgx/sgxsdk
SGX_MODE ?= HW
SGX_ARCH ?= x64
SGX_DEBUG ?= 1

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
######## App Settings ########

ifneq ($(SGX_MODE), HW)
	Urts_Library_Name := sgx_urts_sim
else
	Urts_Library_Name := sgx_urts
endif

App_Src := gt_app/src
App_Cpp_Files := $(App_Src)/ai.cpp $(App_Src)/enclave_app.cpp $(App_Src)/enclave_control.cpp $(App_Src)/ra.cpp $(App_Src)/ra_network.cpp $(App_Src)/tools.cpp $(App_Src)/network_listener.cpp
App_C_Files := $(App_Src)/cJSON.c

App_Include_Paths := -Igt_app/include -I$(SGX_SDK)/include

App_C_Flags := $(SGX_COMMON_CFLAGS) -fPIC -Wno-attributes $(App_Include_Paths)

# Three configuration modes - Debug, prerelease, release
#   Debug - Macro DEBUG enabled.
#   Prerelease - Macro NDEBUG and EDEBUG enabled.
#   Release - Macro NDEBUG enabled.
ifeq ($(SGX_DEBUG), 1)
        App_C_Flags += -DDEBUG -UNDEBUG -UEDEBUG
else ifeq ($(SGX_PRERELEASE), 1)
        App_C_Flags += -DNDEBUG -DEDEBUG -UDEBUG
else
        App_C_Flags += -DNDEBUG -UEDEBUG -UDEBUG
endif

#App_C_Flags += -D_UTF16_STRINGS

REST_SSL_Link_Flags = -L /usr/local/opt/openssl/lib  -lboost_system -lcrypto -lssl -lcpprest -lboost_system -lboost_chrono
#-lboost_thread-mt
App_Cpp_Flags := $(App_C_Flags) -std=c++11
App_Link_Flags := $(SGX_COMMON_CFLAGS) -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -lsgx_uprotected_fs -L. -lsgx_ukey_exchange -lpthread $(REST_SSL_Link_Flags) -Wl,-rpath=$(CURDIR)/sample_libcrypto -Wl,-rpath=$(CURDIR)


ifneq ($(SGX_MODE), HW)
	App_Link_Flags += -lsgx_uae_service_sim
else
	App_Link_Flags += -lsgx_uae_service
endif

App_C_Objects := $(App_C_Files:.c=.o)

App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)

App_Objects := $(App_C_Objects) $(App_Cpp_Objects)

App_Name := app

######## Service Provider Settings ########

ServiceProvider_Cpp_Files := service_provider/ecp.cpp service_provider/ias_ra.cpp service_provider/isa_service.cpp service_provider/network_dealer.cpp service_provider/service_provider.cpp service_provider/sp_app.cpp $(App_Src)/tools.cpp
ServiceProvider_Include_Paths := -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/libcxx -Isample_libcrypto

ServiceProvider_C_Flags := $(SGX_COMMON_CFLAGS) -fPIC -Wno-attributes -I$(SGX_SDK)/include -Isample_libcrypto
# -D_UTF16_STRINGS
ServiceProvider_Cpp_Flags := $(ServiceProvider_C_Flags) -std=c++11
#ServiceProvider_Link_Flags :=  -shared $(SGX_COMMON_CFLAGS) -L$(SGX_LIBRARY_PATH) -lsample_libcrypto -Lsample_libcrypto $(REST_SSL_Link_Flags)
ServiceProvider_Link_Flags :=  $(SGX_COMMON_CFLAGS) -L$(SGX_LIBRARY_PATH) -lsample_libcrypto -Lsample_libcrypto $(REST_SSL_Link_Flags)

ServiceProvider_Cpp_Objects := $(ServiceProvider_Cpp_Files:.cpp=.o)

######## Enclave Settings ########

ifneq ($(SGX_MODE), HW)
	Trts_Library_Name := sgx_trts_sim
	Service_Library_Name := sgx_tservice_sim
else
	Trts_Library_Name := sgx_trts
	Service_Library_Name := sgx_tservice
endif
Crypto_Library_Name := sgx_tcrypto

Enclave_Cpp_Files := gt_enclave/ai/ai.cpp gt_enclave/gt_enclave.cpp gt_enclave/key_management.cpp
Enclave_Include_Paths := -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/libcxx

CC_BELOW_4_9 := $(shell expr "`$(CC) -dumpversion`" \< "4.9")
ifeq ($(CC_BELOW_4_9), 1)
	Enclave_C_Flags := $(SGX_COMMON_CFLAGS) -nostdinc -fvisibility=hidden -fpie -ffunction-sections -fdata-sections -fstack-protector
else
	Enclave_C_Flags := $(SGX_COMMON_CFLAGS) -nostdinc -fvisibility=hidden -fpie -ffunction-sections -fdata-sections -fstack-protector-strong
endif
Enclave_C_Flags += -fPIC $(Enclave_Include_Paths)
Enclave_Cpp_Flags := $(Enclave_C_Flags) -std=c++11 -nostdinc++

# To generate a proper enclave, it is recommended to follow below guideline to link the trusted libraries:
#    1. Link sgx_trts with the `--whole-archive' and `--no-whole-archive' options,
#       so that the whole content of trts is included in the enclave.
#    2. For other libraries, you just need to pull the required symbols.
#       Use `--start-group' and `--end-group' to link these libraries.
# Do NOT move the libraries linked with `--start-group' and `--end-group' within `--whole-archive' and `--no-whole-archive' options.
# Otherwise, you may get some undesirable errors.
Enclave_Link_Flags := $(SGX_COMMON_CFLAGS) -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_LIBRARY_PATH) -lsgx_tprotected_fs \
	-Wl,--whole-archive -l$(Trts_Library_Name) -Wl,--no-whole-archive \
	-Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tkey_exchange -l$(Crypto_Library_Name) -l$(Service_Library_Name) -Wl,--end-group \
	-Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
	-Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
	-Wl,--defsym,__ImageBase=0 -Wl,--gc-sections   \
	-Wl,--version-script=gt_enclave/gt_enclave.lds 

Enclave_Cpp_Objects := $(Enclave_Cpp_Files:.cpp=.o)

Enclave_Name := gt_enclave.so
Signed_Enclave_Name := gt_enclave.signed.so
Enclave_Config_File := gt_enclave/gt_enclave.config.xml

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


.PHONY: all run

ifeq ($(Build_Mode), HW_RELEASE)
all: .config_$(Build_Mode)_$(SGX_ARCH) serviceprovider $(App_Name) $(Enclave_Name)
	@echo "The project has been built in release hardware mode."
	@echo "Please sign the $(Enclave_Name) first with your signing key before you run the $(App_Name) to launch and access the enclave."
	@echo "To sign the enclave use the command:"
	@echo "   $(SGX_ENCLAVE_SIGNER) sign -key <your key> -enclave $(Enclave_Name) -out <$(Signed_Enclave_Name)> -config $(Enclave_Config_File)"
	@echo "You can also sign the enclave using an external signing tool."
	@echo "To build the project in simulation mode set SGX_MODE=SIM. To build the project in prerelease mode set SGX_PRERELEASE=1 and SGX_MODE=HW."
else
all: .config_$(Build_Mode)_$(SGX_ARCH) serviceprovider $(App_Name) $(Signed_Enclave_Name)
ifeq ($(Build_Mode), HW_DEBUG)
	@echo "The project has been built in debug hardware mode."
else ifeq ($(Build_Mode), SIM_DEBUG)
	@echo "The project has been built in debug simulation mode."
else ifeq ($(Build_Mode), HW_PRERELEASE)
	@echo "The project has been built in pre-release hardware mode."
else ifeq ($(Build_Mode), SIM_PRERELEASE)
	@echo "The project has been built in pre-release simulation mode."
else
	@echo "The project has been built in release simulation mode."
endif
endif

run: all
ifneq ($(Build_Mode), HW_RELEASE)
	@$(CURDIR)/$(App_Name) 	
	@echo "RUN  =>  $(App_Name) [$(SGX_MODE)|$(SGX_ARCH), OK]"
endif

.config_$(Build_Mode)_$(SGX_ARCH):
	@rm -f .config_* $(App_Name) $(App_Name) $(Enclave_Name) $(Signed_Enclave_Name) $(App_Objects) gt_app/gt_enclave_u.* $(Enclave_Cpp_Objects) gt_enclave/gt_enclave_t.* gt_enclave/ai/cJSON.o serviceprovider.* $(ServiceProvider_Cpp_Objects)
	@touch .config_$(Build_Mode)_$(SGX_ARCH)


######## App Objects ########

gt_app/gt_enclave_u.c: $(SGX_EDGER8R) gt_enclave/gt_enclave.edl
	@cd gt_app && $(SGX_EDGER8R) --untrusted ../gt_enclave/gt_enclave.edl --search-path ../gt_enclave --search-path $(SGX_SDK)/include
	@echo "GEN  =>  $@"

gt_app/gt_enclave_u.o: gt_app/gt_enclave_u.c
	@$(CC) $(App_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

gt_app/src/cJSON.o: gt_app/src/cJSON.c
	@$(CC) $(App_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

gt_app/src/%.o: gt_app/src/%.cpp
	@$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

$(App_Name): gt_app/gt_enclave_u.o $(App_Objects)
	@$(CXX) $^ -o $@ $(App_Link_Flags)
	@echo "LINK =>  $@"

######## Service Provider Objects ########


service_provider/%.o: service_provider/%.cpp
	@$(CXX) $(ServiceProvider_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

serviceprovider: $(ServiceProvider_Cpp_Objects)
	@$(CXX) $^ -o $@ $(ServiceProvider_Link_Flags)
	@echo "LINK =>  $@"

libserviceprovider.so: $(ServiceProvider_Cpp_Objects)
	@$(CXX) $^ -o $@ $(ServiceProvider_Link_Flags)
	@echo "LINK =>  $@"

######## Enclave Objects ########

gt_enclave/gt_enclave_t.c: $(SGX_EDGER8R) gt_enclave/gt_enclave.edl
	@cd gt_enclave && $(SGX_EDGER8R) --trusted ../gt_enclave/gt_enclave.edl --search-path ../gt_enclave --search-path $(SGX_SDK)/include
	@echo "GEN  =>  $@"

gt_enclave/gt_enclave_t.o: gt_enclave/gt_enclave_t.c
	@$(CC) $(Enclave_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

gt_enclave/ai/cJSON.o: gt_enclave/ai/cJSON.c
	@$(CC) $(Enclave_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

gt_enclave/%.o: gt_enclave/%.cpp
	@$(CXX) $(Enclave_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

gt_enclave/ai/%.o: gt_enclave/ai/%.cpp
	@$(CXX) $(Enclave_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $<"

$(Enclave_Name): gt_enclave/gt_enclave_t.o $(Enclave_Cpp_Objects) gt_enclave/ai/cJSON.o
	@$(CXX) $^ -o $@ $(Enclave_Link_Flags)
	@echo "LINK =>  $@"

$(Signed_Enclave_Name): $(Enclave_Name)
	@$(SGX_ENCLAVE_SIGNER) sign -key gt_enclave/gt_enclave_private.pem -enclave $(Enclave_Name) -out $@ -config $(Enclave_Config_File)
	@echo "SIGN =>  $@"

.PHONY: clean

clean:
	@rm -f .config_* $(App_Name) $(App_Name) $(Enclave_Name) $(Signed_Enclave_Name) $(App_Objects) gt_app/gt_enclave_u.* $(Enclave_Cpp_Objects) gt_enclave/gt_enclave_t.* gt_enclave/ai/cJSON.o serviceprovider.* $(ServiceProvider_Cpp_Objects)

