#this code deal with LD for GWAS summary statitstics and convert the file in JSON format 

#convert GWAS summary stats into a standard format readable by Plink 

GWASsum<-read.table(file = '/Users/kaiyuanmifen/Google Drive/GeneTank/Project/Data/GWASSumStatData/AD/IGAP_summary_statistics/IGAP_stage_1.txt',header = T)
head(GWASsum)
nrow(GWASsum)

GWASsum<-GWASsum[,c("Chromosome",'Position',"MarkerName","Effect_allele","Non_Effect_allele","Pvalue","Beta")]
names(GWASsum)=c("Chromosome",'Position',"SNP","EA","NEA","P","BETA")
GWASsum$SNPidentifier=paste(GWASsum$Chromosome,GWASsum$Position,sep = ":")

head(GWASsum)
#add allele frequency (obtained from 1000 genome genotype data)
## In plink output A1 was the 
AllelleFreq=read.table("1000Genome.frq",header = T)
head(AllelleFreq)
dim(AllelleFreq)


GWASsum$EAF=AllelleFreq$MAF[match(GWASsum$SNPidentifier,AllelleFreq$SNP)]
GWASsum$EAF[GWASsum$EA!=as.character(AllelleFreq$A1[match(GWASsum$SNPidentifier,AllelleFreq$SNP)])]=NA

write.table(x = GWASsum,file = "IGAP_stage_1_reformatted.txt",quote = F,row.names = F)
