# Docker Guides for dApp and Enclave

## 1. Guides for dockerizing the dApp server

### 1.1 Build Docker Image
Build a docker image named 'sail_dapp' according the Dockerfile
```
$ docker build -f Dockerfile -t sail_dapp ../../
```

### 1.2 Run a docker container
Create the container named 'dapp' from image 'sail_dapp' and run the container 
```
$ docker run --name dapp -dit -p 4000-4099:4000-4099 sail_dapp
```

### 1.3 Attach the container
Attach as default user dapp:
```
$ docker exec -it dapp /bin/bash
```
Attach as root:
```
$ docker exec -u 0 -it dapp /bin/bash
```

### 1.4 Create a docker image from a container
Create docker image named dapp_image from the container dapp:
```
$ docker commit dapp dapp_image
```

### 1.5 Save docker image to a file
Save the docker image dapp_image to a file dapp_image.tar.gz
```
$ docker save dapp_image |gzip>dapp_image.tar.gz
```

### 1.6 Load docker image from a file
Load docker image from file dapp_image.tar.gz
```
$ docker load <dapp_image.tar.gz
```

### 1.7 Run a dapp container
```
$ docker run --name <name_of_dapp_container> -dit \
   -p <external_port_range_start>-<external_port_range_end>:<internal_port_range_start>-<internal_port_range_end> sail_dapp
```
Example
```
$ docker run --name dapp -dit -p 4000-4099:4000-4099 sail_dapp
```
