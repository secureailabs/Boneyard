# Juputer lab commands
The commands are start with #% at the first line of a code cell
There are below commands: <> means required parameters, [] means optional parameters
Configure the current hub enclave or sub enclave. If there is a SubEnclave parameter then it will talk to the sub enclave
```
#%config {Enclave: <enclave_name>, [SubEnclave: <subEnclave_name>]}
```
Show current enclave configuration
```
#%status
```
Show current directory content
```
#%ls [path]
```
Change current directory
```
#%cd <path>
```
Delete a file or a directory
```
#%rm <path>
```
Upload a piece of code start from the second line of current cell
```
#%upload
<codes to be uploaded>
```
