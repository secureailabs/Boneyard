#this code: output from Plink LD-clumping => model file in Json format for 

ClumpedSNPs<-read.table(file = './IGAP_stage_1_Out.clumped',header = T)


tail(ClumpedSNPs)
dim(ClumpedSNPs)
#ClumpedSNPs=ClumpedSNPs[!is.na(ClumpedSNPs$CHR),]



GWASsum<-read.table(file = './IGAP_stage_1_reformatted.txt',header = T)
head(GWASsum)
nrow(GWASsum)


ClumpedSNPs$Beta=GWASsum$BETA[match(ClumpedSNPs$SNP,GWASsum$SNP)]
ClumpedSNPs$EA=GWASsum$EA[match(ClumpedSNPs$SNP,GWASsum$SNP)]
ClumpedSNPs$NEA=GWASsum$NEA[match(ClumpedSNPs$SNP,GWASsum$SNP)]
ClumpedSNPs$CHR=GWASsum$Chromosome[match(ClumpedSNPs$SNP,GWASsum$SNP)]
ClumpedSNPs$BP=GWASsum$Position[match(ClumpedSNPs$SNP,GWASsum$SNP)]
ClumpedSNPs$EAF=GWASsum$MAF[match(ClumpedSNPs$SNP,GWASsum$SNP)]
head(ClumpedSNPs)


#Write it into JSOn format 

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



Items=c("SNPs",'Build','Phenotype',"baselineProb")
Output=vector(mode="list",length = length(Items))
names(Output)=Items

Output$Build='hg19'
Output$Phenotype='alzheimers_disease'
Output$baselineProb=0.11


Output$SNPs=lapply(1:nrow(ClumpedSNPs),function(x){list(SNP=as.character(ClumpedSNPs$SNP[x]),
                                            CHR=ClumpedSNPs$CHR[x],
                                            BP=ClumpedSNPs$BP[x],
                                            P=ClumpedSNPs$P[x],
                                            Beta=ClumpedSNPs$Beta[x],
                                            EA=as.character(ClumpedSNPs$EA[x]),
                                            NEA=as.character(ClumpedSNPs$NEA[x]),
                                            EAF=ClumpedSNPs$EAF[x]
                                            )})



Output=toJSON(Output)

write(Output, "alzheimers_disease.json")


