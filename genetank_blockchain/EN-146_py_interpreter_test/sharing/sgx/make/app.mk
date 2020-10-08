APP_BUILD_DIR = $(BUILD_DIR)/$(App_Dir)

App_Include_Paths := -I$(App_Dir)/include -I$(SGX_SDK)/include -I/usr/local/include -I/usr/include
App_C_Flags := $(SGX_COMMON_CFLAGS) -fPIC -Wno-attributes $(App_Include_Paths)

ifeq ($(SGX_DEBUG), 1)
        App_C_Flags += -DDEBUG -UNDEBUG -UEDEBUG
else ifeq ($(SGX_PRERELEASE), 1)
        App_C_Flags += -DNDEBUG -DEDEBUG -UDEBUG
else
        App_C_Flags += -DNDEBUG -UEDEBUG -UDEBUG
endif

REST_SSL_Link_Flags = -L /usr/local/lib  -lcrypto -lssl -lcpprest -lboost_system -lboost_chrono
App_Cpp_Flags := $(App_C_Flags) -std=c++11
App_Link_Flags := $(SGX_COMMON_CFLAGS) -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -lncurses -ltinfo -lsgx_uprotected_fs -L. -lsgx_ukey_exchange -lpthread $(REST_SSL_Link_Flags) -Wl,-rpath=$(CURDIR)

ifneq ($(SGX_MODE), HW)
        App_Link_Flags += -lsgx_epid_sim
else
        App_Link_Flags += -lsgx_epid
endif

App_C_Objects := $(App_C_Files:.c=.o)
App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)
App_Objects_t := $(App_C_Objects) $(App_Cpp_Objects)
App_Objects = $(patsubst %, $(BUILD_DIR)/%, $(App_Objects_t))

$(App_Name): $(APP_BUILD_DIR)/gt_enclave_u.o $(App_Objects)
	@$(CXX) $^ -o $@ $(App_Link_Flags)
	@echo "LINK =>  $@"

gt_app/gt_enclave_u.c: $(SGX_EDGER8R) $(Enclave_Dir)/src/gt_enclave.edl
	@$(SGX_EDGER8R) --untrusted $(Enclave_Dir)/src/gt_enclave.edl --search-path $(Enclave_Dir) --search-path $(SGX_SDK)/include --untrusted-dir gt_app
	@echo "GEN  =>  $@"

$(APP_BUILD_DIR)/gt_enclave_u.o: $(App_Dir)/gt_enclave_u.c
	@mkdir -p $(@D)
	@$(CC) $(App_C_Flags) -c $< -o $@
	@echo "CC   <=  $<"

build/$(App_Dir)/src/%.o : $(App_Dir)/src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(App_Cpp_Flags) -c $< -o $@
	@echo "CXX  <=  $< $@"


