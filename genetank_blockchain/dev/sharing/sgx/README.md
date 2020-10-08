# Secure AI Labs SGX Enclave Software

This is the directory for all the codes and data for SGX enclave.

# A guide for Linux remote desktop setup

Two commands below are must while rest commends are optional:
```
$ sudo apt-get install xfce4
```
```
$ sudo apt-get -y install xrdp
```

https://askubuntu.com/questions/592537/can-i-access-ubuntu-from-windows-remotely
or
https://www.tweaking4all.com/software/linux-software/use-xrdp-remote-access-ubuntu-14-04/

$ echo "xfce4-session" > ~/.xsession

The below are steps from https://help.ubuntu.com/community/xrdp

$ sudo nano /etc/xrdp/xrdp.ini

$ encrypt_level=high

$ sudo ufw allow 3389/tcp

$ sudo nano /etc/polkit-1/localauthority.conf.d/02-allow-colord.conf

input:

polkit.addRule(function(action, subject) {
if ((action.id == “org.freedesktop.color-manager.create-device” || action.id == “org.freedesktop.color-manager.create-profile” || action.id == “org.freedesktop.color-manager.delete-device” || action.id == “org.freedesktop.color-manager.delete-profile” || action.id == “org.freedesktop.color-manager.modify-device” || action.id == “org.freedesktop.color-manager.modify-profile”) && subject.isInGroup(“{group}”))
{
return polkit.Result.YES;
}
});

$ sudo /etc/init.d/xrdp restart

# Install Browsers

## Install Chrome
```
$ wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
```
```
$ sudo dpkg -i google-chrome-stable_current_amd64.deb
```
If there are some package dependencies error, run below command and redo above:
```
$ sudo apt-get -f install
```

## install Firefox
```
$ sudo apt-get update
$ sudo apt-get --purge --reinstall install firefox
```
If there are some package dependencies error, run below command and redo above:
```
$ sudo apt-get -f install
```

# Install SGX SDK

## Download SGX software from

https://01.org/intel-software-guard-extensions/downloads

Download the latest version of driver, psw and sdk thress files.

## Install prerequisites
```
$ sudo apt-get install libssl-dev libcurl4-openssl-dev libprotobuf-dev
$ sudo apt-get install build-essential python
```
*(no iclsClient required for later version of SGX driver)*
Download iclsClient (x86_64) from below link. (Need signed in with Intel company account, or sign up one by yourself)

https://01.org/intel-software-guard-extensions/downloads

Run below commands:
```
$ sudo apt-get install alien
$ sudo alien --scripts iclsClient-1.45.449.12-1.x86_64.rpm
$ sudo dpkg -i iclsclient_1.45.449.12-2_amd64.deb
```
Download and install dynamic application loader host interface
```
$ sudo apt-get install uuid-dev libxml2-dev cmake pkg-config libsystemd-dev
$ git clone https://github.com/intel/dynamic-application-loader-host-interface.git JHI
$ cd JHI
$ cmake .;make;sudo make install;sudo ldconfig;sudo systemctl enable jhi
$ cd ..
```
## Installation
For Azure DC-series VM, there could be pre-installed SGX DCAP driver. You may have to unstall the DCAP driver as described in https://github.com/intel/SGXDataCenterAttestationPrimitives/tree/master/driver/linux
Basically, using below steps to uninstall sgx DCAP driver:
```
$ sudo /sbin/modprobe -r intel_sgx
$ sudo dkms remove -m sgx -v <version> --all
$ sudo rm -rf /usr/src/sgx-<version>
```
To remove intel_sgx from /etc/modules:
```
$ sudo /bin/sed -i '/^intel_sgx$/d' /etc/modules
```

* Install driver
```
$ chmod +x *.bin
$ sudo ./sgx_linux_x64_driver_*.bin
```
* Install PSW
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
* Install SDK
```
$ sudo ./sgx_linux_x64_sdk_2.8.100.3.bin
Do you want to install in current directory? [yes/no] : no
Please input the directory which you want to install in : /opt/intel
```
Some system may need to configure the shared library:
```
$ sudo ln -s /opt/intel/sgxsdk/lib64/libsgx_uae_service.so /usr/lib/libsgx_uae_service.so
```

## Set environment variables
```
$ source /opt/intel/sgxsdk/environment
```
optional download and install eclipse sgx plugins from https://01.org/intel-software-guard-extensions/downloads

Or follow the guidance in <github root>/doc/SGX/Linux/Intel_SGX_Installation_Guide_Linux_2.1.1_Open_Source.pdf
  
  
# Download and build cpprestsdk

Install the sdk to the system
```
sudo apt-get install libcpprest-dev
```
The source code and more information:

https://github.com/Microsoft/cpprestsdk/wiki/How-to-build-for-Linux


# A guide for Eclipse C/C++ development environment setup

Reference: http://linuxpitstop.com/install-eclipse-ide-on-ubuntu-linux-15-04/

## 1. Install Java
```
$ sudo apt-get install openjdk-8-jdk
```

## 2. download eclipse

http://www.eclipse.org/downloads/?osType=linux


## 3. setup eclipse
```
$ sudo mv eclipse-inst-linux64.tar.gz /opt/.
$ cd /opt
$ sudo tar xvf eclipse-inst-linux64.tar.gz
$ cd eclipse-installer
```
Run below command and select c++ development tools to install. Enter installation foulder: /opt/eclipse/cpp-photon
```
$ sudo ./eclipse-inst
```
Setup environment:
```
$ sudo nano /usr/share/applications/eclipse.desktop
```
Enter text below:
```
[Desktop Entry]
Name=Eclipse
Type=Application
Exec=/opt/eclipse/cpp-photon/eclipse/eclipse
Terminal=false
Icon=/opt/eclipse/icon.xpm
Comment=Integrated Development Environment
NoDisplay=false
Categories=Development;IDE;
Name[en]=eclipse.desktop
```

Continue installation with below commands:
```
$ sudo desktop-file-install /usr/share/applications/eclipse.desktop
$ cd /usr/local/bin
$ sudo ln -s /opt/eclipse/cpp-photon/eclipse/eclipse
```

In somecase, eclipse can't be lauched because of companion library. The below command may fix it.
```
$ sudo chmod 755 /root
```

## Import SGX project to the Eclipse

Use menu File --> Import

Select C/C++ --> Existing Code as Makefile Project --> "Next" button

Select "Exiting code location" "Browse" button --> select "./sharing/sgx" --> "OK" button --> "Finish" button

## Eclipse path setting up

Select project, goto menu "project"->"properties"->"C/C++ General"->"Paths and Symbols" and add path to include directories.

## SGX enclave debugger setting up

Select project, goto menu "Run"->"Debug Configuration"->"C/C++ Application"->"sgx(project name) Default"->"Debugger"->
"DBG debugger" "Browse..." Select sgxsdk/bin/sgx-gdb -->"Apply"

If gdb hasn't been installed, run below command to install.
```
sudo apt-get install gdb
```


# A guide for attestation and signing flow

## The flow of an attestation

Generate Enclave (state) report 

--> create a credential by signing the report 

--> generate a quote which includes the report and the signature

--> send the quote to the verifier


A quote includes:

  . MRENCLAVE: Measurement of the code and data in the enclave

  . MRSIGNER: A hash of the public key of enclave signer

  . ISVPRODID, ISVSVN: The Product ID and the Security Version Number (SVN) of the enclave

  . ATTRIBUTES: Attributes of the enclave

  . User data
  
  . A signature block over the above data, which is signed by the CPU private key (EPID key)

The Quoting Enclave: An enclave provided by Intel known as the quating Enclave (QE) verifies the reports that have been created to its MRENCLAVE measurement value and then converts and signs them using EPID key. The output of this process is called a quote.


## Two types enclave signing 

### Single step signing:

$ "C:\Program Files (x86)\Intel\IntelSGXSDK\bin\win32\release\sgx_sign.exe" sign -key "isv_enclave_private.pem" -enclave "C:\zsf\RemoteAttestation\x64\Debug\isv_enclave.dll" -out "C:\zsf\RemoteAttestation\x64\Debug\isv_enclave.signed.dll" -config "isv_enclave.config.xml"


### Two steps signing (actually three steps):

Step 1: generate the data for signing from the enclave image file and configurations.

$ <SGXSDK path>\bin\win32\release\sgx_sign gendata -enclave isv_enclave.dll -config isv_enclave.config.xml -out isv_enclave_hash.bin

Step #: signing with openssl or hardware private key module. See below for openssl signing.

Step 2: generate the signed enclave image file.

$ <SGXSDK path>\bin\win32\release\sgx_sign catsig -enclave isv_enclave.dll -config isv_enclave.config.xml -out isv_enclave_signed.dll -key isv_enclave_public.pem -sig signature.bin -unsigned isv_enclave_hash.bin

Example of 2 steps signing:

"$(SGXSDKInstallPath)bin\win32\release\sgx_sign.exe" gendata -enclave "$(OutDir)isv_enclave.dll" -config "isv_enclave.config.xml" -out "$(OutDir)isv_enclave_hash.bin"

"\OpenSSL-Win64\bin\openssl.exe" dgst -sha256 -out "$(OutDir)signature.bin" -sign "isv_enclave_private.pem" -keyform PEM "$(OutDir)isv_enclave_hash.bin"

"$(SGXSDKInstallPath)bin\win32\release\sgx_sign.exe" catsig -enclave "$(OutDir)isv_enclave.dll" -config "isv_enclave.config.xml" -out "$(OutDir)isv_enclave_signed.dll" -key "isv_enclave_public.pem" -sig "$(OutDir)signature.bin" -unsigned "$(OutDir)isv_enclave_hash.bin"

### OpenSSL Examples

The following command lines are typical examples using OpenSSL*.

Generate a 3072-bit RSA private key. Use 3 as the public exponent value.

$ openssl genrsa -out private_key.pem -3 3072

Produce the public part of a private RSA key.

$ openssl rsa -in private_key.pem -pubout -out public_key.pem
 
Sign the file containing the enclave signing material.

$ openssl dgst -sha256 -out signature.bin -sign private_key.pem -keyform PEM enclave_hash.bin

or

$ openssl dgst -sha256 -out signature.bin -sign \<private-key\> enclave_hash.bin

Convert bin to base64 encoding.

$ openssl base64 -in signature.bin -out signature_base64.hex

** The base64 encoded signature will be stored on the blockchain. The information in the isv_enclave.config.xml and MRSIGNER will be presented on the html/javascripts of the Dapp. The verifier can get these information from a running enclave then verify with the information from the blockchain and the Dapp.

Convert base64 to bin coding.

$ openssl base64 -d -in signature_base64.hex -out signature.bin

Verify the signature.

$ openssl dgst -sha256 -verify <pub-key> -signature signature.bin isv_enclave.hex

# Run an SGX Enclave Application Guides

## prerequisitions
We need below files
```
app: An sgx enclave application software
gt_enclave.signed.so: the dynamic loading library file for an enclave
Lib/: Python library files
libsgx_uae_service.so: SGX library
```
You can copy both app and gt_enclave.signed.so from /home/accuser/platform/sharing/sgx and libsgx_uae_service.so from $(SGXSDK)/lib64

## System preparation
* Install SGX driver
* Install Intel SGX PSW
* Install cpprest library
```
$ sudo apt-get install libcpprest
```
* Install sgx library
```
$ sudo cp libsgx_uae_service.so /usr/lib
```

## Run an enclave
The commandline is as below.
```
$ ./app -a dapp_account -u dapp_url -e enclave_url -x enclave_external_url -d enclave_home_directory -f -l <src_path>:<dest_path>
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
-l (Lower case of 'L', if updating library use 'L' instead of 'l') loading libraries into the enclave. The format of this option is -l <full path of the source directory or file>:<destination path start with /lib>. The libraries are only need to be loaded for once.
```
Example command line for running an enclave:
```
$ ./app -a admin -u http://app-dev.secureailabs.com:4000 -e http://0.0.0.0:3080 -x http://ns556691.ip-54-39-19.net:3180 -d /root/enclaves/enclave1 -l ./Lib:/lib
```

# Installing and using Google Test for CPP unit tests

## Installation

1. Download and expand the source code. We will use Google Test 1.8.1.
   ```
   $ wget https://github.com/google/googletest/archive/release-1.8.1.tar.gz
   $ tar -xvzf release-1.8.1.tar.gz
   ```
1. Move the code to live under the `/usr/src/` folder.
   ```
   $ sudo mv googletest-release-1.8.1 /usr/src/gtest
   ```

## Running unit tests

1. Enter the directory you want to run tests in.
1. There is a makefile called `tests.mk` in each directory containing tests. `tests.mk` is not linked in by the main Makefile and needs to be run on its own.
1. To build tests:
   ```
   $ make -f tests.mk
   ```
1. This should generate test executables in the same directory, for example `conversion_tests`. Run the test by running the executable:
   ```
   $ ./conversion_tests
   ```
1. When you are done, clean up the generated files:
   ```
   $ make -f tests.mk clean
   ```

## Adding new unit tests

1. If the folder you want to add a test to doesn't already have a test makefile (a file called `tests.mk`), create one based on the existing test makefile in the `gt_enclave/` folder.
1. In the `gt_enclave/` folder, there is a sample unit test (`gt_enclave/conversion_tests.cpp`) for `conversion.cpp` and a sample test makefile (`gt_enclave/tests.mk`).
