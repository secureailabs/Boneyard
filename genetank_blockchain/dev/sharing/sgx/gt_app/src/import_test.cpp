/*
 * 		  Name: import_test.cpp
 * Description: copies test files to SFS
 *
 *  Created on: May 4, 2020
 *      Author: accuser
 */

#include "../include/stdafx.h"
#include "enclave_control.h"
#include "enclave_tab.h"
#include "../gt_enclave_u.h"

#include <dirent.h>
#include <string.h>
#include <stdlib.h>

sgx_status_t call_ec_import_test(const char *fileName, const uint8_t *buff, size_t size){
	sgx_status_t ret, retVal;
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	ret = ec_import_test(pTabNode->pEncl->getEnclaveId(), &retVal, fileName, strlen(fileName)+1, buff, size);
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retVal)
		return SGX_SUCCESS;
	return (sgx_status_t)-1;
}

sgx_status_t import_test_file(const char *test_dir, const char *fileName, size_t size, const char *destDir){
	FILE *fp;
	char fullName[FILENAME_MAX];
	snprintf(fullName, FILENAME_MAX, "%s", test_dir);
	fp = fopen(fullName, "r");
	if (NULL == fp)
		return (sgx_status_t)-1;
	uint8_t *buff = (uint8_t *)malloc(size);
	if (NULL == buff){
		fclose(fp);
		return (sgx_status_t)-1;
	}
	if (1 != fread(buff, size, 1, fp)){
		fclose(fp);
		free(buff);
		return (sgx_status_t)-1;
	}
	snprintf(fullName, FILENAME_MAX, "%s%s", destDir, fileName);
	sgx_status_t ret = call_ec_import_test(fullName, buff, size);
	free(buff);
	fclose(fp);
	return ret;
}

sgx_status_t walk_test_dir(const char *test_dir, const char *sub_dir, const char *destDir){
	struct dirent *dent;
	DIR *dir;
	struct stat st;
	char fn[FILENAME_MAX];
	int len = strlen(sub_dir);
	sgx_status_t ret;
	if ((strlen(test_dir) + len) >= FILENAME_MAX - 1)
		return (sgx_status_t)-1;

	strcpy(fn, sub_dir);
	char fullName[FILENAME_MAX];

	stat(test_dir, &st);
	ret = import_test_file(test_dir, fn, st.st_size, destDir);
	if (SGX_SUCCESS != ret){
		return (sgx_status_t)-1;
	}
	return SGX_SUCCESS;
}

/*
* 1- Generates test file paths using pytest_traverse_dir.py
* 2- Saves the file paths to enclaveHome/tmp/file_list.txt
* 3- Writes data from file_list.txt to test.dir
* 4- Deletes file_list.txt
*/
int import_tests(char *lib_dir, char *test_dir, char *enclaveHome)
{
	sgx_status_t ret;
	char *destDir;
	const char *script_name = "../pytest_traverse_dir.py";
	const char *file_name = "/tmp/file_list.txt";
	char test_file_list[FILENAME_MAX];
	snprintf(test_file_list, FILENAME_MAX, "%s%s", enclaveHome, file_name);

	if (!strcmp(test_dir, "")){
		std::cout<<"Test path is not provided, no test path is imported. Use \"-t source_path:/test/dest_path\" to specify the test path."<<std::endl;
		return 1;
	}
	else {
		std::string str(test_dir);
		std::size_t find = str.find(":/test");
		if (find == std::string::npos) {
			std::cout<<"To import test path, use the option \"-t source_path:/test/dest_path\" where \":/test\" must be provided."<<std::endl;
			return 1;
		}
		*(test_dir+find)='\0';
		destDir = test_dir+find+6;
		std::cout<<"Test paths are being imported from "<<test_dir<<" to /test"<<destDir<<" ...\n"<<std::endl;
	}

	// runs the python script
	std::string command = std::string("python ") + script_name + " " + lib_dir + " " + test_dir + " " + enclaveHome;
	system(command.c_str());

	// writes file paths to test.dir
	ret = walk_test_dir(test_file_list, "file_list.txt", destDir);
	if (SGX_SUCCESS == ret){
		std::cout<<"\nTest paths from "<<test_dir<<" imported."<<std::endl;
		remove(test_file_list);
	} else {
		std::cout<<"\nTest paths from "<<test_dir<<" importing interrupted or failed."<<std::endl;
	}
	return 0;
}
