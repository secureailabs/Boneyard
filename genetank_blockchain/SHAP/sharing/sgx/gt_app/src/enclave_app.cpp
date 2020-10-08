#include "stdafx.h"
#include "enclave_control.h"
#include "enclave_tab.h"
#include "network_listener.h"
#include "network_client.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>

// Needed to query extended epid group id.
#include "sgx_uae_epid.h"

void getpassword(char *password,size_t pwSize)
{
    static struct termios old_terminal;
    static struct termios new_terminal;

    //get settings of the actual terminal
    tcgetattr(STDIN_FILENO, &old_terminal);

    // do not echo the characters
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ECHO);

    // set this as the new terminal options
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

    // get the password
    // the user can add chars and delete if he puts it wrong
    // the input process is done when he hits the enter
    // the \n is stored, we replace it with \0
    if (fgets(password, pwSize, stdin) == NULL)
        password[0] = '\0';
    else
        password[strlen(password)-1] = '\0';

    // go back to the old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
}


#define BUFSIZE80 80
#define LOCKFILENAME "lock"
#define defaultDappUrl "http://localhost:3200"
#define defaultEnclaveUrl "http://0.0.0.0:4300"
#define defaultAccount "admin"
#define defaultDir ".enclave"

void usage(const char *cmd//, const char *dapp_url, const char *account, const char *enc_url
		) {
	std::cout<<"Usage:"<<std::endl
	  <<cmd<<" -u dapp_url -a dapp_account -e enclave_url -d enclave_home_directory -f"<<std::endl
	  <<"-u The URL of the Dapp Server that manages this enclave. ("<<defaultDappUrl<<")"<<std::endl
	  <<"-a The account name in the Dapp which is the enclave owner. ("<<defaultAccount<<")"<<std::endl
	  <<"-e The URL of this enclave. ("<<defaultEnclaveUrl<<")"<<std::endl
	  <<"-l Import library files from the path provided by this option. No library will be imported if library has been imported"<<std::endl
	  <<"-L Import library files. Same as -l option but force to import even the library existed in the enclave"<<std::endl
	  <<"-d The path and directory of the enclave data." <<std::endl
	  <<"    without -d option, use "<<defaultDir<<"/ current directory"<<std::endl
	  <<"    -d without value, use "<<defaultDir<<"/ under home directory: "<<std::endl
	  <<"-f This option has no a value parameter. If presents, forces to break the lock which"<<std::endl
	  <<"   prevent multiple enclave instances"<<std::endl
	  <<"   Be careful when use -f option, make sure there is no other enclave instance is running"<<std::endl;
}

int getLockPid(const char *filename){
    int pid;

	FILE *fp = fopen(filename,"r");
	if (NULL != fp){
		fscanf(fp,"%d",&pid);
		fclose(fp);
		return pid;
	}
	return -1;
}

void clearLock(const char *home){
	char lockFileName[MAX_PATH];

	snprintf(lockFileName,MAX_PATH,"%s/%s",home,LOCKFILENAME);
	unlink(lockFileName);
}

int checkLock(const char *home, int fflag){
	char lockFileName[MAX_PATH];
	int pid;
	int myPid = getpid();

	snprintf(lockFileName,MAX_PATH,"%s/%s",home,LOCKFILENAME);

	pid = getLockPid(lockFileName);
	if (-1 != pid)
		if (pid != myPid) {
			if (0 == fflag)
		      return 1;
			clearLock(home);
		}
	FILE *fp = fopen(lockFileName,"a");
	if (NULL == fp)
		return 2;
	fprintf(fp,"%d,",myPid);
	fclose(fp);

	pid = getLockPid(lockFileName);
	if (pid != myPid)
	  return 1;

	return 0;
}

int import_libs(char *lib_dir, bool forceImport);
//
// To start the enclave, run the below command with admin privileges:
// gt_app.exe <port>
// If port is not specified, will listen on 4300
//
#define _T(x) x
int main(int argc, char* argv[])
{
	/* command line options handling */
	opterr = 0;
	char dappUrl[MAX_PATH] = { '\0' };
	char enclaveUrl[MAX_PATH] = { '\0' };
	char enclaveUrlEx[MAX_PATH] = { '\0' };
	char enclaveHome[MAX_PATH] = { '\0' };
	char libPath[MAX_PATH] = { '\0' };
	char account[BUFSIZE80] = { '\0' };
	bool forceImportLib = false;

	int fflag=0;
	int c;

	opterr = 0;
	while ((c = getopt(argc, argv, "fu:a:e:x:d:l:L:")) != -1)
	    switch (c)
	      {
	      case 'u':
	        strncpy((char *)dappUrl,(const char *)optarg, MAX_PATH);
	        break;
	      case 'a':
	    	strncpy(account, optarg, BUFSIZE80);
	    	break;
	      case 'e':
	        strncpy(enclaveUrl,optarg, MAX_PATH);
	        break;
	      case 'x':
	        strncpy(enclaveUrlEx,optarg, MAX_PATH);
	        break;
	      case 'd':
	        strncpy(enclaveHome,optarg, MAX_PATH);
	        break;
	      case 'L':
	    	forceImportLib = true;
	        strncpy(libPath,optarg, MAX_PATH);
	        break;
	      case 'l':
	        strncpy(libPath,optarg, MAX_PATH);
	        break;
	      case 'f':
	    	fflag=1;
	    	break;
	      case '?':
	        if (optopt == 'u' || optopt == 'a' || optopt == 'e' || optopt == 'x')
	          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	        else if (optopt == 'd'){
	        	char path[MAX_PATH];
	        	if (getcwd(path, sizeof(path)) != NULL) {

	            	snprintf(enclaveHome, MAX_PATH, "%s/%s", path, defaultDir);
	        	}
	        }
	        else {
	          if (isprint (optopt))
	               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	          else
	               fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	          usage(argv[0]);
	          return 1;
	        }
	        break;
	      default:
	        usage(argv[0]);
	        return 1;
	}
	if (dappUrl[0]==0)
		strncpy(dappUrl,defaultDappUrl,MAX_PATH);
	if (account[0]==0)
		strncpy(account,defaultAccount,BUFSIZE80);
	if (enclaveUrl[0]==0)
		strncpy(enclaveUrl,defaultEnclaveUrl,MAX_PATH);
	if (enclaveUrlEx[0]==0)
		strncpy(enclaveUrl,defaultEnclaveUrl,MAX_PATH);
	if (enclaveHome[0]==0){
    	snprintf(enclaveHome, MAX_PATH, "%s/%s", getpwuid(getuid())->pw_dir, defaultDir);
	}

	struct stat st = {0};

	if (stat(enclaveHome, &st) == -1)
		mkdir(enclaveHome, 0700);

	char tmp[MAX_PATH];
	snprintf(tmp, MAX_PATH, "%s/tmp", enclaveHome);
	if (stat(tmp, &st) == -1)
		mkdir(tmp, 0700);

	if (checkLock(enclaveHome,fflag)) {
		std::cout<<"There is another SGX enclave application running. Software exiting"<< std::endl;
		std::cout<<"If the other enclave instance exited without cleaning the lock, "
		  <<"use -f option to run the enclave again."<< std::endl;
        return 1;
	}

	/* Initialize the enclave */
	std::cout<<"Initializing Enclave"<<std::endl;
	enclaveTab = new EnclaveTab();
	if (NULL == enclaveTab) {
		std::cout<<"Create enclave table failed.\n"<<std::endl;
		clearLock(enclaveHome);
		return -1;
	}
	if (SGX_SUCCESS != enclaveTab->initEnclaveHome(enclaveHome)){
		std::cout<<"Initialize enclave home directory failed.\n"<<std::endl;
		clearLock(enclaveHome);
		return -1;
	}
	if (SGX_SUCCESS != enclaveTab->initMutex()){
		std::cout<<"Initialize enclave table mutex failed.\n"<<std::endl;
		clearLock(enclaveHome);
		return -1;
	}
	if (SGX_SUCCESS != enclaveTab->initEnclave0()){
		std::cout<<"Create the first enclave failed.\n"<<std::endl;
		clearLock(enclaveHome);
		return -1;
	}
	std::cout<<"Enclave initialized"<<std::endl;

	utility::string_t address(enclaveUrl);
	listener_initialize(address);

	std::cout<<"Registering to Dapp at: "<<dappUrl<<std::endl;
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	if (NULL == pTabNode || SGX_SUCCESS != (pTabNode->pEncl->reg_dapp((char *)dappUrl, (char *)enclaveUrlEx, (char *)account)<0)) {
		std::cout<<"Register Dapp fail. Exiting ...\n"<<std::endl;
		clearLock(enclaveHome);
		return -1;
	}
	import_libs(libPath, forceImportLib);

	//enclaveTab->initFreeEnclaves();

	std::cout << "To exit press return twice." << std::endl;

	std::string line;
	std::getline(std::cin, line);
	std::getline(std::cin, line);

	listener_shutdown();
	clearLock(enclaveHome);
	std::cout << "Good bye." << std::endl;
	return 0;
}
