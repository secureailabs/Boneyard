#this version changes the output format and make it easiest for C code to process 
#this code convert the VCF file into JOSON format 

#source("http://bioconductor.org/biocLite.R") 
#biocLite("VariantAnnotation") #install the package

#check package 

        if (!require("VariantAnnotation",character.only = TRUE))
        {
                source("http://bioconductor.org/biocLite.R") 
                biocLite("VariantAnnotation") 
                
                if(!require("VariantAnnotation",character.only = TRUE)) stop("Package not found")
        }
library("VariantAnnotation") #load the package

#source("SNPediaRFunction 2.R")
        
        
#Get patient information
# PatientID='010'
# InputAddress='/Users/kaiyuanmifen/Google Drive/GeneTank/Project/Data/FakeOutput/Patient_010/6308.23andme..vcf'
# 
# OutputAddress='/Users/kaiyuanmifen/Google Drive/GeneTank/Project/PolygenicScore/PatientFiles/Patient_010/'
PatientID=as.character(commandArgs(trailingOnly = T)[[1]][1])

InputAddress=as.character(commandArgs(trailingOnly = T)[[2]][1])

OutputAddress=as.character(commandArgs(trailingOnly = T)[[3]][1])



print(paste("Patient ID:", PatientID))
print(paste("InputAddress:", InputAddress))
print(paste("OutputAddress:", OutputAddress))


#read vcf

VCF=InputAddress
Target=readVcf(VCF,"hg19")
print('reading VCF')
#geno(Target)



#Tidy up Genotype 
GT <-geno(Target)$GT
tail(GT)
class(GT)
GT=as.data.frame(GT)
GT$ref=as.vector(ref(Target))
GT$alt=as.vector(unlist(alt(Target)))
##There some SNP with only one allele , assuming the other one is zero

GT$Dosage=unlist(lapply(strsplit(as.character(GT$GENOTYPE),'[/]'),FUN = function(x)(sum(as.integer(x)))))
GetGeno=function(i){
        c(rep(GT$ref[i],2-GT$Dosage[i]),rep(GT$alt[i],GT$Dosage[i]))
}



GT$Genotype2=unlist(lapply(1:nrow(GT),FUN=function(x){paste(GetGeno(x),collapse = ';')}))
GT$Genotype2=paste0('(',GT$Genotype2,")")
GT$Genotype2=paste0(rownames(GT),GT$Genotype2)
tail(GT)

GT$Al1=unlist(lapply(GT$Genotype2,FUN = function(x){strsplit(x,"[\\(]|[\\;]|[\\)]")[[1]][2]}))
GT$Al2=unlist(lapply(GT$Genotype2,FUN = function(x){strsplit(x,"[\\(]|[\\;]|[\\)]")[[1]][3]}))



#Save as JSON 



if (!require("rjson",character.only = TRUE))
{
  install.packages("rjson")
  if(!require("rjson",character.only = TRUE)) stop("Package not found")
}
library("rjson") #load the package


if (!require("RJSONIO",character.only = TRUE))
{
  install.packages("RJSONIO")
  if(!require("RJSONIO",character.only = TRUE)) stop("Package not found")
}
library("RJSONIO") #load the package

Output=vector(mode="list",length = nrow(GT))
names(Output)=rownames(GT)
for (i in 1:length(Output)){
  Output[[i]]=c(GT[i,c("Al1","Al2")])
}
#lapply(Output,function(x){names(x)=c("Al1","Al2");return(x)})

head(Output)
Output=toJSON(Output)


#create directory for output 
dir.create(file.path(paste0(OutputAddress)))

write(Output, paste0(OutputAddress,'Patient',PatientID,'_','Data.json'))

print('Patient JSON generated')
