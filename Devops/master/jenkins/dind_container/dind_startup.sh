#!/bin/sh

# Stop execution in case something fails 
set -e

# Print commands as they execute
set -x

if [ ! -f "dind_certs.tar.gz" ]; then
    aws s3 cp s3://sail-jenkins-backups/dind_certs.tar.gz ./
    mkdir certs
    tar -C certs -xvf dind_certs.tar.gz
fi

docker build . -t sail_dind:latest

docker container run \
  --name sail_jenkins_dind \
  --rm \
  --detach \
  --privileged \
  --network jenkins \
  --network-alias docker \
  --env DOCKER_TLS_CERTDIR=/certs \
  --volume jenkins-docker-certs:/certs/client \
  --volume jenkins-data:/var/jenkins_home \
  --publish 127.0.0.1:2376:2376 \
  sail_dind:latest

sleep 10

docker exec -it sail_jenkins_dind "./build_container.sh"
rm -rf dind_certs.tar.gz certs
