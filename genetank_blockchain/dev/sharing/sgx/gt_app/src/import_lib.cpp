/*
 * import_lib.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: accuser
 */
#include "stdafx.h"
#include "enclave_control.h"
#include "enclave_tab.h"
#include "../gt_enclave_u.h"

#include <dirent.h>
#include <string.h>

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

sgx_status_t call_ec_import_lib(const char *fileName, const uint8_t *buff, size_t size){
	sgx_status_t ret, retVal;
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	ret = ec_import_lib(pTabNode->pEncl->getEnclaveId(), &retVal, fileName, strlen(fileName)+1, buff, size);
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retVal)
		return SGX_SUCCESS;
	return (sgx_status_t)-1;
}

bool check_lib_import(char *destDir, bool forceImport){
	int imported;
	char rootPath[] = "/.";
	char *path = rootPath;
	if ('\0' != *destDir)
		path = destDir;
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	sgx_status_t ret, retVal;
	ret = ec_lib_imported(pTabNode->pEncl->getEnclaveId(), &retVal, path, strnlen(path, MAX_PATH)+1, &imported);
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retVal)
			return (0 == imported) || forceImport;
	return false;
}

sgx_status_t import_lib_file(const char *lib_dir, const char *fileName, size_t size, const char *destDir){
	FILE *fp;
	char fullName[FILENAME_MAX];
	snprintf(fullName, FILENAME_MAX, "%s%s", lib_dir, fileName);
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
	sgx_status_t ret = call_ec_import_lib(fullName, buff, size);
	free(buff);
	fclose(fp);
	return ret;
}

sgx_status_t walk_lib_dir(const char *lib_dir, const char *sub_dir, const char *destDir){
	struct dirent *dent;
	DIR *dir;
	struct stat st;
	char fn[FILENAME_MAX];
	int len = strlen(sub_dir);
	if ((strlen(lib_dir) + len) >= FILENAME_MAX - 1)
		return (sgx_status_t)-1;

	strcpy(fn, sub_dir);

	char fullName[FILENAME_MAX];
	snprintf(fullName, FILENAME_MAX, "%s%s", lib_dir, sub_dir);


	if (!(dir = opendir(fullName))) {
	    if(errno == ENOTDIR) {
	    	if(0!=stat(lib_dir, &st))
	    		return sgx_status_t(-1);
	    	import_lib_file(lib_dir, fn, st.st_size, destDir);
	    }
		return SGX_SUCCESS;
	}

	fn[len++] = '/';
	while ((dent = readdir(dir))) {
		if (dent->d_name[0] == '.')
			continue;

		strncpy(fn + len, dent->d_name, FILENAME_MAX - len);
		snprintf(fullName, FILENAME_MAX, "%s%s", lib_dir, fn);
		if (lstat(fullName, &st) == -1) {
			continue;
		}

		/* don't follow symlink unless told so */
		if (S_ISLNK(st.st_mode))
			continue;

		/* will be false for symlinked dirs */
		if (S_ISDIR(st.st_mode)) {
			/* recursively follow dirs */
			sgx_status_t ret;
			ret = walk_lib_dir(lib_dir, fn, destDir);
			if (SGX_SUCCESS != ret)
				return ret;
		} else {
			int ch = kbhit();
			if (0 != ch){
				std::string line;
				std::getline(std::cin, line);
				return (sgx_status_t)-1;
			}
			import_lib_file(lib_dir, fn, st.st_size, destDir);
		}
	}

	if (dir) closedir(dir);
	return SGX_SUCCESS;
}

int import_libs(char *lib_dir, bool forceImport)
{
	sgx_status_t ret;
	char *destDir;

	if (!strcmp(lib_dir, "")){
		std::cout<<"Library path is not provided, no library is imported. Use \"-l source_path:/lib/dest_path\" to specify the library path."<<std::endl;
		return 1;
	}
	else {
		std::string str(lib_dir);
		std::size_t find = str.find(":/lib");
		if (find == std::string::npos) {
			std::cout<<"To import library, use the option \"-l source_path:/lib/dest_path\" where \":/lib\" must be provided."<<std::endl;
			return 1;
		}
		*(lib_dir+find)='\0';
		destDir = lib_dir+find+5;
		std::cout<<"Library files are being imported from "<<lib_dir<<" to /lib"<<destDir<<" ...\n"<<std::endl;
	}
	if (check_lib_import(destDir, forceImport)){
		std::cout<<"Press return to stop importing...\n"<<std::endl;
		ret = walk_lib_dir(lib_dir, "", destDir);
		if (SGX_SUCCESS == ret){
			std::cout<<"\nLibrary "<<lib_dir<<" imported."<<std::endl;
		} else
			std::cout<<"\nLibrary "<<lib_dir<<" importing interrupted or failed."<<std::endl;
	} else
		std::cout<<"No library imported this time."<<std::endl;
	return 0;
}


