######## Enclave Settings ########
Enclave_Cpp_Files := \
  src/gt_enclave.cpp \
  src/data_management.cpp \
  src/dir_management.cpp \
  src/data_inspector.cpp \
  src/mtcsv2R.cpp \
  src/conversion.cpp \
  src/session.cpp \
  src/cluster.cpp \
  src/projects.cpp \
  src/enclave_view.cpp \
  src/crypto.cpp \
  src/data_share.cpp \
  src/stream.cpp \
  src/util.cpp \
  src/dir_cache.cpp \
  src/task_manager_enclave.cpp

Enclave_C_Files := \
  src/cjson.c \
  src/keccakSponge.c \
  src/keccakF-1600-opt32.c \
  src/keccakNISTInterface.c

