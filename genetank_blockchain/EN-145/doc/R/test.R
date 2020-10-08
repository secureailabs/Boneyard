  x <- 2\n\
  y <- 3\n\
  x <- x + y\n\
  y <- x * y\n\
  a <- x-y\n\
  b <- x/y\n\
  a\n\
  b\n\
\n\
\n\
  # ---Data Structures---\n\
\n\
  # Vectors\n\
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)\n\
  workshop\n\
\n\
  gender <- c("f", "f", "f", NA, "m", "m", "m", "m")\n\
  q1 <- c(1, 2, 2, 3, 4, 5, 5, 4)\n\
  q2 <- c(1, 1, 2, 1, 5, 4, 3, 5)\n\
  q3 <- c(5, 4, 4,NA, 2, 5, 4, 5)\n\
  q4 <- c(1, 1, 3, 3, 4, 5, 4, 5)\n\
\n\
  # Selecting Elements of Vectors\n\
  q1[5]\n\
  q1[ c(5, 6, 7, 8) ]\n\
  q1[5:8]\n\
  q1[gender == "m"]\n\
  mean( q1[ gender == "m" ], na.rm = TRUE)\n\
\n\
  # ---Factors---\n\
\n\
  # Numeric Factors\n\
\n\
  # First, as a vector\n\
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)\n\
  workshop\n\
  table(workshop)\n\
  mean(workshop)\n\
  gender[workshop == 2]\n\
\n\
  # Now as a factor\n\
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)\n\
  workshop <- factor(workshop)\n\
  workshop\n\
  table(workshop)\n\
  #mean(workshop) #generates error now.\n\
  gender[workshop == 2]\n\
  gender[workshop == "2"]\n\
\n\
  # Recreate workshop, making it a factor\n\
  # including levels that don't yet exist.\n\
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)\n\
  workshop <- factor(\n\
    workshop,\n\
    levels = c( 1,   2,     3,      4),\n\
    labels = c("R", "SAS", "SPSS", "Stata")\n\
  )\n\
\n\
  # Recreate it with just the levels it\n\
  # curently has.\n\
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)\n\
  workshop <- factor(\n\
    workshop,\n\
    levels = c( 1,  2),\n\
    labels = c("R","SAS")\n\
  )\n\
\n\
  workshop\n\
  table(workshop)\n\
  gender[workshop == 2]\n\
  gender[workshop == "2"]\n\
  gender[workshop == "SAS"]\n\
\n\
  # Character factors\n\
\n\
  gender <- c("f", "f", "f", NA, "m", "m", "m", "m")\n\
  gender <- factor(\n\
    gender,\n\
    levels = c("m",    "f"),\n\
    labels = c("Male", "Female")\n\
  )\n\
\n\
  gender\n\
  table(gender)\n\
  workshop[gender == "m"]\n\
  workshop[gender == "Male"]\n\
\n\
  # Recreate gender and make it a factor,\n\
  # keeping simpler m and f as labels.\n\
  gender <- c("f", "f", "f", NA, "m", "m", "m", "m")\n\
  gender <- factor(gender)\n\
  gender\n\
\n\
  # Data Frames\n\
  mydata <- data.frame(workshop, gender, q1, q2, q3, q4)\n\
  mydata\n\
\n\
  names(mydata)\n\
  row.names(mydata)\n\
\n\
  # Selecting components by index number\n\
  mydata[8, 6] #8th obs, 6th var\n\
  mydata[ , 6] #All obs, 6th var\n\
  mydata[ , 6][5:8] #6th var, obs 5:8\n\
\n\
  # Selecting components by name\n\
  mydata$q1\n\
  mydata$q1[5:8]\n\
\n\
  # Example renaming gender to sex while\n\
  # creating a data frame (left as a comment)\n\
  \n\
  mydata <- data.frame(workshop, sex = gender,\n\
     q1, q2, q3, q4)\n\
\n\
  # Matrices\n\
\n\
  # Creating from vectors\n\
  mymatrix <- cbind(q1, q2, q3, q4)\n\
  mymatrix\n\
  dim(mymatrix)\n\
\n\
  # Selecting Subsets of Matrices\n\
\n\
  mymatrix[8, 4]\n\
  mymatrix[5:8, 3:4]\n\
  mymatrix[ ,4][1:4]\n\
  mymatrix[ ,"q4"]\n\
\n\
  # Creating from matrix function\n\
  # left as a comment so we keep\n\
  # version with names q1, q2...\n\
  \n\
  mymatrix <- matrix(\n\
     c(1, 1, 5, 1,\n\
       2, 1, 4, 1,\n\
       2, 2, 4, 3,\n\
       3, 1, NA,3,\n\
       4, 5, 2, 4,\n\
       5, 4, 5, 5,\n\
       5, 3, 4, 4,\n\
       4, 5, 5, 5),\n\
    nrow = 8, ncol = 4, byrow = TRUE)\n\
  mymatrix\n\
\n\
  table(mymatrix)\n\
  mean(mymatrix, na.rm = TRUE)\n\
\n\
  # Matrix Algebra\n\
\n\
  mymatrixT <- t(mymatrix)\n\
  mymatrixT\n\
\n\
  # Lists\n\
  mylist <- list(workshop, gender,\n\
  q1, q2, q3, q4, mymatrix)\n\
  mylist\n\
\n\
  # List, this time adding names\n\
  mylist <- list(\n\
    workshop = workshop,\n\
    gender   = gender,\n\
    q1 = q1,\n\
    q2 = q2,\n\
    q3 = q3,\n\
    q4 = q4,\n\
    mymatrix = mymatrix\n\
  )\n\
  mylist\n\
\n\
  # Selecting components by index number.\n\
  mylist[[2]]\n\
  mylist[[2]][5:8]\n\
\n\
  mylist[2]\n\
\n\
  # Selecting components by name.\n\
  mylist$q1\n\
  mylist$mymatrix[5:8, 3:4]\n\
\n\
  # ---Controlling Functions---\n\
\n\
  # Controlling Functions with Arguments\n\
\n\
  mean(x = q3, trim = .25, na.rm = TRUE)\n\
  mean(na.rm = TRUE, x = q3, trim = .25)\n\
  mean(q3, .25, TRUE)\n\
  mean(q3, t = .25, na.rm = TRUE)\n\
  #mean(1, 2, 3)\n\
  mean( c(1, 2, 3) )\n\
  mean( 1:3 )\n\
\n\
  # ---Writing Your Own Functions (Macros)---\n\
\n\
  myvar <- c(1, 2, 3, 4, 5)\n\
\n\
  # A function with vector output.\n\
  mystats  <- function(x) {\n\
  mymean <- mean(x, na.rm = TRUE)\n\
  c(input = x, mean = mymean)\n\
  }\n\
  mystats(myvar)\n\
  myVector <- mystats(myvar)\n\
  myVector\n\
\n\
  # A function with list output.\n\
  mystats   <- function(x) {\n\
  myinput <- x\n\
  mymean  <- mean(x, na.rm = TRUE)\n\
  list(data = myinput, mean = mymean)\n\
  }\n\
  mystats(myvar)\n\
  myStatlist <- mystats(myvar)\n\
  myStatlist\n\
  mystats\n

x<-1
y<-2
z<-x+y
a<-x*y
b<-x/y
a
b

function(){
  # ---Simple Calculations---
  2 + 3
   
  x <- 2
  y <- 3
  x + y
  x * y
  a<-x-y
  b<-x/y
   
  # ---Data Structures---
   
  # Vectors
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)
  #print(workshop)
  workshop
   
  gender <- c("f", "f", "f", NA, "m", "m", "m", "m")
  q1 <- c(1, 2, 2, 3, 4, 5, 5, 4)
  q2 <- c(1, 1, 2, 1, 5, 4, 3, 5)
  q3 <- c(5, 4, 4,NA, 2, 5, 4, 5)
  q4 <- c(1, 1, 3, 3, 4, 5, 4, 5)
   
  # Selecting Elements of Vectors
  q1[5]
  q1[ c(5, 6, 7, 8) ]
  q1[5:8]
  q1[gender == "m"]
  mean( q1[ gender == "m" ], na.rm = TRUE)
   
  # ---Factors---
   
  # Numeric Factors
   
  # First, as a vector
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)
  workshop
  table(workshop)
  mean(workshop)
  gender[workshop == 2]
   
  # Now as a factor
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)
  workshop <- factor(workshop)
  workshop
  table(workshop)
  #mean(workshop) #generates error now.
  gender[workshop == 2]
  gender[workshop == "2"]
   
  # Recreate workshop, making it a factor
  # including levels that don't yet exist.
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)
  workshop <- factor(
    workshop,
    levels = c( 1,   2,     3,      4),
    labels = c("R", "SAS", "SPSS", "Stata")
  )
   
  # Recreate it with just the levels it
  # curently has.
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)
  workshop <- factor(
    workshop,
    levels = c( 1,  2),
    labels = c("R","SAS")
  )
   
  workshop
  table(workshop)
  gender[workshop == 2]
  gender[workshop == "2"]
  gender[workshop == "SAS"]
   
  # Character factors
   
  gender <- c("f", "f", "f", NA, "m", "m", "m", "m")
  gender <- factor(
  gender,
  levels = c("m",    "f"),
  labels = c("Male", "Female")
  )
   
  gender
  table(gender)
  workshop[gender == "m"]
  workshop[gender == "Male"]
   
  # Recreate gender and make it a factor,
  # keeping simpler m and f as labels.
  gender <- c("f", "f", "f", NA, "m", "m", "m", "m")
  gender <- factor(gender)
  gender
   
  # Data Frames
  mydata <- data.frame(workshop, gender, q1, q2, q3, q4)
  mydata
   
  names(mydata)
  row.names(mydata)
   
  # Selecting components by index number
  mydata[8, 6] #8th obs, 6th var
  mydata[ , 6] #All obs, 6th var
  mydata[ , 6][5:8] #6th var, obs 5:8
   
  # Selecting components by name
  mydata$q1
  mydata$q1[5:8]
   
  # Example renaming gender to sex while
  # creating a data frame (left as a comment)
  #
  # mydata <- data.frame(workshop, sex = gender,
  #   q1, q2, q3, q4)
   
  # Matrices
   
  # Creating from vectors
  mymatrix <- cbind(q1, q2, q3, q4)
  mymatrix
  dim(mymatrix)
   
  # Creating from matrix function
  # left as a comment so we keep
  # version with names q1, q2...
  #
  # mymatrix <- matrix(
  #   c(1, 1, 5, 1,
  #     2, 1, 4, 1,
  #     2, 2, 4, 3,
  #     3, 1, NA,3,
  #     4, 5, 2, 4,
  #     5, 4, 5, 5,
  #     5, 3, 4, 4,
  #     4, 5, 5, 5),
  #  nrow = 8, ncol = 4, byrow = TRUE)
  # mymatrix
   
  table(mymatrix)
  mean(mymatrix, na.rm = TRUE)
  #cor(mymatrix, use = "pairwise")
   
  # Selecting Subsets of Matrices
   
  mymatrix[8, 4]
  mymatrix[5:8, 3:4]
  mymatrix[ ,4][1:4]
  #mymatrix$q4  # No good!
  mymatrix[ ,"q4"]
   
  # Matrix Algebra
   
  mymatrixT <- t(mymatrix)
  mymatrixT
   
  # Lists
  mylist <- list(workshop, gender,
  q1, q2, q3, q4, mymatrix)
  mylist
   
  # List, this time adding names
  mylist <- list(
    workshop = workshop,
    gender   = gender,
    q1 = q1,
    q2 = q2,
    q3 = q3,
    q4 = q4,
    mymatrix = mymatrix
  )
  mylist
   
  # Selecting components by index number.
  mylist[[2]]
  mylist[[2]][5:8]
   
  mylist[2]
  #mylist[2][5:8]  # Bad!
   
  # Selecting components by name.
  mylist$q1
  mylist$mymatrix[5:8, 3:4]
   
  # ---Saving Your Work---
   
  #ls()
  #objects() #same as ls()
   
  #save.image("myall.RData")
  #save(mydata, file = "mydata.RData")
   
  # The 2nd approach is commented to keep
  # the q variables for following examples.
  # rm(x, y, workshop, gender, q1, q2, q3, q4, mylist)
  # ls()
  # save.image(file = "mydata.RData")
   
  # ---Comments to Document Your Programs---
   
  # This comment is on its own line, between functions.
   
  workshop <- c(1, 2, 1, 2, #This comment is within the arguments.
  1, 2, 1, 2)  # And this is at the end.
   
  # ---Comments to Document Your Objects---
   
  comment(mydata) <- "Example data from R for SAS and SPSS Users"
  comment(mydata)
   
  # ---Controlling Functions---
   
  # Controlling Functions with Arguments
   
  #help("mean")
  mean(x = q3, trim = .25, na.rm = TRUE)
  mean(na.rm = TRUE, x = q3, trim = .25)
  mean(q3, .25, TRUE)
  mean(q3, t = .25, na.rm = TRUE)
  #mean(1, 2, 3)
  mean( c(1, 2, 3) )
  mean( 1:3 )
   
  # Controlling Functions With Formulas
   
  lm( q4 ~ q1 + q2 + q3, data = mydata )
   
  t.test(q1 ~ gender, data = mydata)
   
  #t.test( q1[ which(gender == "Female") ],
  #q1[ which(gender == "Male")   ],
  #data = mydata)  # Data ignored!
   
  # Controlling Functions with Extractor Functions
   
  lm( q4 ~ q1 + q2 + q3, data = mydata )
   
  myModel <- lm( q4 ~ q1 + q2 + q3, data = mydata )
  class(myModel)
  summary(myModel)
   
  # How Much Output Is There?
   
  #print(mymodel)
   
  mode(myModel)
  class(myModel)
  names(myModel)
  #print( unclass(myModel) )
   
  myModel$coefficients
  class( myModel$coefficients )
  #barplot( myModel$coefficients )
   
  # ---Writing Your Own Functions (Macros)---
   
  myvar <- c(1, 2, 3, 4, 5)
   
  # A bad function.
  #mystats <- function(x) {
  #mean(x, na.rm = TRUE)
  #sd(x, na.rm = TRUE)
  #}
   
  #mystats(myvar)
   
  # A good function that just prints.
  #mystats <- function(x) {
  #print( mean(x, na.rm = TRUE) )
  #print(   sd(x, na.rm = TRUE) )
  #}
  #mystats(myvar)
   
  # A function with vector output.
  mystats  <- function(x) {
  mymean <- mean(x, na.rm = TRUE)
  mysd   <-   sd(x, na.rm = TRUE)
  c(mean = mymean, sd = mysd )
  }
  mystats(myvar)
  myVector <- mystats(myvar)
  myVector
   
  # A function with list output.
  mystats   <- function(x) {
  myinput <- x
  mymean  <- mean(x, na.rm = TRUE)
  mysd    <-   sd(x, na.rm = TRUE)
  list(data = myinput, mean = mymean, sd = mysd)
  }
  mystats(myvar)
  myStatlist <- mystats(myvar)
  myStatlist
  mystats
   
  #save(mydata, mymatrix, mylist, mystats,file = "myWorkspace.RData")
  #ls()
  return("OK")
}
