See below webpage for details:

https://software.intel.com/en-us/articles/how-to-create-self-signed-certificates-for-use-with-intel-sgx-remote-attestation-using

If you want to decode certificates on your own computer, run this OpenSSL command:
openssl x509 -in certificate.crt -text -noout

Openssl path: C:\zsf\vcpkg\installed\x64-windows\tools\openssl\openssl.exe

Enclave signing key management:
The following command lines are typical examples using OpenSSL*.

1. Generate a 3072-bit RSA private key. Use 3 as the public exponent value.
> openssl genrsa -out private_key.pem -3 3072

2. Produce the public part of a private RSA key.
> openssl rsa -in private_key.pem -pubout -out public_key.pem

3. Sign the file containing the enclave signing material.
> openssl dgst -sha256 -out signature.hex -sign private_key.pem -keyform PEM enclave_hash.hex 