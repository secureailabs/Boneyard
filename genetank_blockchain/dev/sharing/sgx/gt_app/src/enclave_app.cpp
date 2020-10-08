#include "stdafx.h"
#include "enclave_control.h"
#include "enclave_tab.h"
#include "network_listener.h"
#include "network_client.h"
#include "../gt_enclave_u.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <fstream>

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
	  <<cmd<<" -u dapp_url -a dapp_account -e enclave_url -d enclave_home_directory -f -r"<<std::endl
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
	  <<"   Be careful when use -f option, make sure there is no other enclave instance is running"<<std::endl
	  <<"-t Import test paths from the path provided by this option."<<std::endl
	  <<"-r To perform remote attestation for the enclave"<<std::endl;
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

sgx_status_t remoteAttestation(){
	sgx_status_t ret;
	sgx_report_t pReport;
	char* attestation_report;
	attestation_report = (char *) malloc(20001);

	//Provide the location of json file containing SPID and Attestation Server URL
	std::ifstream infile("../gt_app/config.json", std::ifstream::binary);
	if(!infile){
		std::cout << "\nFile does not exist" << std::endl;
		ret = SGX_ERROR_INVALID_PARAMETER;
		return ret;
	}

	sgx_spid_t spid;
	json::value buffer_val;

	infile >> buffer_val;
	std::string att_server = buffer_val["att_server_url"].as_string();
	char att_server_url[att_server.size() + 1];
	att_server.copy(att_server_url, att_server.size() + 1);
	att_server_url[att_server.size()] = '\0';

	int arr[32];
	std::string buff = buffer_val["SPID"].as_string();
	int i=0;
	while(i<32){
		arr[i] = buff[i] - '0';
		if(arr[i] > 48)
			arr[i] = arr[i] - 39;
		else if(arr[i] > 16)
			arr[i] = arr[i] - 7;
		i++;
	}

	for(i=0; i<16; i++){
		spid.id[i] = arr[2*i]*16 + arr[2*i + 1];
	}

	ret = oc_sendAttReq(att_server_url, &pReport, &spid, (char *) attestation_report, (size_t)(20000));
	FILE *fp = fopen("./attestation.txt", "w");
    if(fp!=NULL){
    	size_t newLen=fwrite(attestation_report,sizeof(char),(size_t)(20000),fp);
    	if(ferror(fp)!=0){
    		fputs("Error writing into file", stderr);
    	}
    }
    else{
    	return sgx_status_t(-1);
    }
    fclose(fp);
	free(attestation_report);
	return ret;
}

int import_libs(char *lib_dir, bool forceImport);
int import_tests(char *lib_dir, char *test_dir, char *enclaveHome);

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
	char testDir[MAX_PATH] = { '\0' };
	bool forceImportLib = false;
	bool doRemoteAttestation = false;

	int fflag=0;
	int c;

	opterr = 0;
	while ((c = getopt(argc, argv, "fru:a:e:x:d:l:L:t:")) != -1)
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
	      case 't':
    			if( optarg ) strncpy( testDir, optarg, MAX_PATH);
    			break;
	      case 'r':
	    	  doRemoteAttestation = true;
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

	std::cout<<"Registering to Dapp at: "<<dappUrl<<std::endl;
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	while (NULL != pTabNode) {
		std::cout<<"Registering to Dapp at: "<<dappUrl<<std::endl;
		sgx_status_t ret = pTabNode->pEncl->reg_dapp((char *)dappUrl,
		 (char *)enclaveUrlEx, (char *)account);
			if ( SGX_SUCCESS == ret)
		  break;
		else if ((sgx_status_t)-2 == ret) {
		  sleep(20);
		} else {
		  clearLock(enclaveHome);
		  std::cout<<"Register Dapp fail. Exiting ...\n"<<std::endl;
		  return -1;
		}
	}

	// Perform remote attestation only if doRemoteAttestation is true
	if(doRemoteAttestation){
		sgx_status_t ret;
		ret = remoteAttestation();

		if(SGX_SUCCESS != ret){
			std::cout<<"\n\nRemote attestation error " << ret << std::endl;
			return 1;
		} else {
			std::cout << "\nAttestation successful\n" << std::endl;
		}
	}


	import_libs(libPath, forceImportLib);

  // when user runs app with -t
	if( testDir ) {
		import_tests(libPath, testDir, enclaveHome);
	}

	std::cout << "Setting up network listener.. " << std::endl;
	utility::string_t address(enclaveUrl);
	listener_initialize(address);

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
