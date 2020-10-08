x<-1:16
a<-0
b<-0
for (i in x) {
  if (i%%2 ==1) {
    a<-a+i
  } else {
        b<-b+i
  }
}
y<-b-a
y

