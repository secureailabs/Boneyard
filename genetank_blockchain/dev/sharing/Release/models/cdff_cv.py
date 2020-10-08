'''
Updated: February 1, 2018
Written by: Jeeheh Oh

Purpose: Trains final logistic regression model on complete training set using the optimal C value.

How to use:
-- a. Complete reference.py
-- b. Run

Saved Outputs:
-- auc_cv: Cross validation AUC, complete for all values of c, shape = len(c_param) x nfolds
-- c: optimal hyperparameter c value
-- clf: final classifier
-- df, dftr: Data frame containing columns for: eid, day, rawest(raw score from logistic regression), label, rollingscore (the rolling average of the raw score) for test and training. Data is at the day level of granularity.
-- df2,dftr2: Data frame containing eid, label and rollingscore for test and training. Data is at the admission level. Maximum rolling score is kept for each admission.
'''

import numpy as np
import pandas as pd
# import reference as reference
import pickle
import sys
from sklearn import linear_model
from sklearn import metrics
from scipy.sparse import csr_matrix

# Fill out desired variable settings
def returnVars():
    #Location where you want output to be saved to
    pathout='/data/'

    #subsample random seed
    randseed= 11

    # C parameter Grid Search Values
    c_param=[5**(-14),5**(-13),5**(-12),5**(-11),5**(-10),5**(-9),5**(-8),5**(-7),5**(-6),5**(-5),5**(-4),5**(-3),5**(-2),5**(-1),5**(0)]

    # Number of folds for the Cross Validation
    nfolds=5

    return pathout,randseed,c_param,nfolds

# Complete Function so that it returns the appropriate data variables
def loadData():
    # m = dimensionality of features. eg: If our observations consist only of Heart Rate and Blood Pressure, m=2.
    # n_train, n_test = number of patient days we have data for in the training and test set. eg: If our training dataset consists of: (Patient A, LOS=3), (Patient B, LOS=2). Then n_train=5.
    # eid = Admission ID/Encounter ID. Each admission should have a unique ID regardless if it is a returning patient.
    # day = Day of observation, in relation to the admission day. eg: 1, 2, 3, 4, etc.

    indptr = np.array([0,3,6,9,12,15,18,21,24,27,30])
    indices = np.array([0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2,0,1,2])
    data = np.array([70, 120, 98,
        74, 110, 100,
        72, 128, 96,
        80, 113, 97,
        76, 126, 98,
        90, 123, 99,
        60, 121, 96,
        68,  98, 95,
        59, 130, 97,
        78, 140, 98])
    xtrain= csr_matrix((data, indices, indptr), shape=(10, 3)) # heart rate, blood pressure, temperature
    ytrain= np.array([1, 0, 1, 1, 0, 0, 0, 0, 0, 0])
    data = np.array([71, 122, 96,
        84, 120, 97,
        77, 138, 100,
        82, 103, 98,
        86, 116, 97,
        79, 133, 96,
        68, 141, 99,
        64,  95, 97,
        69, 134, 98,
        88, 130, 95])
    xtest= csr_matrix((data, indices, indptr), shape=(10, 3)) # heart rate, blood pressure, temperatur
    ytest= np.array([1, 0, 1, 0, 0, 1, 0, 1, 0, 0])
    eid_train= np.array(["1", "2", "3", "4", "5", "6", "7", "8", "9", "10"])
    eid_test= np.array(["11", "12", "13", "14", "15", "16", "17", "18", "19", "20"])
    day_train= np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 2])
    day_test= np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 2])

    return xtrain,ytrain,xtest,ytest,eid_train,eid_test,day_train,day_test


# Returns subsampled version of data, where for eg, if size=3, each patient is only represented 3 times in the dataset, aka only 3 days from that patient are kept in the subsampled dataset.
# Set desired subsample size

def subsample(eid,day,x,y):
    df=pd.DataFrame({'eid':eid,'day':day})
    df['index']=df.index

    size= 3
    replace=True
    subspl=lambda obj: obj.loc[np.random.choice(obj.index,size,replace),:]
    df=df.groupby('eid',as_index=False).apply(subspl)

    x=x[df['index'].tolist(),:]
    y=y[df['index'].tolist()]

    return x,y

pathout,randseed,c_param,nfolds=returnVars()
np.random.seed(randseed)


# Import data
xtrain,ytrain,xtest,ytest,eid_train,eid_test,day_train,day_test=loadData()


# Import data: CV
auc_cv=np.empty((len(c_param),nfolds))
auc_cv[:]=np.NAN

auc_train_ef=[]
auc_test_ef=[]

for i in range(len(c_param)):
    zauc_cv,zauc_train_ef,zauc_test_ef,_,_=pickle.load(open(pathout+"cv_c"+str(i)+"_auc.pickle","rb"))
        #auc_cv,auc_train_ef,auc_test_ef,c,c_param
    auc_cv[i,:]=zauc_cv[i,:]
    auc_train_ef.append(zauc_train_ef)
    auc_test_ef.append(zauc_test_ef)


# Find best c
c=c_param[np.argmax(np.mean(auc_cv,axis=1))]


# Learn Model
clf=linear_model.LogisticRegression(penalty='l2',class_weight='balanced',C=c)
zx,zy=subsample(eid_train,day_train,xtrain,ytrain)
clf.fit(zx,zy)


# Create Output Dataframe
idx=np.where(clf.classes_==1)[0]
test_rawest=clf.predict_proba(xtest)[:,idx]
train_rawest=clf.predict_proba(xtrain)[:,idx]
df=pd.DataFrame({'encounterID':eid_test,'day':day_test     ,'rawest':test_rawest.reshape((len(test_rawest),)),'label':ytest.reshape((len(ytest),))})
dftr=pd.DataFrame({'encounterID':eid_train,'day':day_train     ,'rawest':train_rawest.reshape((len(train_rawest),)),'label':ytrain.reshape((len(ytrain),))})


#Smoothing
df=df.sort_values(['encounterID','day'],ascending=[1,1])
df['csum']=df['rawest'].groupby([df['encounterID']]).cumsum()
df['rollingscore']=df['csum']/df['day']

dftr=dftr.sort_values(['encounterID','day'],ascending=[1,1])
dftr['csum']=dftr['rawest'].groupby([dftr['encounterID']]).cumsum()
dftr['rollingscore']=dftr['csum']/dftr['day']

df2=df.loc[df.day>=2,['rollingscore','label','encounterID']].groupby([df['encounterID']]).max()
dftr2=dftr.loc[dftr.day>=2,['rollingscore','label','encounterID']].groupby([dftr['encounterID']]).max()

# Save Data
with open(pathout+'learn_model.pickle','wb') as f:
    pickle.dump([auc_cv, c, clf, df, df2,dftr,dftr2],f)
