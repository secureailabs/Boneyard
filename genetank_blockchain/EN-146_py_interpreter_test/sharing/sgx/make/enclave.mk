ENCLAVE_BUILD_DIR = $(BUILD_DIR)/$(Enclave_Dir)

RVM := $(Enclave_Dir)/rvm
PYVM := $(Enclave_Dir)/pyvm
TRUSTED_LIB := $(Enclave_Dir)/trusted_libraries

include make/sgx.mk
include $(Enclave_Dir)/files.mk
include make/enclave_objects.mk
include make/rvm.mk
include make/pyvm.mk

# To generate a proper enclave, it is recommended to follow below guideline to link the trusted libraries:
#    1. Link sgx_trts with the `--whole-archive' and `--no-whole-archive' options,
#       so that the whole content of trts is included in the enclave.
#    2. For other libraries, you just need to pull the required symbols.
#       Use `--start-group' and `--end-group' to link these libraries.
# Do NOT move the libraries linked with `--start-group' and `--end-group' within `--whole-archive' and `--no-whole-archive' options.
# Otherwise, you may get some undesirable errors.
Enclave_Link_Flags := $(SGX_COMMON_CFLAGS) -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_LIBRARY_PATH) \
	-lsgx_tprotected_fs -Wl,--whole-archive -l$(Trts_Library_Name) -Wl,--no-whole-archive -Wl,--start-group \
	-lsgx_tstdc -lsgx_tcxx -lsgx_pthread -lsgx_tkey_exchange -l$(Crypto_Library_Name) -l$(Service_Library_Name) -Wl,--end-group \
	-Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
	-Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
	-Wl,--defsym,__ImageBase=0 -Wl,--gc-sections   \
	-Wl,--version-script=gt_enclave/src/gt_enclave.lds \
	#-Wl, $(RVM_OPTIONS) \
	#-Wl, $(RVM_LIBS) 

$(Enclave_Name): PYVM_OBJS_B RVM_OBJS_B $(Enclave_Objects) 
	$(CXX) $(Enclave_Objects) $(PYVM_OBJS) $(RVM_OBJS) -o $@ $(Enclave_Link_Flags)
	@echo "LINK =>  $@"

######## Enclave Objects ########
PYVM_OBJS_B: $(Enclave_Objects)
	(cd $(PYVM) && $(MAKE) all||exit 1)

RVM_OBJS_B: $(Enclave_Objects)
	(cd $(RVM) && $(MAKE) all||exit 1)

$(Enclave_Dir)/gt_enclave_t.c: $(SGX_EDGER8R) $(Enclave_Dir)/src/gt_enclave.edl
	@mkdir -p $(@D)
	@$(SGX_EDGER8R) --trusted $(Enclave_Dir)/src/gt_enclave.edl --search-path gt_enclave/src \
		--search-path $(SGX_SDK)/include --trusted-dir $(Enclave_Dir)
	@echo "GEN  =>  $@"

$(ENCLAVE_BUILD_DIR)/%.o: $(Enclave_Dir)/%.c  $(Enclave_Dir)/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

$(ENCLAVE_BUILD_DIR)/%.o: gt_enclave/%.cpp gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CXX) $(Enclave_Cpp_Flags) -Igt_enclave/rvm/include \
		-Igt_enclave/trusted_libraries/secp256k1/include \
	      	-Igt_enclave/pyvm/Include -Igt_enclave -c $< -o $@
	@echo "CXX  <=  $<"

$(BUILD_DIR)/$(SECP256K1DIR)/%.o: $(SECP256K1DIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -W -Wall -Wextra -Wcast-align -Wnested-externs -Wshadow \
		-Wstrict-prototypes -Wno-unused-function -Wno-long-long -Wno-overlength-strings \
	       	-D HAVE_CONFIG_H -D ENABLE_MODULE_ECDH=1 -I$(SECP256K1DIR)/../ -I$(SECP256K1DIR) \
	       	-I$(SECP256K1DIR)/../include -I$(SGX_SDK)/include/tlibc -c $< -o $@
	@echo "CC   <=  $<"

$(BUILD_DIR)/$(FENVSRCDIR)/%.o: $(FENVSRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -Wno-write-strings -c $< -o $@
	@echo "CC   <=  $<"

$(BUILD_DIR)/$(PNGSRCDIR)/%.o: $(PNGSRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -Igt_enclave/pyvm/Include -Wno-write-strings -c $< -o $@
	@echo "CC   <=  $<"

$(BUILD_DIR)/$(FT2SRCDIR)/%.o: $(FT2SRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -Igt_enclave/trusted_libraries -I$(FT2SRCDIR)/include \
		-Igt_enclave/pyvm/Include -Wno-write-strings -c $< -o $@
	@echo "CC   <=  $<"

$(BUILD_DIR)/$(LAPACKSRCDIR)/%.o: $(LAPACKSRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CXX) $(Enclave_C_Flags) -I$(LAPACKSRCDIR) -Wno-write-strings -c $< -o $@
	@echo "CXX   <=  $<"
	
$(BUILD_DIR)/$(PRNGSRCDIR)/%.o: $(PRNGSRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -I$(PRNGSRCDIR) -Wno-write-strings -c $< -o $@
	@echo "CC   <=  $<"

$(BUILD_DIR)/$(ZLIBSRCDIR)/%.o: $(ZLIBSRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -Igt_enclave/pyvm/Include -Wno-write-strings -c $< -o $@
	@echo "CC   <=  $<"
	
$(BUILD_DIR)/$(LIBFFISRCDIR)/%.o: $(LIBFFISRCDIR)/%.c gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -I$(LIBFFISRCDIR) -Wno-write-strings -c $< -o $@
	@echo "CC   <=  $<"

$(BUILD_DIR)/$(LIBFFISRCDIR)/%.o: $(LIBFFISRCDIR)/%.S gt_enclave/gt_enclave_t.c
	@mkdir -p $(@D)
	$(CC) $(Enclave_C_Flags) -I$(LIBFFISRCDIR) -c $< -o $@
	@echo "CC   <=  $<"

include $(Enclave_Dir)/filedep.mk
include $(FENVSRCDIR)/filedep.mk
include $(PNGSRCDIR)/filedep.mk
include $(PRNGSRCDIR)/filedep.mk
include $(ZLIBSRCDIR)/filedep.mk
include $(FT2SRCDIR)/filedep.mk
include $(LIBFFISRCDIR)/filedep.mk
include $(LAPACKSRCDIR)/filedep.mk

