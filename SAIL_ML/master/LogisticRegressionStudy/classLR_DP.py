## Class logistic regression

import numpy as np 

class LogReg:
    def __init__(self, penalty = 'l1', lbd = 10, lr=0.01, num_iter=1000, fit_intercept=True, DP = None):

        self.penalty = penalty
        self.lbd = lbd 
        self.lr = lr
        self.num_iter = num_iter
        self.fit_intercept = fit_intercept
        self.DP = DP 
    
    def __add_intercept(self, X): # add intercept 
        intercept = np.ones((X.shape[0], 1))
        return np.concatenate((intercept, X), axis=1)
    
    def __sigmoid(self, z):
        return 1 / (1 + np.exp(-z)) # Maybe add 0.001 if 
    
    def __loss(self, h, y):

        if self.DP == 'DP1' or self.DP == None:

            if self.penalty == None or self.penalty == 0:
                self.theta = np.asarray(self.theta)
                h = np.reshape(h,(len(h),1))
                return(np.mean(-y*h - (1 - y)*(np.log(1 - h)),axis = 0))

            if self.penalty == 'l1':
                self.theta = np.asarray(self.theta)
                h = np.reshape(h,(len(h),1))
                return np.mean(-y*h - (1 - y)*(np.log(1 - h)),axis = 0) + self.lbd*np.linalg.norm(self.theta, ord = 1)

            if self.penalty == 'l2': 
                self.theta = np.asarray(self.theta)
                h = np.reshape(h,(len(h),1))
                return np.mean(-y*h - (1 - y)*(np.log(1 - h)),axis = 0) + self.lbd*np.linalg.norm(self.theta, ord = 2)

        if self.DP == 'DP2':

            if self.penalty == False or self.penalty == 0:
                self.theta = np.asarray(self.theta)
                h = np.reshape(h,(len(h),1))
                return np.mean(-y*h - (1 - y)*(np.log(1 - h)),axis = 0) + (self.b.T.dot(self.theta))/y.size

            if self.penalty == 'l1':
                self.theta = np.asarray(self.theta)
                h = np.reshape(h,(len(h),1))
                return np.mean(-y*h - (1 - y)*(np.log(1 - h)),axis = 0) + self.lbd*np.linalg.norm(self.theta, ord = 1)\
                + (self.b.T.dot(self.theta))/y.size

            if self.penalty == 'l2': 
                self.theta = np.asarray(self.theta)
                h = np.reshape(h,(len(h),1))
                return np.mean(-y*h - (1 - y)*(np.log(1 - h)),axis = 0)  + self.lbd*np.linalg.norm(self.theta, ord = 2)\
                +(self.b.T.dot(self.theta))/y.size

    
    def fit(self, X, y):
        
        if self.fit_intercept: #we make a condition on the input of the class 
            X = self.__add_intercept(X)
        # weights initialization
        self.theta = np.random.normal(0,1,size = X.shape[1])

        if self.penalty == None or self.penalty == 0: 
            
            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X , axis = 0) 
                self.theta = self.theta - self.lr * gradient
            self.theta_NonDP = self.theta

        if self.penalty == 'l1': 

            for i in range(self.num_iter):
                grad_l1_theta = np.sign(self.theta)
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + grad_l1_theta*self.lbd
                self.theta = self.theta - self.lr * gradient
            self.theta_NonDP = self.theta 

        if self.penalty == 'l2': 

            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + 2*(self.lbd*np.asarray(self.theta))
                self.theta = self.theta - self.lr * gradient
            self.theta_NonDP = self.theta

    def fit_LossStudy(self, X, y,X_Test,Y_Test):
        X_No_intercept = X 
        if self.fit_intercept: #we make a condition on the input of the class 
            X = self.__add_intercept(X)
        # weights initialization
        self.theta = np.random.normal(0,1,size = X.shape[1])
        L_Test = []
        L_Train = []

        if self.penalty == None or self.penalty == 0: 
            
            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X , axis = 0) 
                self.theta = self.theta - self.lr * gradient
                a,b = self.loss(X_No_intercept, y, X_Test, Y_Test)
                L_Train.append(a)
                L_Test.append(b)
            

        if self.penalty == 'l1': 

            for i in range(self.num_iter):
                grad_l1_theta = np.sign(self.theta)
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + grad_l1_theta*self.lbd
                self.theta = self.theta - self.lr * gradient
                a,b = self.loss(X_No_intercept, y, X_Test, Y_Test)
                L_Train.append(a)
                L_Test.append(b)
            
        if self.penalty == 'l2': 

            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + 2*(self.lbd*np.asarray(self.theta))
                self.theta = self.theta - self.lr * gradient
                a,b = self.loss(X_No_intercept, y, X_Test, Y_Test)
                L_Train.append(a)
                L_Test.append(b)

        return (L_Train,L_Test)


    ## For differential privacy 1, according to the paper, we only add some noise to the classifier. 
    def fit_DP_Alg1(self,X, y,eps): 

        n = X.shape[0]
        norm_etha = np.random.gamma(len(self.theta), 2/(n*eps*self.lbd),1)
        noise = np.random.laplace(0, 2/(n*eps*self.lbd*abs(norm_etha)),self.theta.shape)
        self.theta = self.theta_NonDP + noise
     
    ## For differential privacy 2 according to the paper, we add noise to the cross entropy function.   
    def fit_DP_Alg2(self, X, y,eps):
        
        if self.fit_intercept: #we make a condition on the input of the class 
            X = self.__add_intercept(X)
        # weights initialization
        self.theta = np.random.normal(0,1,size = X.shape[1])
        
        n = X.shape[0]
        norm_b = np.random.gamma(11, 2/(eps),1)
        self.b = np.random.laplace(0, 2/(eps*abs(norm_b)),self.theta.shape)
        
        if self.penalty == None or self.penalty == 0: 
            
            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + self.b /y.size ## The last term of the updated gradient represents the differential privacy factor
                self.theta = self.theta - self.lr * gradient
        
        if self.penalty == 'l1': 

            for i in range(self.num_iter):
                grad_l1_theta = np.sign(self.theta)
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + grad_l1_theta*self.lbd + self.b /y.size
                self.theta = self.theta - self.lr * gradient

        if self.penalty == 'l2': 

            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + 2*(self.lbd*np.asarray(self.theta)) + self.b /y.size
                self.theta = self.theta - self.lr * gradient

    def fit_LossStudyDP2(self, X, y,X_Test,Y_Test,eps):
        X_No_intercept = X 
        L_Train = []
        L_Test = []

        if self.fit_intercept: #we make a condition on the input of the class 
            X = self.__add_intercept(X)
        # weights initialization
        self.theta = np.random.normal(0,1,size = X.shape[1])
        
        n = X.shape[0]
        norm_b = np.random.gamma(11, 2/(eps),1)
        self.b = np.random.laplace(0, 2/(eps*abs(norm_b)),self.theta.shape)
        
        if self.penalty == None or self.penalty == 0: 
            
            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + self.b /y.size
                self.theta = self.theta - self.lr * gradient
                a,b = self.loss(X_No_intercept, y, X_Test, Y_Test)
                L_Train.append(a)
                L_Test.append(b)
        
        if self.penalty == 'l1': 

            for i in range(self.num_iter):
                grad_l1_theta = np.sign(self.theta)
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + grad_l1_theta*self.lbd + self.b /y.size
                self.theta = self.theta - self.lr * gradient
                a,b = self.loss(X_No_intercept, y, X_Test, Y_Test)
                L_Train.append(a)
                L_Test.append(b)

        if self.penalty == 'l2': 

            for i in range(self.num_iter):
                z = np.array(np.dot(X, self.theta),dtype=np.float32)
                h = self.__sigmoid(z)
                h = np.reshape(h,(len(h),1))
                gradient = np.mean((y - h)*X, axis = 0) + 2*(self.lbd*np.asarray(self.theta)) + self.b /y.size
                self.theta = self.theta - self.lr * gradient
                a,b = self.loss(X_No_intercept, y, X_Test, Y_Test)
                L_Train.append(a)
                L_Test.append(b)

                                 
    def predict_prob(self, X):
        return self.__sigmoid(np.array(np.dot(X,self.theta[1:]) + self.theta[0] ,dtype = float)) 
    
    def predict(self, X, threshold):
        Y = []
        for i in X:
        
            if self.predict_prob(i) >= threshold: 
                Y.append(1)
            else: 
                Y.append(0)
        return np.asarray(Y)

    ## F1 score for unbalances data. 
    def F1_score(self,X_Test,Y_Test):
        
        Y_Test = np.reshape(Y_Test,(1,len(Y_Test)))[0]
        Predict = self.predict(X_Test,0.5)

        P = np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) / (np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) +\
                                                     np.sum(Predict[Y_Test == 0])) # False positive
        R = np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) / (np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) +\
                                                     np.sum(Y_Test[Predict==0])) # False negative
    
        return((2*P*R)/(P+R))
    
    def Score(self,X_Test,Y_Test): 
        Predict = self.predict(X_Test,0.5)
        Y_Test = np.reshape(Y_Test,(1,len(Y_Test)))[0]
        return(np.sum(Predict == Y_Test) / len(Y_Test))

    def loss(self,X_Train, Y_Train, X_Test, Y_Test):
    
        if self.fit_intercept:
            X_Train = self.__add_intercept(X_Train)
            X_Test = self.__add_intercept(X_Test)
        
        z_train = np.array(np.dot(X_Train, self.theta),dtype=np.float32)
        h_train = self.__sigmoid(z_train)
        
        z_test = np.array(np.dot(X_Test, self.theta),dtype=np.float32)
        h_test = self.__sigmoid(z_test)

        a = self.__loss(h_train,Y_Train) 
        b = self.__loss(h_test,Y_Test) 
        
        return(a,b)
    
    def classifier():
        return (self.theta)