FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y \
	build-essential \
	wget \
	curl \
	unzip \
	openssh-server \
	openjdk-8-jdk 

RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
RUN unzip awscliv2.zip
RUN ./aws/install

RUN apt-get install -qy openssh-server && \
    sed -i 's|session required pam_loginuid.so|session optional pam_loginuid.so|g' /etc/pam.d/sshd && \
    mkdir -p /var/run/sshd

RUN mkdir -p /jdk/bin
RUN ln -s /usr/bin/java /jdk/bin/java

RUN adduser --quiet jenkins
RUN echo jenkins:jenkins | chpasswd

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
