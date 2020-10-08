# Docker Guides for Enclave

## 1. Install docker
See details in "../Readme.MD"

## 2. Install SGX driver
See details in "../Readme.MD"

## 3. Create docker for the enclave

### 3.1 Build Docker Image
Build a docker image named 'sail_enclave' according the Dockerfile
```
$ docker build -f Dockerfile -t sail_enclave ../../
```

### 3.2 Run a docker container
Create the container named 'enclave' from image 'sail_enclave' and run the container 
```
$ EXT_PORT=3180 DAPP_URL=dapp_host HOST_DOMAIN=enclave.secureailabs.com docker run --name enclave -dit -p $EXT_PORT:3080 \
  --device /dev/isgx \
  sail_enclave \
  /usr/src/app/app -a admin \
  -u http://$DAPP_URL:4000 \
  -e http://0.0.0.0:3080 -x http://$HOST_DOMAIN:$EXT_PORT \
  -d /usr/src/app/enclave \
  -l /usr/src/app/Lib:/lib
```

### 3.3 Attach the container
```
$ docker exec -it enclave /bin/bash
```

### 3.4 Save docker image to a file
Save the docker image enclave_image to a file enclave_image.tar.gz
```
$ docker save sail_enclave |gzip>enclave_image.tar.gz
```

### 3.5 Load docker image from a file
Load docker image from file enclave_image.tar.gz
```
$ docker load <enclave_image.tar.gz
```

### 3.6 Create an enclave container
Use the command below. Replace the infor inside <> before run it. The option in [] is optional.
```
$ docker run --name <enclave_container_name> -dit -p <ext_port>:<internal_port> \
  --device /dev/isgx [--device /dev/mei0] \
  sail_enclave \
  /usr/src/app/app -a admin \
  -u http://<dApp_server_IP_or_URL>:<dApp_port> \
  -e http://0.0.0.0:<internal_port> -x http://enclave.secureailabs.com:<internal_port> \
  -d /usr/src/app/enclave \
  -l /usr/src/app/Lib:/lib
```
If you want to update files in the container, you can use "docker cp" command to copy files into the container.
