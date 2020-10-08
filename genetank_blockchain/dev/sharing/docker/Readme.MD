# Guides for deploying a dApp and a enclave docker container
Updated on April 4, 2020

## 1. Prerequisitions
Docker software (community version is enough) must be installed on the system (Linux, Windows or MacOs). <br/>
The below are example of installing docker on a Linux Ubuntu system.

### 1.1 Preparations
```
$ sudo apt-get update

$ sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common

$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

$ sudo apt-key fingerprint 0EBFCD88

$ sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
```
### 1.2 Install docker community version
```
$ sudo apt-get update
$ sudo apt-get install docker-ce docker-ce-cli containerd.io
```
Run "docker ps" command to exam the installation. If it shows error, run below command to add current user to the docker group.
```
$ sudo usermod -aG docker $USER
```
And re-login to take effect.

## 2. Deploy a dApp Server

### 2.1 Preparation
* Docker installed (install as above)
* A docker image file `dapp_image.tar.gz`
* A range of 100 network ports available on the system

### 2.2 Load docker image from a file
Load docker image from file dapp_image.tar.gz
```
$ docker load -i <dapp_image.tar.gz
```
Use the command `docker image ls` to check the image created:
```
$ docker image ls
REPOSITORY                  TAG                 IMAGE ID            CREATED             SIZE
dapp_image                  latest              ef50b1eae68c        1 minutes ago       5.48GB
```

### 2.3 Run a dapp container
```
$ docker run --name <name_of_dapp_container> -dit \
   -p <external_port_range_start>-<external_port_range_end>:4000-4099 sail_dapp
```
For example
```
$ docker run --name dapp -dit -p 4000-4099:4000-4099 dapp_image
```
In which, the both external and internal are using port number range from 4000 to 4099. The newly created container has a name "dapp".
Use command "docker ps" to show the container:
```
$ docker ps
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                                             NAMES
df3c2f11f111        dapp_image          "/bin/sh -c 'meteor …"   3 minutes ago       Up 45 hours         0.0.0.0:4000-4099->4000-4099/tcp, 4100-4999/tcp   dapp
```
To examine the container you can attach it with below command:
```
$ docker exec -it dapp /bin/bash
```
Open URL http://localhost:4000 with a browser to do test.

You can upgrade the files for the dApp with "docker cp" command.
For example, to copy a file "settings.json" into the container:
```
$ docker cp settings.json dapp:/home/dapp/dapp/.
```

## 3. Deploy a SGX enclave on a SGX enabled Server

You need below things before start:
* SGX driver installed as described below
* A SGX docker image file "enclave_image.tar.gz"
* The URL and port number that your dApp server
* The URL/IP address of your system
* A port number available on your system

### 3.1 Install SGX driver

#### 3.1.1 Prerequisition
```
$ sudo apt-get install libssl-dev libcurl4-openssl-dev libprotobuf-dev
$ sudo apt-get install build-essential python
```
#### 3.1.2 Install the driver
Download the driver bin file from the Intel server save to the file sgx_driver.bin.
Run:
```
$ chmod +x sgx_driver.bin
$ sudo ./sgx_driver.bin
```
Install SGX PSW:
1. Download PSW package:
```
$ echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu bionic main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
$ wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add -
```
2. Update the apt and install the packages:
```
$ sudo apt-get update
```
Install launch service:
```
$ sudo apt-get install libsgx-launch libsgx-urts
```
Install EPID-based attestation service:
```
$ sudo apt-get install libsgx-epid libsgx-urts
```
Install algorithm agnostic attestation service:
```
$ sudo apt-get install libsgx-quote-ex libsgx-urts
```
#### 3.1.3 Check device
/dev/isgx must be there, otherwise restart the computer to enabe the bios
Another optional device is /dev/mei0 which can also be there.

### 3.2 Load docker image from a file
Load docker image from file enclave_image.tar.gz
```
$ docker load <enclave_image.tar.gz
```
Use command "docker image ls" to check a docker image "sail_enclave" which has been created:
```
$ docker image ls
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
sail_enclave        latest              11fa87e3b21f        1 minutes ago       1.33GB
```

### 3.3 Create an enclave container
Use the command below. Replace the infor inside <> before run it. The option in [] is optional.
```
$ docker run --name <enclave_container_name> -dit -p <external_port>:3080 \
  --device /dev/isgx [--device /dev/mei0] \
  sail_enclave \
  /usr/src/app/app -a admin \
  -u http://<dApp_server_public_IP_or_URL>:<dApp_port> \
  -e http://0.0.0.0:3080 -x http://<dApp_server_public_IP_or_URL>:<external_port> \
  -d /usr/src/app/enclave \
  -l /usr/src/app/Lib:/lib
```

As an example, an enclave named "enclave" can be created with below command:
```
$ docker run --name enclave -dit -p 3180:3080 \
  --device /dev/isgx --device /dev/mei0 \
  sail_enclave \
  /usr/src/app/app -a admin \
  -u http://app-dev.secureailabs.com:4000 \
  -e http://0.0.0.0:3080 -x http://enclave.secureailabs.com:3180 \
  -d /usr/src/app/enclave \
  -l /usr/src/app/Lib:/lib
```

To examine the container you can attach it with below command:
```
$ docker exec -it enclave /bin/bash
```

### 3.4 MISC: Update files in an enclave container
If you want to update files in the container, you can use "docker cp" command to copy files into the container.
For example, if you need to upgrade the SAIL SGX enclave application, you need to run below commands (assume you have both files "app" and "gt_enclave.signed.so" on current directory):
```
$ docker cp app enclave:/usr/src/app/.
$ docker cp gt_enclave.signed.so enclave:/usr/src/app/.
```

## 4: Connecting the Dapp with the Hub Enclave
If everything is set up correctly, the connection between the enclave and the dapp should be established. 

### 4.1: Log In
To add the connected enclave to the dapp, though, you need to log in as admin. To do this, open a browser and go to the IP address (including the port you set your dapp to--if you used the tutorial example, it is 4000). Log in.

### 4.2: Navigate to the Enclaves
On the left hand panel, select Enclaves

### 4.3: Click on the Edit button in the new Enclave
You should see your newly created Enclave. The public key should match the key that was generated when the enclave started (in your terminal). To the right of it are three buttons: add, edit, and delete. Click on edit. Configure the enclave by assigning it to a user. The assigned user will be able to access the enclave in the future. For example, assign it to Shifa.

### 4.4: Restart the enclave container
```
$ docker stop enclave
$ docker start enclave
```
The enclave must be restarted for the configurations to take effect.

### 4.5: Test the Enclave
In the dapp, log out of admin and log back in to the user you assigned the enclave to (e.g., Shifa). After logging in, select Analyze Data on the left Panel. From there, click New Project. Type in a name for the project and select the enclave that you just created. Now you can enter some python code. For example, enter the code below. Then click run. The output should be Hello World.
```
$ print "hello world"
```
When the enclave restarts, it takes some time for it to set up. Therefore, you should wait up to one minute before testing the enclave.
