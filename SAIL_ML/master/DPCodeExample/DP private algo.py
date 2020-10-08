{
 "cells": [
  {
   "cell_type": "code",
   "execution_count": 1,
   "metadata": {},
   "outputs": [],
   "source": [
    "import os\n",
    "import numpy as np\n",
    "import pandas as pd\n",
    "import matplotlib.pyplot as plt\n",
    "import tensorflow as ts"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 2,
   "metadata": {},
   "outputs": [],
   "source": [
    "os.chdir(\"/Users/MauriceLubetzki/Documents/Etats-Unis/SAIL/HaoDeng/13063_2006_152_MOESM1_ESM\")"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 3,
   "metadata": {},
   "outputs": [
    {
     "data": {
      "text/html": [
       "<div>\n",
       "<style scoped>\n",
       "    .dataframe tbody tr th:only-of-type {\n",
       "        vertical-align: middle;\n",
       "    }\n",
       "\n",
       "    .dataframe tbody tr th {\n",
       "        vertical-align: top;\n",
       "    }\n",
       "\n",
       "    .dataframe thead th {\n",
       "        text-align: right;\n",
       "    }\n",
       "</style>\n",
       "<table border=\"1\" class=\"dataframe\">\n",
       "  <thead>\n",
       "    <tr style=\"text-align: right;\">\n",
       "      <th></th>\n",
       "      <th>id</th>\n",
       "      <th>age</th>\n",
       "      <th>sex</th>\n",
       "      <th>migraine</th>\n",
       "      <th>chronicity</th>\n",
       "      <th>acupuncturist</th>\n",
       "      <th>date_randomized</th>\n",
       "      <th>practice_id</th>\n",
       "      <th>group</th>\n",
       "      <th>pk1</th>\n",
       "      <th>...</th>\n",
       "      <th>p5cacu</th>\n",
       "      <th>p5all</th>\n",
       "      <th>p5dos</th>\n",
       "      <th>totalacu</th>\n",
       "      <th>totalother</th>\n",
       "      <th>totalgp</th>\n",
       "      <th>totalspec</th>\n",
       "      <th>totaldos</th>\n",
       "      <th>exposure</th>\n",
       "      <th>exposuredos</th>\n",
       "    </tr>\n",
       "  </thead>\n",
       "  <tbody>\n",
       "    <tr>\n",
       "      <th>0</th>\n",
       "      <td>100</td>\n",
       "      <td>47</td>\n",
       "      <td>1</td>\n",
       "      <td>1</td>\n",
       "      <td>35</td>\n",
       "      <td>12</td>\n",
       "      <td>24/03/2000</td>\n",
       "      <td>35</td>\n",
       "      <td>1</td>\n",
       "      <td>10.75</td>\n",
       "      <td>...</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "    </tr>\n",
       "    <tr>\n",
       "      <th>1</th>\n",
       "      <td>101</td>\n",
       "      <td>52</td>\n",
       "      <td>1</td>\n",
       "      <td>1</td>\n",
       "      <td>8</td>\n",
       "      <td>12</td>\n",
       "      <td>24/03/2000</td>\n",
       "      <td>35</td>\n",
       "      <td>0</td>\n",
       "      <td>9.50</td>\n",
       "      <td>...</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>0.0</td>\n",
       "      <td>2.0</td>\n",
       "      <td>4.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>6.0</td>\n",
       "      <td>3.0</td>\n",
       "      <td>3.0</td>\n",
       "    </tr>\n",
       "    <tr>\n",
       "      <th>2</th>\n",
       "      <td>104</td>\n",
       "      <td>32</td>\n",
       "      <td>1</td>\n",
       "      <td>1</td>\n",
       "      <td>14</td>\n",
       "      <td>12</td>\n",
       "      <td>13/04/2000</td>\n",
       "      <td>35</td>\n",
       "      <td>0</td>\n",
       "      <td>16.00</td>\n",
       "      <td>...</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>3.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>3.0</td>\n",
       "      <td>3.0</td>\n",
       "      <td>3.0</td>\n",
       "    </tr>\n",
       "    <tr>\n",
       "      <th>3</th>\n",
       "      <td>105</td>\n",
       "      <td>53</td>\n",
       "      <td>1</td>\n",
       "      <td>1</td>\n",
       "      <td>10</td>\n",
       "      <td>9</td>\n",
       "      <td>5/4/00</td>\n",
       "      <td>25</td>\n",
       "      <td>0</td>\n",
       "      <td>32.50</td>\n",
       "      <td>...</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>NaN</td>\n",
       "      <td>0.0</td>\n",
       "      <td>2.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>NaN</td>\n",
       "      <td>3.0</td>\n",
       "      <td>NaN</td>\n",
       "    </tr>\n",
       "    <tr>\n",
       "      <th>4</th>\n",
       "      <td>108</td>\n",
       "      <td>56</td>\n",
       "      <td>1</td>\n",
       "      <td>1</td>\n",
       "      <td>40</td>\n",
       "      <td>9</td>\n",
       "      <td>24/03/2000</td>\n",
       "      <td>25</td>\n",
       "      <td>0</td>\n",
       "      <td>16.50</td>\n",
       "      <td>...</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>4.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>0.0</td>\n",
       "      <td>23.0</td>\n",
       "      <td>12.0</td>\n",
       "      <td>12.0</td>\n",
       "    </tr>\n",
       "  </tbody>\n",
       "</table>\n",
       "<p>5 rows × 94 columns</p>\n",
       "</div>"
      ],
      "text/plain": [
       "    id  age  sex  migraine  chronicity  acupuncturist date_randomized  \\\n",
       "0  100   47    1         1          35             12      24/03/2000   \n",
       "1  101   52    1         1           8             12      24/03/2000   \n",
       "2  104   32    1         1          14             12      13/04/2000   \n",
       "3  105   53    1         1          10              9          5/4/00   \n",
       "4  108   56    1         1          40              9      24/03/2000   \n",
       "\n",
       "   practice_id  group    pk1  ...  p5cacu  p5all  p5dos  totalacu  totalother  \\\n",
       "0           35      1  10.75  ...     NaN    NaN    NaN       NaN         NaN   \n",
       "1           35      0   9.50  ...     NaN    NaN    NaN       0.0         2.0   \n",
       "2           35      0  16.00  ...     0.0    0.0    3.0       0.0         0.0   \n",
       "3           25      0  32.50  ...     NaN    NaN    NaN       0.0         2.0   \n",
       "4           25      0  16.50  ...     0.0    0.0    4.0       0.0         0.0   \n",
       "\n",
       "   totalgp  totalspec  totaldos  exposure  exposuredos  \n",
       "0      NaN        NaN       NaN       NaN          NaN  \n",
       "1      4.0        0.0       6.0       3.0          3.0  \n",
       "2      0.0        0.0       3.0       3.0          3.0  \n",
       "3      0.0        0.0       NaN       3.0          NaN  \n",
       "4      0.0        0.0      23.0      12.0         12.0  \n",
       "\n",
       "[5 rows x 94 columns]"
      ]
     },
     "execution_count": 3,
     "metadata": {},
     "output_type": "execute_result"
    }
   ],
   "source": [
    "df = pd.read_csv(\"Data-Table.csv\")\n",
    "df.head()"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 4,
   "metadata": {},
   "outputs": [],
   "source": [
    "def Sub_gradient(xi, yi,C,w,b): \n",
    "    if float(1- yi*(np.dot(xi.T, w)+b)) >0:\n",
    "        comp_w =  w + C *(-yi * xi)\n",
    "        comp_b =  C * (-yi)\n",
    "    else:\n",
    "        comp_w = w  \n",
    "        comp_b = 0  \n",
    "    return [comp_w,comp_b] "
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 5,
   "metadata": {},
   "outputs": [],
   "source": [
    "def data_loader():\n",
    "    Data=pd.read_csv(\"Data-Table.csv\")\n",
    "    Data =np.asarray(Data)\n",
    "    Y=Data[:,3]\n",
    "    X=Data[:,9:20]\n",
    "    X =np.asarray(X,dtype=np.float64)\n",
    "    X =np.nan_to_num(X)#Nan to Zeros\n",
    "\n",
    "    X_Train=X[0:int(0.7*X.shape[0]),:]\n",
    "    Y_Train = Y[0:int(0.7 * Y.shape[0])]\n",
    "\n",
    "    X_Test = X[int(0.7 * X.shape[0]):X.shape[0], :]\n",
    "    Y_Test = Y[int(0.7 * Y.shape[0]):X.shape[0]]\n",
    "\n",
    "    return(X_Train,Y_Train,X_Test,Y_Test)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 6,
   "metadata": {},
   "outputs": [],
   "source": [
    "def get_DP_SVM(x_train,y_train,epsilon,C,L,coef_init=None,intercept_init=None,epochs = 100,learning_rate = 0.01, delta = 10**-3):\n",
    "  # Initialize weights, epochs, learning rate\n",
    "    if coef_init is None:\n",
    "        w = np.random.uniform(size=(x_train.shape[1],))\n",
    "        b= np.random.uniform(size=(1,))\n",
    "    else:\n",
    "        w=coef_init\n",
    "        b=intercept_init\n",
    "\n",
    "    epochs = epochs \n",
    "\n",
    "    y_train=np.asarray(y_train)\n",
    "    y_train[y_train==0]=-1#SVM do 0 and 1\n",
    "    y_train[y_train> 1]=-1\n",
    "    #DP gradient descent \n",
    "    \n",
    "    for epoch in range(0, epochs):\n",
    "        randomize = np.arange(len(x_train))\n",
    "        np.random.shuffle(randomize)\n",
    "        randomize = randomize[0:L]\n",
    "        x = x_train[randomize]\n",
    "        y = y_train[randomize]\n",
    "        Grad = [[],[]]\n",
    "        Noise_w = np.random.normal(0,np.sqrt((2*np.log(1.25/delta)/epsilon**2)),11)\n",
    "        Noise_b = np.random.normal(0,np.sqrt((2*np.log(1.25/delta)/epsilon**2)),1)\n",
    "        \n",
    "        for xi, yi in zip(x, y):\n",
    "            Grad[0].append([Sub_gradient(xi, yi,C,w,b)[0]])\n",
    "            Grad[1].append([Sub_gradient(xi, yi,C,w,b)[1]])\n",
    "           \n",
    "        Grad_w = (np.sum(Grad[0],axis = 0 ) + Noise_w)/len(x)\n",
    "        Grad_b = (np.sum(Grad[1],axis = 0 ) + Noise_b)/len(x)\n",
    "        \n",
    "        w = w - (learning_rate)*Grad_w\n",
    "        w = w[0]\n",
    "        b = b - (learning_rate)*Grad_b \n",
    "        b = b[0]\n",
    "        \n",
    "        \n",
    "    return w,b"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 7,
   "metadata": {},
   "outputs": [],
   "source": [
    "def get_SVM(x_train,y_train,C,coef_init=None,intercept_init=None,epochs = 100,learning_rate = 0.01):\n",
    "  # Initialize weights, epochs, learning rate\n",
    "    if coef_init is None:\n",
    "        w = np.random.uniform(size=(x_train.shape[1],))\n",
    "        b= np.random.uniform(size=(1,))\n",
    "    else:\n",
    "        w=coef_init\n",
    "        b=intercept_init\n",
    "\n",
    "    epochs = epochs \n",
    "\n",
    "    y_train=np.asarray(y_train)\n",
    "    y_train[y_train==0]=-1#SVM do 0 and 1\n",
    "    y_train[y_train> 1]=-1 # Maybe some two in the migrain data set, unsure data 2 \n",
    "\n",
    "    # Stochastic Gradient Descent with hinge loss\n",
    "    for epoch in range(0, epochs):\n",
    "        randomize = np.arange(len(x_train))\n",
    "        np.random.shuffle(randomize)\n",
    "        x = x_train[randomize]\n",
    "        y = y_train[randomize]\n",
    "        loss = 0\n",
    "        for xi, yi in zip(x, y):\n",
    "            loss += max(0, 1 - yi * (np.dot(xi, w)+b))\n",
    "        if (1-yi * (np.dot(xi, w)+b) >0 ): #or loss function > 0 \n",
    "            # grad+=-yi*xi\n",
    "            w =  (1-learning_rate)* w - learning_rate * C *(-yi * xi)\n",
    "            b =  b - learning_rate * C * (-yi)\n",
    "        else:\n",
    "            # grad+=0\n",
    "            w = (1-learning_rate)*w  \n",
    "            b =  b  \n",
    "    \n",
    "    return w,b,loss"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 10,
   "metadata": {},
   "outputs": [],
   "source": [
    "def F1_score(Predict,Y_Test):\n",
    "    P = np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) / (np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) +\\\n",
    "                                                     np.sum(Predict[Y_Test == 0])) # False positive\n",
    "    R = np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) / (np.sum(Predict[Y_Test==1] == Y_Test[Y_Test==1]) +\\\n",
    "                                                     np.sum(Y_Test[Predict==0])) # False negative\n",
    "    \n",
    "    return((2*P*R)/(P+R))"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 12,
   "metadata": {},
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "F1 score 0.09836065573770492 No Differential Privacy\n",
      "F1 score 0.9789029535864979 With Differential Privacy\n"
     ]
    }
   ],
   "source": [
    "def SVMpredict(x,w,b):\n",
    "    Output=np.sign(np.dot(x, w)+b)\n",
    "    Output[Output==-1]=0#convert the predict from +1/-1 to 1/0\n",
    "    Output[Output == 1]=1\n",
    "    return(Output)\n",
    "\n",
    "if __name__ == \"__main__\":\n",
    "    X_Train, Y_Train, X_Test, Y_Test=data_loader()\n",
    "\n",
    "    w, b, loss=get_SVM(X_Train, Y_Train, C = 1)\n",
    "    \n",
    "    w_DP,b_DP= get_DP_SVM(X_Train, Y_Train,epsilon = 1,C = 1,L = 130)\n",
    "\n",
    "    Predict=SVMpredict(X_Test,w, b)\n",
    "    \n",
    "    Predict_DP = SVMpredict(X_Test,w_DP,b_DP)\n",
    "    \n",
    "    \n",
    "\n",
    "    print( \"F1 score \" + str(F1_score(Predict,Y_Test)) + \" No Differential Privacy\")\n",
    "    print(\"F1 score \" + str(F1_score(Predict_DP,Y_Test)) + \" With Differential Privacy\")"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 13,
   "metadata": {},
   "outputs": [],
   "source": [
    "B = np.linspace(1,0.01,100)\n",
    "L , L_DP = [] , []\n",
    "for i in range(len(B)):\n",
    "    \n",
    "    X_Train, Y_Train, X_Test, Y_Test=data_loader()\n",
    "    \n",
    "    w, b, loss = get_SVM(X_Train, Y_Train,C = 19)\n",
    "    w_DP ,b_DP = get_DP_SVM(X_Train, Y_Train,epsilon = B[i],C = 19, L= 130)\n",
    "    \n",
    "    Predict = SVMpredict(X_Test,w, b)\n",
    "    Predict_DP = SVMpredict(X_Test,w_DP,b_DP)\n",
    "    \n",
    "    L.append(F1_score(Predict,Y_Test))\n",
    "    L_DP.append(F1_score(Predict_DP,Y_Test))"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 16,
   "metadata": {},
   "outputs": [
    {
     "data": {
      "text/plain": [
       "Text(0, 0.5, '# of experiments')"
      ]
     },
     "execution_count": 16,
     "metadata": {},
     "output_type": "execute_result"
    },
    {
     "data": {
      "image/png": "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\n",
      "text/plain": [
       "<Figure size 720x360 with 2 Axes>"
      ]
     },
     "metadata": {
      "needs_background": "light"
     },
     "output_type": "display_data"
    }
   ],
   "source": [
    "fig = plt.figure(figsize =(10,5))\n",
    "fig.add_subplot(1,2,1)\n",
    "plt.hist(L)\n",
    "plt.title(\"Distribution of SVM's F1_score without DP\")\n",
    "plt.xlabel('F1_score')\n",
    "plt.ylabel('# of experiments')\n",
    "fig.add_subplot(1,2,2)\n",
    "plt.hist(L_DP)\n",
    "plt.title(\"Distribution of SVM's F1_score with DP\")\n",
    "plt.xlabel('F1_score')\n",
    "plt.ylabel('# of experiments')"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 17,
   "metadata": {},
   "outputs": [
    {
     "data": {
      "text/plain": [
       "Text(0.5, 1.0, \"BoxPlot of SVM's F1_score with DP\")"
      ]
     },
     "execution_count": 17,
     "metadata": {},
     "output_type": "execute_result"
    },
    {
     "data": {
      "image/png": "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\n",
      "text/plain": [
       "<Figure size 720x360 with 2 Axes>"
      ]
     },
     "metadata": {
      "needs_background": "light"
     },
     "output_type": "display_data"
    }
   ],
   "source": [
    "fig = plt.figure(figsize =(10,5))\n",
    "fig.add_subplot(1,2,1)\n",
    "plt.boxplot(L)\n",
    "plt.title(\"BoxPlot of SVM's F1_score without DP\")\n",
    "fig.add_subplot(1,2,2)\n",
    "plt.boxplot(L_DP)\n",
    "plt.title(\"BoxPlot of SVM's F1_score with DP\")"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 18,
   "metadata": {},
   "outputs": [
    {
     "data": {
      "text/plain": [
       "Text(0, 0.5, 'F1_score')"
      ]
     },
     "execution_count": 18,
     "metadata": {},
     "output_type": "execute_result"
    },
    {
     "data": {
      "image/png": "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\n",
      "text/plain": [
       "<Figure size 432x288 with 1 Axes>"
      ]
     },
     "metadata": {
      "needs_background": "light"
     },
     "output_type": "display_data"
    }
   ],
   "source": [
    "plt.plot(B.dot(np.sqrt(100)),L_DP)\n",
    "plt.title('F1_score function of Privacy Budget')\n",
    "plt.xlabel('Big O of Privacy Budget')\n",
    "plt.ylabel('F1_score')"
   ]
  },
  {
   "cell_type": "markdown",
   "metadata": {},
   "source": [
    "Need to compute an optimal C, thus we use Cross Validation on SVM "
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 19,
   "metadata": {},
   "outputs": [],
   "source": [
    "def data_loader_CV():\n",
    "    Data=pd.read_csv(\"Data-Table.csv\")\n",
    "    Data =np.asarray(Data)\n",
    "    Y=Data[:,3]\n",
    "    X=Data[:,9:20]\n",
    "    X =np.asarray(X,dtype=np.float64)\n",
    "    X =np.nan_to_num(X)#Nan to Zeros\n",
    "\n",
    "    X_Train_CV = X[0:int(0.7*0.7*X.shape[0]),:]\n",
    "    Y_Train_CV = Y[0:int(0.7*0.7* Y.shape[0])]\n",
    "    \n",
    "    X_Test_CV = X[int(0.7*0.7*X.shape[0]):int(0.7*X.shape[0]),:]\n",
    "    Y_Test_CV = Y[int(0.7*0.7*Y.shape[0]):int(0.7*Y.shape[0])]\n",
    "\n",
    "    return(X_Train_CV,Y_Train_CV,X_Test_CV,Y_Test_CV)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 26,
   "metadata": {},
   "outputs": [],
   "source": [
    "def SVM_CV(C): \n",
    "    X_Train_CV,Y_Train_CV,X_Test_CV,Y_Test_CV = data_loader_CV()\n",
    "    L = []\n",
    "    for i in range (len(C)):\n",
    "        w, b, loss = get_SVM(X_Train_CV,Y_Train_CV,C[i])\n",
    "        Predict=SVMpredict(X_Test_CV,w, b)\n",
    "        L.append(1/len(Y_Test_CV)*sum([(Y_Test_CV[j]-Predict[j])**2 for j in range(0,len(Y_Test_CV))]))\n",
    "    plt.plot(np.arange(len(C)),L,color ='r')\n",
    "    print(\"optimal C is \" + str(C[L.index(min(L))]))\n",
    "    print(\"RSS is \" + str(min(L)) ) \n",
    "    \n",
    "def SVM_CV_DP(C,epsilon): \n",
    "    X_Train_CV,Y_Train_CV,X_Test_CV,Y_Test_CV = data_loader_CV()\n",
    "    L = []\n",
    "    for i in range (len(C)):\n",
    "        w, b = get_DP_SVM(X_Train_CV,Y_Train_CV,epsilon,C[i],L=130)\n",
    "        Predict=SVMpredict(X_Test_CV,w, b)\n",
    "        L.append(1/len(Y_Test_CV)*sum([(Y_Test_CV[j]-Predict[j])**2 for j in range(0,len(Y_Test_CV))]))\n",
    "        \n",
    "    return [C[L.index(min(L))], min(L)]\n",
    "    print(\"optimal C is \" + str(C[L.index(min(L))]))\n",
    "    print(\"RSS is \" + str(min(L)))"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": 30,
   "metadata": {},
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "optimal C is 10.0\n",
      "RSS is 0.05952380952380952\n"
     ]
    },
    {
     "data": {
      "image/png": "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\n",
      "text/plain": [
       "<Figure size 432x288 with 1 Axes>"
      ]
     },
     "metadata": {
      "needs_background": "light"
     },
     "output_type": "display_data"
    }
   ],
   "source": [
    "C = np.linspace(10,0.001,100)\n",
    "SVM_CV(C)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {},
   "outputs": [],
   "source": [
    "epsilon = np.linspace(1,0.01,20)\n",
    "C = np.linspace(10,0.001,100)\n",
    "L = []\n",
    "M = []\n",
    "N = []\n",
    "for i in epsilon:\n",
    "    a = SVM_CV_DP(C,i)\n",
    "    L.append(a[0])\n",
    "    M.append(a[1])\n",
    "    N.append(i)\n",
    "    \n",
    "d = {\"Optimal C \" : L, \"RSS\": M, \"epsilon\": N }\n",
    "print(pd.DataFrame(data = d))"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {},
   "outputs": [],
   "source": []
  }
 ],
 "metadata": {
  "kernelspec": {
   "display_name": "Python 3",
   "language": "python",
   "name": "python3"
  },
  "language_info": {
   "codemirror_mode": {
    "name": "ipython",
    "version": 3
   },
   "file_extension": ".py",
   "mimetype": "text/x-python",
   "name": "python",
   "nbconvert_exporter": "python",
   "pygments_lexer": "ipython3",
   "version": "3.7.3"
  }
 },
 "nbformat": 4,
 "nbformat_minor": 2
}
