#!/bin/sh

# Stop execution in case something fails 
set -e

# Print commands as they execute
set -x

docker build . -t sail_jenkins:latest

docker container run \
  --name sail_jenkins \
  --rm \
  --detach \
  --network jenkins \
  --network-alias jenkins \
  --env DOCKER_HOST=tcp://docker:2376 \
  --env DOCKER_CERT_PATH=/certs/client \
  --env DOCKER_TLS_VERIFY=1 \
  --env HTTP_HOST=127.0.0.1 \
  --publish 127.0.0.1:8080:8080 \
  --publish 127.0.0.1:50000:50000 \
  --volume jenkins-data:/var/jenkins_home \
  --volume jenkins-docker-certs:/certs/client:ro \
  sail_jenkins:latest

