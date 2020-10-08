session.o: session.cpp gt_enclave_t.h include/data_management.h include/dir_management.h include/enclave_view.h include/session.h
gt_enclave.o: gt_enclave.cpp gt_enclave_t.h
interactiveR_t.o:interactiveR_t.cpp gt_enclave_t.h $(PYVM)/Include/pyvm.h
data_management.o:data_management.cpp include/data_management.h
dir_management.o:dir_management.cpp $(PYVM)/Include/enclave_wrapper/io.h ./include/session.h include/dir_management.h
enclave_view.o:enclave_view.cpp include/enclave_view.h include/data_management.h include/dir_management.h
task_manager_enclave.o: task_manager_enclave.cpp include/session.h ../gt_app/include/msg_def.h
cluster.cpp: include/cluster.h
projects.cpp: include/projects.h
util.cpp: include/util.h
