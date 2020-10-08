#pragma once
#include "../include/stdafx.h"

#define SNP_NAME_SIZE 2
/*
class gwasSnp {
public:
	gwasSnp() 
		snp_name[0] = 0;
		eas = 0;
		betas = 0;
		eafs = 0;
	}
	//void set_snp_name(char *name);
    void set_snp_name(utility::string_t &name);
		size_t len;
		wcstombs_s(&len, snp_name, name.c_str(), SNP_NAME_SIZE);
		//strncpy_s(snp_name, name, SNP_NAME_SIZE);
	
	char snp_name[SNP_NAME_SIZE];
	char eas;
	double betas;
	double eafs;
}
*/

class patientSnp {
public:
	patientSnp(){
		//snp_name[0] = 0;
		valid = false;
		al1 = 0;
		al2 = 0;
		overlap_eas = 0;
		overlap_betas = 0;
		overlap_eafs = 0;
	}
	//void set_snp_name(char *name);
//	void set_snp_name (utility::string_t &name){
//		size_t len;
//		wcstombs_s(&len, snp_name, name.c_str(), SNP_NAME_SIZE);
		//strncpy_s(snp_name, name, SNP_NAME_SIZE);
//	}
//	char snp_name[SNP_NAME_SIZE];
	bool valid;
	char al1;
	char al2;
	char overlap_eas;
	double overlap_betas;
	double overlap_eafs;
};

//double ai_ad(char *patient_json);
double ai_ad(web::json::value & data_json);

