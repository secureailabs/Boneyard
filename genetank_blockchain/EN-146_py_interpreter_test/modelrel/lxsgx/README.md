# Genetank SGX

This is the directory for all the codes and data for SGX enclave.

# A guide for attestation and signing flow

The flow of an attestation:
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


Single step signing:

$ "C:\Program Files (x86)\Intel\IntelSGXSDK\bin\win32\release\sgx_sign.exe" sign -key "isv_enclave_private.pem" -enclave "C:\zsf\RemoteAttestation\x64\Debug\isv_enclave.dll" -out "C:\zsf\RemoteAttestation\x64\Debug\isv_enclave.signed.dll" -config "isv_enclave.config.xml"


Two step signing (actually three steps):

Step 1: generate the data for signing from the enclave image file and configurations.

$ <SGXSDK path>\bin\win32\release\sgx_sign gendata -enclave isv_enclave.dll -config isv_enclave.config.xml -out isv_enclave_hash.bin

Step #: signing with openssl or hardware private key module. See below for openssl signing.

Step 2: generate the signed enclave image file.

$ <SGXSDK path>\bin\win32\release\sgx_sign catsig -enclave isv_enclave.dll -config isv_enclave.config.xml -out isv_enclave_signed.dll -key isv_enclave_public.pem -sig signature.bin -unsigned isv_enclave_hash.bin

Example of 2 steps signing:

"$(SGXSDKInstallPath)bin\win32\release\sgx_sign.exe" gendata -enclave "$(OutDir)isv_enclave.dll" -config "isv_enclave.config.xml" -out "$(OutDir)isv_enclave_hash.bin"

"\OpenSSL-Win64\bin\openssl.exe" dgst -sha256 -out "$(OutDir)signature.bin" -sign "isv_enclave_private.pem" -keyform PEM "$(OutDir)isv_enclave_hash.bin"

"$(SGXSDKInstallPath)bin\win32\release\sgx_sign.exe" catsig -enclave "$(OutDir)isv_enclave.dll" -config "isv_enclave.config.xml" -out "$(OutDir)isv_enclave_signed.dll" -key "isv_enclave_public.pem" -sig "$(OutDir)signature.bin" -unsigned "$(OutDir)isv_enclave_hash.bin"

# OpenSSL Examples

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
