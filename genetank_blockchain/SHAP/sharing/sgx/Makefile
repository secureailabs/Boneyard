SGX_ROOT_b:=$(shell pwd)
export SGX_ROOT=$(SGX_ROOT_b)

include $(SGX_ROOT)/make/defaults.mk
include $(SGX_ROOT)/make/sgx.mk
include $(SGX_ROOT)/gt_app/files.mk

.PHONY: all run $(App_Name) $(Enclave_Name)

ifeq ($(Build_Mode), HW_RELEASE)
all: .config_$(Build_Mode)_$(SGX_ARCH) $(App_Name) $(Enclave_Name)
	@echo "The project has been built in release hardware mode."
	@echo "Please sign the $(Enclave_Name) first with your signing key before you \
	       	run the $(App_Name) to launch and access the enclave."
	@echo "To sign the enclave use the command:"
	@echo "   $(SGX_ENCLAVE_SIGNER) sign -key <your key> -enclave $(Enclave_Name) \
		-out <$(Signed_Enclave_Name)> -config $(Enclave_Config_File)"
	@echo "You can also sign the enclave using an external signing tool."
	@echo "To build the project in simulation mode set SGX_MODE=SIM. To build the \
		project in prerelease mode set SGX_PRERELEASE=1 and SGX_MODE=HW."
else
all: .config_$(Build_Mode)_$(SGX_ARCH) $(App_Name) $(Signed_Enclave_Name)
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

include $(SGX_ROOT)/make/app.mk

$(Signed_Enclave_Name): $(Enclave_Name)
	@$(SGX_ENCLAVE_SIGNER) sign -key gt_enclave/src/gt_enclave_private.pem \
		-enclave $(Enclave_Name) -out $@ -config $(Enclave_Config_File)
	@echo "SIGN =>  $@"
	
run: all
ifneq ($(Build_Mode), HW_RELEASE)
	@$(CURDIR)/$(App_Name) 	
	@echo "RUN  =>  $(App_Name) [$(SGX_MODE)|$(SGX_ARCH), OK]"
endif

.config_$(Build_Mode)_$(SGX_ARCH):
	@rm -f .config_* $(App_Name) $(App_Name) $(Enclave_Name) $(Signed_Enclave_Name) \
		$(App_Objects) $(App_Dir)/gt_enclave_u.* $(Enclave_Cpp_Objects) $(ENClave_Dir)/gt_enclave_t.*
	@touch .config_$(Build_Mode)_$(SGX_ARCH)

include $(SGX_ROOT)/make/enclave.mk

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(App_Dir)/gt_enclave_u.* $(Enclave_Dir)/gt_enclave_t.*
