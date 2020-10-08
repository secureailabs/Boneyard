library(Biobase)
library(GEOquery)

# load series and platform data from GEO
gset <- getGEO("GSE30999", GSEMatrix =TRUE, getGPL=FALSE)
if (length(gset) > 1) idx <- grep("GPL570", attr(gset, "names")) else idx <- 1 
gset <- gset[[idx]]

# set parameters and draw the plot
dev.new(width=4+dim(gset)[[2]]/5, height=6)
par(mar=c(2+round(max(nchar(sampleNames(gset)))/2),4,2,1))
title <- paste ("GSE30999","/", annotation(gset), " selected samples",sup = "")
boxplot(exprs(gset), boxwex=0.7, notch=T, main=title, outline=FALSE, las=2)
r<-as.data.frame.matrix(exprs(gset))

# In each mechanism, sensitivity is set to one. 

Differential_privacy_Laplace_boxplot <- function(e)
  
{
  title1 <- paste ("GSE30999","/", annotation(gset), " selected samples","\n","e-0 Differentially private Laplace Mechanism boxplot",sup = "")
  title <- paste ("GSE30999","/", annotation(gset), " selected samples",sup = "")
  par(mfrow = c(1, 2))
  boxplot(r + rlaplace(1,0,1/e), boxwex=0.7, notch=T, main=title1, outline=FALSE, las=2)
  boxplot(exprs(gset), boxwex=0.7, notch=T, main=title, outline=FALSE, las=2)
  paste(e,"- 0 differentially private Laplace mechanism")
}


Differential_privacy_Gaussian_boxplot <- function(e,delt)
  
{
  c = sqrt(2*ln(1.25001/delt))
  b = c/e
  title1 <- paste ("GSE30999","/", annotation(gset), " selected samples","\n","e-delta Differentially private","\n","Gaussian Mechanism boxplot",sup = "")
  title <- paste ("GSE30999","/", annotation(gset), " selected samples",sup = "")
  
  par(mfrow = c(1, 2))
  
  boxplot(r + rnorm(1,0,b), boxwex=0.7, notch=T, main=title1, outline=FALSE, las=2)
  boxplot(exprs(gset), boxwex=0.7, notch=T, main=title, outline=FALSE, las=2)
  paste(e,"-",delt,"differentially private Gaussian mechanism")
  
}
