#!/bin/sh

cd slaves && cd sgx_container && ./agent_startup.sh
cd ../..
cd slaves && cd jenkins_backup_s3 && ./agent_startup.sh
cd ../..


