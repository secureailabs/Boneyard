//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ai.h"
#include "sgx_tprotected_fs.h"

#include "cJSON.h"

using namespace std;

//extern char ad_json[];
//extern char patient_json[];

double ai_ad(string ad_json, char *patient_json)
{
     char * buffer;
	 //long length;

	 //buffer = ad_json.c_str();
/*
//get the GWAS file
FILE * f = fopen ("alzheimers_disease.json", "rb");

if (f)
{
  fseek (f, 0, SEEK_END);
  length = ftell (f);
  fseek (f, 0, SEEK_SET);
  buffer = malloc (length);
  if (buffer)
  {
    fread (buffer, 1, length, f);
  }
  fclose (f);
}
*/

  //printf("%s",buffer);
  //Parse the data
cJSON *root = cJSON_Parse(ad_json.c_str());

  //get the some values
//char *GenomeBuild = cJSON_GetObjectItem(root, "Build")->valuestring;
//char *Phenotype = cJSON_GetObjectItem(root, "Phenotype")->valuestring;
double baselineProb = cJSON_GetObjectItem(root, "baselineProb")->valuedouble;

//printf("Genome build: %s \nPhenotype: %s \nBaselineRisk: %fl \n",GenomeBuild, Phenotype,baselineProb);

   //Get the SNP information
cJSON *GWAS_SNPs = cJSON_GetObjectItem(root, "SNPs");
int GWAS_SNP_count = cJSON_GetArraySize(GWAS_SNPs);
//printf("Number of SNPs in GWAS: %d \n",GWAS_SNP_count);

    //define the array for data storage
#define SNP_NAME_SIZE 20
#define SNP_COUNT_SIZE 2
//char GWAS_SNP_names[GWAS_SNP_count][20];
Array<char[SNP_NAME_SIZE]> GWAS_SNP_names(GWAS_SNP_count);
//char GWAS_EAs[GWAS_SNP_count][2];
Array<char[SNP_COUNT_SIZE]> GWAS_EAs(GWAS_SNP_count);
//double GWAS_Betas[GWAS_SNP_count];
Array<double> GWAS_Betas(GWAS_SNP_count);
//double GWAS_EAFs[GWAS_SNP_count];
Array<double> GWAS_EAFs(GWAS_SNP_count);

    //read SNP data
//const int N=GWAS_SNP_count;
int i;
//for (i = 0; i < N; i++)
for (i = 0; i < GWAS_SNP_count; i++)
{
        //printf("SNP:\n");
        // Get the JSON element and then get the values as before
    cJSON *EachGWAS_SNP = cJSON_GetArrayItem(GWAS_SNPs, i);
    strncpy(GWAS_SNP_names[i],cJSON_GetObjectItem(EachGWAS_SNP, "SNP")->valuestring, SNP_NAME_SIZE);
    strncpy(GWAS_EAs[i],cJSON_GetObjectItem(EachGWAS_SNP, "EA")->valuestring, SNP_COUNT_SIZE);


    GWAS_Betas[i] = cJSON_GetObjectItem(EachGWAS_SNP, "Beta")->valuedouble;
    GWAS_EAFs[i] = cJSON_GetObjectItem(EachGWAS_SNP, "EAF")->valuedouble;

    //printf("SNP:%s\t EA:%s \t Beta: %.2fl EAF:%.2fl \n" ,GWAS_SNP_names[i],GWAS_EAs[i],GWAS_Betas[i],GWAS_EAFs[i]);

}

//printf("Finish loading GWAS data\n");


    // Finally remember to free the memory!
cJSON_Delete(root);

//Get Patient data
/*
FILE *f2 = fopen ("/Users/kaiyuanmifen/Google Drive/GeneTank/Project/PolygenicScore/PatientFiles/Patient_010/Patient010_Data.json", "rb");

if (f2)
{
  fseek (f2, 0, SEEK_END);
  length = ftell (f2);
  fseek (f2, 0, SEEK_SET);
  buffer = malloc (length);
  if (buffer)
  {
    fread (buffer, 1, length, f2);
  }
  fclose (f2);
}
*/
buffer = patient_json;

  //printf("%s",buffer);
  //Parse the data
  cJSON *Patient_data = cJSON_Parse(buffer);

    //int Patient_SNP_count = cJSON_GetArraySize(Patient_data);
    //printf("Number of SNPs in Patient: %d \n",Patient_SNP_count);

    //define the array for data storage
    //char Patient_SNP_names[GWAS_SNP_count][20];
	Array<char[SNP_NAME_SIZE]> Patient_SNP_names(GWAS_SNP_count);
    //char Patient_Al1[GWAS_SNP_count][2];
	Array<char[SNP_COUNT_SIZE]> Patient_Al1(GWAS_SNP_count);
    //char Patient_Al2[GWAS_SNP_count][2];
	Array<char[SNP_COUNT_SIZE]> Patient_Al2(GWAS_SNP_count);
	//char Overlap_EAs[GWAS_SNP_count][2];
	Array<char[SNP_COUNT_SIZE]> Overlap_EAs(GWAS_SNP_count);
	//double Overlap_Betas[GWAS_SNP_count];
	Array<double> Overlap_Betas(GWAS_SNP_count);
	//double Overlap_EAFs[GWAS_SNP_count];
	Array<double> Overlap_EAFs(GWAS_SNP_count);

    int NumberOverlap=0;
	
    //read SNPs exist in GWAS data and produce arrays for overlaped data
    int j;
    for (j = 0; j < GWAS_SNP_count; j++) {
        //printf("SNP:\n");
        // Get the JSON element and then get the values as before
        cJSON *EachSNP = cJSON_GetObjectItem(Patient_data, GWAS_SNP_names[j]);
        if (EachSNP)
        {
			strncpy(Patient_SNP_names[j],GWAS_SNP_names[j], SNP_NAME_SIZE);
			strncpy(Patient_Al1[j],cJSON_GetObjectItem(EachSNP, "Al1")->valuestring, SNP_COUNT_SIZE);
			strncpy(Patient_Al2[j],cJSON_GetObjectItem(EachSNP, "Al2")->valuestring, SNP_COUNT_SIZE);

			strncpy(Overlap_EAs[j],GWAS_EAs[j], SNP_COUNT_SIZE);
			Overlap_Betas[j]=GWAS_Betas[j];
			Overlap_EAFs[j]=GWAS_EAFs[j];
			NumberOverlap +=1;
		}
        //printf("SNP: %s \t Al1 %s \t Al2 %s EA %s \t Beta %fl \t EAF %fl \n ", Patient_SNP_names[j],Patient_Al1[j],Patient_Al2[j],Overlap_EAs[j],Overlap_Betas[j],Overlap_EAFs[j]);
    }

    //printf("Finish loading patient data\n Number of overlapped SNPs:%d \n",NumberOverlap);

    // Finally remember to free the memory!
    cJSON_Delete(Patient_data);

    //calculate polygenic score
    double PS=0;

    for (j = 0; j < GWAS_SNP_count; j++) {
        //printf("SNP:\n");
        // Get the JSON element and then get the values as before
        if (Patient_SNP_names[j])
        {
            if(strcmp(Patient_Al1[j],Overlap_EAs[j])==0)
            {
                PS+=Overlap_Betas[j];
            }
            if(strcmp(Patient_Al2[j],Overlap_EAs[j])==0)
            {
                PS+=Overlap_Betas[j];
            }
        }
    }

    //printf("Polygenic score %fl\n",PS);

    //calculate risk
    //mean score
    double MeanScore=0.0;
    for (j = 0; j < GWAS_SNP_count; j++) {
        //printf("SNP:\n");
        // Get the JSON element and then get the values as before

        if (Patient_SNP_names[j])
        {
            MeanScore = MeanScore+2*(Overlap_EAFs[j])*Overlap_Betas[j];
        }
    }
    //printf("mean score %fl\n",MeanScore);

    //CALCULATE RISK

    double Odds,BaslineOdds,absodds,absrisk;

    Odds=exp(PS-MeanScore);
    BaslineOdds=baselineProb/(1-baselineProb);
    absodds=BaslineOdds*Odds;
    absrisk=absodds/(1+absodds);

	return absrisk;
    //printf("trait risk is: %fl",absodds);
}

