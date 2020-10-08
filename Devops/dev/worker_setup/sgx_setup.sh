#!/bin/sh

# Stop execution in case something fails 
set -e

# Print commands as they execute
set -x

# Dependencies for sgx drivers and SDK
sudo apt-get install -y libssl-dev libcurl4-openssl-dev libprotobuf-dev
sudo apt-get install -y build-essential python
sudo apt-get install -y dkms
sudo apt-get install linux-headers-$(uname -r)

# Download and install driver
wget https://download.01.org/intel-sgx/sgx-linux/2.9.1/distro/ubuntu18.04-server/sgx_linux_x64_driver_2.6.0_95eaa6f.bin
chmod +x sgx_linux_x64_driver_2.6.0_95eaa6f.bin
sudo ./sgx_linux_x64_driver_2.6.0_95eaa6f.bin

# Add the repo for SGX PSW 
echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add -
sudo apt-get update
sudo apt-get install -y libsgx-launch libsgx-urts
sudo apt-get install -y libsgx-epid libsgx-urts
sudo apt-get install -y libsgx-quote-ex libsgx-urts

# Cleanup 
rm sgx_linux_x64_driver_2.6.0_95eaa6f.bin
