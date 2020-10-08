FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y \
	build-essential \
	wget \
	git \
	openssh-server \
	openjdk-8-jdk \
	libprotobuf-dev \
	libprotobuf-c0-dev \
	libboost-all-dev \
	libcpprest-dev \
	libncurses-dev

RUN apt-get install -qy openssh-server && \
    sed -i 's|session required pam_loginuid.so|session optional pam_loginuid.so|g' /etc/pam.d/sshd && \
    mkdir -p /var/run/sshd

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
RUN echo -e "Yes\n/home/accuser/" | ./sgx_linux_x64_sdk_2.9.101.2.bin

RUN mkdir -p /jdk/bin
RUN ln -s /usr/bin/java /jdk/bin/java

RUN adduser --quiet jenkins
RUN echo jenkins:jenkins | chpasswd

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
