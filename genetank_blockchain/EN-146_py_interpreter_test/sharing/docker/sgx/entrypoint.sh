#!/bin/bash

source /opt/intel/sgxsdk/environment
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/intel/sgxpsw/aesm/

jhid -d
/opt/intel/sgxpsw/aesm/aesm_service &
pid=$!

trap "kill ${pid}" TERM INT

sleep 2

exec "$@" &>>sgx.log

#./app -a admin -u http://app-dev.secureailabs.com:4000 \
#  -e http://0.0.0.0:3080 -x http://localhost:3080 \
#  -d /usr/src/app/enclave \
#  -l /usr/src/app/Lib:/lib 2>&1 >enclave.log
