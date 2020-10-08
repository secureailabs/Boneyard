class LogRegCV:

    def __init__(self, penalty_range = [0.1,1,10,100,1000], penalty='l1', lbd = 10, lr=0.01, num_iter=1000, fit_intercept=True, DP = None):

        self.penalty = penalty
        self.penalty_range = penalty_range
        self.lbd = lbd 
        self.lr = lr
        self.num_iter = num_iter
        self.fit_intercept = fit_intercept
        self.DP = DP 
    
    def __add_intercept(self, X): # add intercept 
        intercept = np.ones((X.shape[0], 1))
        return np.concatenate((intercept, X), axis=1)
    
    def __sigmoid(self, z):
        return 1 / (1 + np.exp(-z)) # Maybe add 0.001 

    def RSS(self,X_Test,Y_Test):
        
        if self.fit_intercept: #we make a condition on the input of the class 
            X_Test = self.__add_intercept(X_Test)
    
        predict_prob = self.__sigmoid(np.array(np.dot(self.theta,X_Test.T),dtype = float))
        return(np.mean((predict_prob - np.reshape(Y_Test,(1,len(Y_Test))))**2))

    def RSS_DP1(self,X_Test,Y_Test,eps):
        
        if self.fit_intercept: #we make a condition on the input of the class 
            X_Test = self.__add_intercept(X_Test)

        n = X_Test.shape[0]
        norm_etha = np.random.gamma(len(self.theta), 2/(n*eps*self.lbd),1)
        noise = np.random.laplace(0, 2/(n*eps*self.lbd*abs(norm_etha)),self.theta.shape)
        self.theta = self.theta + noise # adjusting the weight

        predict_prob = self.__sigmoid(np.array(np.dot(self.theta,X.T),dtype = float))
        return(np.mean((predict_prob - np.reshape(Y_Test,(1,len(Y_Test))))**2))

    def fitCV_DP1(self, X, y,X_Test,Y_Test,eps):
        
        RSS = []
        RSS_DP = []

        if self.fit_intercept: #we make a condition on the input of the class IS it possible that we repeat this function two times? 
            X = self.__add_intercept(X)
        # weights initialization 
        for reg in self.penalty_range:
            self.lbd = reg

            self.theta = np.random.normal(0,1,size = X.shape[1])

            if self.penalty == None or self.penalty == 0: 
                
                for i in range(self.num_iter):
                    z = np.array(np.dot(X, self.theta),dtype=np.float32)
                    h = self.__sigmoid(z)
                    h = np.reshape(h,(len(h),1))
                    gradient = np.mean((y - h)*X , axis = 0) 
                    self.theta = self.theta - self.lr * gradient
                RSS.append(self.RSS(X_Test,Y_Test))
                RSS_DP.append(self.RSS_DP1(X_Test,Y_Test))

            if self.penalty == 'l1': 

                for i in range(self.num_iter):
                    grad_l1_theta = np.sign(self.theta)
                    z = np.array(np.dot(X, self.theta),dtype=np.float32)
                    h = self.__sigmoid(z)
                    h = np.reshape(h,(len(h),1))
                    gradient = np.mean((y - h)*X, axis = 0) + grad_l1_theta*self.lbd
                    self.theta = self.theta - self.lr * gradient

                RSS.append(self.RSS(X_Test,Y_Test))
                RSS_DP.append(self.RSS_DP1(X_Test,Y_Test))

            if self.penalty == 'l2': 

                for i in range(self.num_iter):
                    z = np.array(np.dot(X, self.theta),dtype=np.float32)
                    h = self.__sigmoid(z)
                    h = np.reshape(h,(len(h),1))
                    gradient = np.mean((y - h)*X, axis = 0) + 2*(self.lbd*np.asarray(self.theta))
                    self.theta = self.theta - self.lr * gradient

                RSS.append(self.RSS(X_Test,Y_Test))
                RSS_DP.append(self.RSS_DP1(X_Test,Y_Test))
        return RSS, RSS_DP, [self.penalty_range[RSS.index(min(RSS))], min(RSS)], [self.penalty_range[RSS_DP.index(min(RSS_DP))], min(RSS_DP)]
        print ("optimal Regularizer is" + str(self.penalty_range[RSS.index(min(RSS))]))
        print("RSS is " + str(min(RSS)))
        print ("optimal DP_Regularizer is" + str(self.penalty_range[RSS_DP.index(min(RSS_DP))]))
        print("RSS_DP is " + str(min(RSS_DP)))

# NEED TO THINK OF BEING ABLE VARIATE EPSILON !!!!!!!!!!!! 

    def fitCV_DP2(self, X, y,X_Test,Y_Test,eps):

        RSS_DP = []
        if self.fit_intercept: #we make a condition on the input of the class 
            X = self.__add_intercept(X)
        # weights initialization
        
        for reg in self.penalty_range:
            self.lbd = reg

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
                RSS_DP.append(RSS(X_Test,Y_Test))
            
            if self.penalty == 'l1': 

                for i in range(self.num_iter):
                    grad_l1_theta = np.sign(self.theta)
                    z = np.array(np.dot(X, self.theta),dtype=np.float32)
                    h = self.__sigmoid(z)
                    h = np.reshape(h,(len(h),1))
                    gradient = np.mean((y - h)*X, axis = 0) + grad_l1_theta*self.lbd + self.b /y.size
                    self.theta = self.theta - self.lr * gradient
                RSS_DP.append(RSS(X_Test,Y_Test))

            if self.penalty == 'l2': 

                for i in range(self.num_iter):
                    z = np.array(np.dot(X, self.theta),dtype=np.float32)
                    h = self.__sigmoid(z)
                    h = np.reshape(h,(len(h),1))
                    gradient = np.mean((y - h)*X, axis = 0) + 2*(self.lbd*np.asarray(self.theta)) + self.b /y.size
                    self.theta = self.theta - self.lr * gradient
                RSS_DP.append(RSS(X_Test,Y_Test))

        return RSS_DP,[self.penalty_range[RSS_DP.index(min(RSS_DP))], min(RSS_DP)]
        print ("optimal DP_Regularizer is" + str(self.penalty_range[RSS_DP.index(min(RSS_DP))]))
        print("RSS_DP is " + str(min(RSS_DP)))
                        
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
    
    def classifier():
        return (self.theta)