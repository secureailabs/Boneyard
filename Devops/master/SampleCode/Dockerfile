FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y \
	build-essential \
	wget \
	git \
	libcurl4-openssl-dev \
	libprotobuf-dev \
	libprotobuf-c0-dev \
	libboost-all-dev \
	libcpprest-dev \
	libncurses-dev

# Add the repo for SGX PSW 
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' | tee /etc/apt/sources.list.d/intel-sgx.list
RUN wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -
RUN apt-get update
RUN apt-get install -y libsgx-launch libsgx-urts
RUN apt-get install -y libsgx-epid libsgx-urts
RUN apt-get install -y libsgx-quote-ex libsgx-urts

# Install the SGXSDK
RUN wget https://download.01.org/intel-sgx/sgx-linux/2.9.1/distro/ubuntu18.04-server/sgx_linux_x64_sdk_2.9.101.2.bin
RUN chmod +x sgx_linux_x64_sdk_2.9.101.2.bin
RUN echo -e "Yes\n/opt/intel/" | ./sgx_linux_x64_sdk_2.9.101.2.bin

RUN git clone https://github.com/intel/linux-sgx.git
RUN cd linux-sgx/SampleCode/SampleEnclave && make 
ENTRYPOINT cd linux-sgx/SampleCode/SampleEnclave && ./app

