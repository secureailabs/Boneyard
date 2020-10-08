import os, sys

# used for pytest
# creates list of paths to test paths and writes to a file
def main(root_dir, directory, enclave_dir):
	file_list = enclave_dir + "/tmp/file_list.txt"
	file_set = set()

	#creates a list of paths
	for dir_, _, files in os.walk(directory):
	    for file_name in files:
	        if file_name.endswith(".py") and file_name != "__init__.py":
	            if file_name.startswith("test") or file_name.endswith("conftest.py"):
		            rel_dir = os.path.relpath(dir_, root_dir)
		            rel_file = os.path.join(rel_dir, file_name)
		            file_set.add(rel_file)

	#writes to the file
	with open(file_list, 'w') as fp:
	    for file in file_set:
	        fp.write(file + '\n')

if __name__ == "__main__":
	main(sys.argv[1], sys.argv[2], sys.argv[3])
