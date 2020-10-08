# pytest Configuration

Run your sgx app with the following command in your command line:

./app -f -a admin -u http://127.0.0.1:3000 -e http://0.0.0.0:3250 -x http://0.0.0.0:3250 -d /home/user/enclave/enclave1 -l /home/user/genetank_blockchain/sharing/sgx/gt_enclave/pyvm/Lib:/lib/ -t /home/user/genetank_blockchain/sharing/sgx/gt_enclave/pyvm/Lib/[library_directory_name]:/test/

You can now execute tests for the desired library by running the following code using dapp:

```
import pytest
pytest.main([‘library_directory_name’])
```
You can also add an option by adding it to the arguments array passed to pytest.main. For example:

```
pytest.main([‘library_directory_name’, ‘collectonly’])
```
pytest will run all files in the library_directory_name and its subdirectories. The tests results will be shown on the console and can also be accessed from pytest_output.txt under sgx folder.
