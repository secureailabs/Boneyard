ydat<-scan("/home/jingwei/genetank_blockchain/doc/R/y.txt",
              quiet=TRUE)
xdat<-matrix(scan("/home/jingwei/genetank_blockchain/doc/R/x.txt",
              quiet=TRUE),
         ncol=1024,
         byrow=TRUE)

len=length(ydat)
train_len<-trunc(len*0.8)
mark=sample(1:len,size=train_len)
y_train=ydat[mark]
y_test=ydat[-mark]
x_train=xdat[mark,]
x_test=xdat[-mark,]
#timer of trainning
library(randomForest)
starttime<-Sys.time()
re<-randomForest(x=x_train,y=factor(y_train),xtest=x_test,ytest=factor(y_test),ntree=100)
endtime<-Sys.time()
retime<-endtime-starttime
retime

re

