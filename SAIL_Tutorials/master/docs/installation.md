#Hardware specs for the SAIL platform
This section will guide throughout the installation

##Processor
Intel Core series and Intel Xeon E3 series after 2015

##Bios
Supports SGX enabling

##Memory, hard drive/SSD
Depends on the needs of the computation and data sizes. It is recommended to have minimum 4GB RAM and 1TB disk. Larger is better.

##Hardware
Desktops or servers are recommended although most of laptops also work. A monitor, a keyboard and a mouse are needed for the early stages of software installations.
Some cloud services support SGX. More information can be found in  <a href="https://github.com/ayeks/SGX-hardware/blob/master/README.md">https://github.com/ayeks/SGX-hardware/blob/master/README.md</a>

##Software
No software required. Bare metal is enough. If an OS other than Linux Ubuntu (16 or later) is installed, we will replace it with a Linux during installation.

##Quantity
It can start from one computer and grow for more computational power.

# Install Docker
##Install from a package

If you cannot use Docker’s repository to install Docker Engine - Community, you can download the .deb file for your release and install it manually. You need to download a new file each time you want to upgrade Docker.

Go to <a href="https://download.docker.com/linux/ubuntu/dists/">https://download.docker.com/linux/ubuntu/dists/</a>, choose your Ubuntu version, browse to pool/stable/, choose amd64, armhf, arm64, ppc64el, or s390x, and download the .deb file for the Docker Engine - Community version you want to install.

Note: To install a nightly package, change the word stable in the URL to nightly. Learn about nightly and test channels.

Install Docker Engine - Community, changing the path below to the path where you downloaded the Docker package.
```
    $ sudo dpkg -i /path/to/package.deb
```
The Docker daemon starts automatically.

Verify that Docker Engine - Community is installed correctly by running the hello-world image.
```
    $ sudo docker run hello-world
```
This command downloads a test image and runs it in a container. When the container runs, it prints an informational message and exits.

Docker Engine - Community is installed and running. The docker group is created but no users are added to it. You need to use sudo to run Docker commands. Continue to Post-installation steps for Linux to allow non-privileged users to run Docker commands and for other optional configuration steps.

##Upgrade Docker Engine - Community
To upgrade Docker Engine - Community, download the newer package file and repeat the installation procedure, pointing to the new file.


# Install a SGX Enclave
The information here is been extracted from the:
Intel_SGX_Installation_Guide_Linux_2.7.1_Open_Source.pdf  
updated on:
2019-12-03 18:19

For the most up to date information please visit:

https://download.01.org/intel-sgx/sgx-linux/2.7.1/docs/

or

https://01.org/intel-software-guard-extensions/downloads

Look for the version under details:
Intel SGX Linux 2.7.1

Under downloads section, look for the Documentation link.

Installation

To install the driver, PSW, and SDK packages, you need the root (or sudo) privilege. Install the components in following order:

1.Intel®SGXdriver

2.Intel®SGXPSW


1.Install the Intel® SGX driver package:
 Using following command to install Intel® SGX driver without ECDSA attestation:

 $ sudo. /sgx_linux_x64_driver.bin

 The installer also loads the driver and sets it to autoload when the system reboots.

 Please refer install Intel® Software Guard Extensions Driver for Data Center Attestation Primitives (Intel® SGX DCAP) to install ECDSA at testation enabled Intel® SGX driver.



# Run a SGX Enclave Application Guides

You need two files in a directory to run an enclave:
```
app: An sgx enclave application software
gt_enclave.signed.so: the dynamic loading library file for an enclave
```
You can copy these files from /home/accuser/platform/sharing/sgx

Run an enclave in below format
```
$ ./app -u dapp_url -a dapp_account -e enclave_url -x enclave_external_url -d enclave_home_directory -f -l <path-to-the-library>
```
The explenations of the command line parameters:
```
-u The URL of the Dapp Server that manages this enclave. (e.g. http://localhost:3200)
-a The account name in the Dapp which is the enclave owner. (e.g. sam)
-e The internal URL of this enclave. (e.g. http://0.0.0.0:4300)
-x The external URL of this enclave in the case of the enclave is running behind a firewall so that the
   enclave can have different internal and external port numbers. (e.g. http://acc.genetank.ai:4300)
-d The path and directory of the enclave data. Without -d option, use ~/.enclave/ under home directory.
   With -d but without a value, use .enclave/ under current directory:
-f This option has no a value parameter. If presents, forces to break the lock which prevent multiple
   enclave instances.    Be careful when use -f option, make sure there is no other enclave instance
   is running
-l (Lower case of 'L') loading libraries into the enclave. The format of this option is -l <full path of the source directory or file>:<destination path start with /lib>. The libraries are only need to be loaded for once.
```
Example command line for running an enclave:
```
$ ./app -a admin -u http://app-dev.secureailabs.com:3300 -e http://0.0.0.0:3160 -x http://40.84.225.145:3160 -d -l /home/accuser/platform/sharing/sgx/gt_enclave/pyvm/Lib:/lib -f
```
