#!/bin/sh

# Stop execution in case something fails 
set -e

# Print commands as they execute
set -x

echo "\n\nTesting the system setup for Docker sgx applications\n\n"
docker build . -t env_test:latest
docker run --device /dev/isgx --volume /var/run/aesmd:/var/run/aesmd env_test:latest
