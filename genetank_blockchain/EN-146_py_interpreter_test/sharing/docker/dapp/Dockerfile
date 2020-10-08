FROM ubuntu:18.04

# Install git
RUN apt update

RUN apt install -y git

# Install c/c++ tool kit.
RUN apt-get update && apt-get install -y build-essential 

# Install python3.6 and pip3
RUN apt-get update && apt-get install -y python3.6 python3-pip

# Install Jupyterlab
RUN pip3 install jupyterlab

# Install python2.7 and pip
RUN apt-get update && apt-get install -y python2.7 python-pip

# Install Jupyterlab Kernels for Python2 and Python3
RUN python2 -m pip install ipykernel
RUN python2 -m ipykernel install
RUN python3 -m pip install ipykernel
RUN python3 -m ipykernel install
RUN jupyter kernelspec remove -f python3
RUN jupyter kernelspec remove -f python2

# Install Enclave Kernel for Jupyterlab
ENV PYTHON_LIB_PATH /usr/local/lib/python3.6/dist-packages
COPY ./jupyter/enclave_kernel ${PYTHON_LIB_PATH}/enclave_kernel
RUN python3 -m enclave_kernel.install

# Install Node.js
# Reference: https://tecadmin.net/install-latest-nodejs-npm-on-ubuntu/
RUN apt-get install -y curl

RUN curl -sL https://deb.nodesource.com/setup_12.x | bash -

RUN apt-get install -y nodejs

# Install Meteor
RUN curl https://install.meteor.com/ | sh

# Create dapp user and home directory
RUN groupadd -r dapp && useradd -r -g dapp dapp
ENV HOME_PATH /home/dapp
RUN mkdir ${HOME_PATH}
RUN chown dapp:dapp /home/dapp

USER dapp

# Copy the Jupyter configuration file
ENV JUPYTER_HOME ${HOME_PATH}/jupyter
RUN mkdir ${JUPYTER_HOME}
COPY --chown=dapp:dapp ./jupyter/jupyter_notebook_config.py ${JUPYTER_HOME}

# Copy the dApp
ENV DAPP_PATH ${HOME_PATH}/dapp
COPY --chown=dapp:dapp ./dapp ${DAPP_PATH}

# Initialize the packages of the dApp
WORKDIR ${DAPP_PATH}
RUN meteor update
RUN meteor npm install --save
# RUN npm audit fix

# Create the setting file for the initial admin account
RUN echo '{ "defaultAccount": [ { "username": "admin", "email": "admin@admin.com", "password": "admin", "role": "admin" } ] }' > settings.json 

# Run the meteor when run the container
CMD meteor --port 4000 --settings settings.json 2>&1 >>dapp.log
# USER root

# Install nginx
# RUN apt-get update

# RUN apt-get install -y nginx

# Install Certbot
# RUN apt-get update
# RUN apt-get install -y software-properties-common
# RUN add-apt-repository universe
# RUN add-apt-repository ppa:certbot/certbot
# RUN apt-get update
# RUN apt-get install -y certbot python-certbot-nginx

# openssl genrsa -out dapp.secureailabs.com.key 2048
# openssl req -new -sha256 -key dapp.secureailabs.com.key -out dapp.secureailabs.com.csr
# openssl x509 -req -days 365 -in dapp.secureailabs.com.csr -signkey dapp.secureailabs.com.key -out dapp.secureailabs.com.crt
# openssl req -text -in dapp.secureailabs.com.csr -noout



