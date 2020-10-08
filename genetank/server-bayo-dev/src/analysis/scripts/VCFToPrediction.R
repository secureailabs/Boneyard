# Generate CSV files based on 23andMe data

# Check packages, installing if necessary
if (!require("VariantAnnotation",character.only = TRUE))
{
        source("http://bioconductor.org/biocLite.R") 
        biocLite("VariantAnnotation") 
        
        if(!require("VariantAnnotation",character.only = TRUE)) stop("Package not found")
}
library("VariantAnnotation") # Load the package

# Read in arguments
PatientID=as.character(commandArgs(trailingOnly = T)[[1]][1])
InputAddress=as.character(commandArgs(trailingOnly = T)[[2]][1])
OutputAddress=as.character(commandArgs(trailingOnly = T)[[3]][1])
OutputAddress=paste0(OutputAddress,"/Patient_",PatientID)
ReferenceAddress="./reference/"

# Read VCF
VCF=paste0(OutputAddress,'/',list.files(OutputAddress)[grepl(pattern = '.vcf',list.files(OutputAddress))])
# VCF=paste0(OutputAddress,'/',PatientID,'.vcf')
Target=readVcf(VCF,"hg19")
print('read VCF')
print(VCF)

GT <-geno(Target)$GT

GTvariants=unlist(lapply(strsplit(GT,'[/]'),FUN = function(x){sum(as.integer(x))}))
names(GTvariants)=rownames(GT)

# Get the GWAS informaiton 
GWAScatalog=read.csv('./GWASCatalogLinkedToPNMe.csv')
GWASsubInfor=GWAScatalog[,c('PatientLikeMeName',"SNPS")]

# Matching (3 situations): matching >0, matching =0, non-matching 
DiseaseReport=GWASsubInfor
DiseaseReport$Risk='Unknown'
DiseaseReport=DiseaseReport[!is.na(DiseaseReport$PatientLikeMeName),]
VecScore=GTvariants[as.character(DiseaseReport$SNPS)]
DiseaseReport$Risk[which(VecScore>0)]='Positive'
DiseaseReport$Risk[which(VecScore==0)]='Negative'

# Aggregate the results
DiseaseReport=split(DiseaseReport,f = DiseaseReport$PatientLikeMeName)

GetType=function(x){
        AllTypes=unique(x$Risk)
        x$PatientLikeMeName=as.character(x$PatientLikeMeName)
        if("Positive"%in%AllTypes){return(c(PhenotypeName=x$PatientLikeMeName[1],Risk='Positive'))} 
        if((!"Positive"%in%AllTypes)&("Negative"%in%AllTypes)){return(c(PhenotypeName=x$PatientLikeMeName[1],Risk='Negative'))}
        if((AllTypes=='Unknown')){return(c(PhenotypeName=x$PatientLikeMeName[1],Risk='Unknown'))}
}

DiseaseReport=lapply(DiseaseReport,FUN = GetType)
DiseaseReport=Reduce(DiseaseReport,f = rbind)
rownames(DiseaseReport)=NULL
DiseaseReport=as.data.frame(DiseaseReport)

write.csv(DiseaseReport,file = paste0(OutputAddress,'/GeneticPhenotypeReport_','Patient_',PatientID,'.csv'))
print('finished writing CSV')