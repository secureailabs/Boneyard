#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ai.h"

#include "cJSON.h"

using namespace std;
using namespace web;
using namespace utility;

//extern char *ad_json;
//extern char patient_json[];

//double ai_ad(char *patient_json)
double ai_ad(json::value &patient_json)
{
     char * buffer1;
	 long length;

	 //buffer = ad_json.c_str();

//get the GWAS file
	 FILE * f;
	 fopen_s(&f,"alzheimers_disease.json", "rb");

if (f)
{
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 0, SEEK_SET);
  buffer1 = (char *)malloc (length);
  if (buffer1)
  {
    fread (buffer1, 1, length, f);
  }
  else {
	  fclose(f);
	  return -1;
  }
  fclose (f);
}
else
  return -1;


  //printf("%s",buffer);
//Parse the data
//cJSON *root = cJSON_Parse(buffer1);
#ifdef WINDOWS
wchar_t *wstr = (wchar_t *)malloc((length+1)*sizeof(wchar_t));
size_t len;
if (NULL == wstr) {
	free(buffer1);
	return -1;
}
//mbstowcs_s(&len,wstr,length+1,buffer1,length);
mbstowcs(wstr,buffer1,length);
utility::string_t gwas_str(wstr);
json::value gwas_json = json::value::parse(gwas_str);
free(wstr);
#endif

json::value gwas_json = json::value::parse(buffer1);

  //get the some values
//char *GenomeBuild = cJSON_GetObjectItem(root, "Build")->valuestring;
//char *Phenotype = cJSON_GetObjectItem(root, "Phenotype")->valuestring;
//double baselineProb = cJSON_GetObjectItem(root, "baselineProb")->valuedouble;
double baselineProb = gwas_json.at(U("baselineProb")).as_double();

//printf("Genome build: %s \nPhenotype: %s \nBaselineRisk: %fl \n",GenomeBuild, Phenotype,baselineProb);

   //Get the SNP information
//cJSON *GWAS_SNPs = cJSON_GetObjectItem(root, "SNPs");
json::value gwasSnp_json = gwas_json.at(U("SNPs"));
//int GWAS_SNP_count = cJSON_GetArraySize(GWAS_SNPs);
size_t GWAS_SNP_count = gwasSnp_json.size();
//printf("Number of SNPs in GWAS: %d \n",GWAS_SNP_count);

size_t Patient_SNP_count = patient_json.size();

patientSnp *patient = new patientSnp[GWAS_SNP_count];

int NumberOverlap=0;
	
    //read SNPs exist in GWAS data and produce arrays for overlaped data
int j;
for (j = 0; j < GWAS_SNP_count; j++) {
	json::value gwas_snp_json = gwasSnp_json.at(j);
	utility::string_t snp_name = gwas_snp_json.at(U("SNP")).as_string();
	if (patient_json.has_field(snp_name)) {
		char eas = (char)(gwas_snp_json.at(U("EA")).as_string()[0]);
		double betas = gwas_snp_json.at(U("Beta")).as_double();
		double eafs = gwas_snp_json.at(U("EAF")).as_double();
		//int index = index_json.at(snp_name).as_integer();
	    json::value snp_json = patient_json.at(snp_name);
	    patient[j].valid = true;
		patient[j].al1 = (char)(snp_json.at(U("Al1")).as_string()[0]);// cJSON_GetObjectItem(EachSNP, "Al1")->valuestring[0];
		patient[j].al2 = (char)(snp_json.at(U("Al2")).as_string()[0]);// cJSON_GetObjectItem(EachSNP, "Al2")->valuestring[0];
		patient[j].overlap_eas = eas;
		patient[j].overlap_betas = betas;
		patient[j].overlap_eafs = eafs;
		NumberOverlap +=1;
	}
}

double PS=0;

for (j = 0; j < GWAS_SNP_count; j++) {
        //printf("SNP:\n");
        // Get the JSON element and then get the values as before
	if (patient[j].valid)//snp_name[0]) //Patient_SNP_names[j])
    {
            if (patient[j].al1==patient[j].overlap_eas)//(strcmp(Patient_Al1[j],Overlap_EAs[j])==0)
            {
				PS += patient[j].overlap_betas;// Overlap_Betas[j];
            }
            if(patient[j].al2==patient[j].overlap_eas)//strcmp(Patient_Al2[j],Overlap_EAs[j])==0)
            {
				PS += patient[j].overlap_betas;// Overlap_Betas[j];
            }
    }
}

    //calculate risk
    //mean score
double MeanScore=0.0;
for (j = 0; j < GWAS_SNP_count; j++) {
        //printf("SNP:\n");
        // Get the JSON element and then get the values as before

	if (patient[j].valid)//snp_name[0])//Patient_SNP_names[j])
    {
			MeanScore = MeanScore + 2 * patient[j].overlap_eafs*patient[j].overlap_betas;
										//(Overlap_EAFs[j])*Overlap_Betas[j];
    }
}
    //printf("mean score %fl\n",MeanScore);

    //CALCULATE RISK

double Odds,BaslineOdds,absodds,absrisk;

Odds=exp(PS-MeanScore); // |MeanScore| 0~1000, |PS|: 0~1000 
BaslineOdds=baselineProb/(1-baselineProb);
absodds=BaslineOdds*Odds;
absrisk=absodds/(1+absodds); //absrisk:0~1

if (NULL != buffer1)
	free(buffer1);
delete[] patient;

return absrisk;
    //printf("trait risk is: %fl",absodds);
}

