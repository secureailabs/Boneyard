MAKE := make

CC := gcc
CXX := g++

BUILD_DIR := build

App_Name := $(BUILD_DIR)/app
App_Dir_abs := $(SGX_ROOT)/gt_app
App_Dir := $(shell realpath --relative-to=$(SGX_ROOT) $(SGX_ROOT)/gt_app)
Enclave_Dir_abs := $(SGX_ROOT)/gt_enclave
Enclave_Dir := $(shell realpath --relative-to=$(SGX_ROOT) $(SGX_ROOT)/gt_enclave)
Enclave_Name := $(BUILD_DIR)/gt_enclave.so
Signed_Enclave_Name := $(BUILD_DIR)/gt_enclave.signed.so
Enclave_Config_File := $(Enclave_Dir)/src/gt_enclave.config.xml

SGX_SDK := /home/jjj/sgxsdk
SGX_MODE := HW
#SGX_MODE := SIM
SGX_ARCH := x64
SGX_DEBUG := 1

