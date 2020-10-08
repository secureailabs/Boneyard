To run sgx applications, run the following command: 

docker run --device /dev/isgx --volume /var/run/aesmd:/var/run/aesmd <container_id>
