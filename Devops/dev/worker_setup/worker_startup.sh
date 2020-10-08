#!/bin/sh

# Stop execution in case something fails 
set -e

# Print commands as they execute
set -x

# install AWS 
apt-get install unzip
curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
unzip awscliv2.zip
sudo ./aws/install

# install SGX drivers
./sgx_setup.sh

#install docker 
./docker_setup.sh

#create docker volume and networks
docker network create jenkins
docker volume create jenkins-docker-certs
docker volume create jenkins-data

#restore jenkins from AWS
cd /var/lib/docker/volumes/jenkins-data/_data 
aws s3 cp aws s3 ls s3://sail-jenkins-backups/jenkins_backup.tar.gz 
tar xvf jenkins_backup.tar.gz
rm jenkins_backup.tar.gz

#setting up AWS credentials
if [ -z "$AWS_ACCESS_KEY_ID" ]; then
    read -p 'Enter your AWS_ACCESS_KEY_ID: ' AWS_ACCESS_KEY_ID
fi

if [ -z "$AWS_SECRET_ACCESS_KEY" ]; then
    read -p 'Enter the AWS_SECRET_ACCESS_KEY: ' AWS_SECRET_ACCESS_KEY
fi

# Testing the AWS credentials 
aws s3 ls > /dev/null
status=$?
if test $status -ne 0
then
    echo "Invalid AWS credentials. Kindly try again.\n"
    exit 1;
fi

# Start jenkins server
pushd ../jenkins/jenkins_master
./jenkins_startup.sh
popd

# Start nginx server 
pushd ../jenkins/nginx
./nginx_start.sh
popd

# Start dind container
pushd ../jenkins/dind_container
./nginx_start.sh
popd

