#moving on from my first project which was implemeted using linear regression this one uses 
# logistic regression which is much closer to what current machine learning models use 
# here i try to determine if students will pass or fail based on 3 variables 


import numpy as np
import matplotlib.pyplot as plt

#my logistic reg. function is the sigmoid, which despite being outdated in the current ML era it was sufficent
# enough for this project                                                                                    
def sigmoid(z):                                                                                              
    return 1/(1+np.exp(-z))                                                                                  

studentNum=1000
learningSpeed=0.001
#initalize random values to my 3 calculated variables
attendance=  np.random.uniform(0,100,studentNum)
hoursStudied=np.random.uniform(0,15,studentNum)
previousScore=np.random.uniform(0,100,studentNum)

passedProb=sigmoid(0.03*attendance+0.3*hoursStudied+0.04*previousScore-3)
passed = np.random.binomial(1, passedProb)
w1=0.015
w2=1
w3=0
b0=-1 

                                                                                                                                                                                                                          
def logisticPrediction(a,h,p):
    return sigmoid(a*w1+h*w2+p*w3+b0)
def lossFunc():
    pred = logisticPrediction(attendance,hoursStudied,previousScore)
    pred = np.clip(pred, 1e-15, 1 - 1e-15)
    return -np.mean(passed * np.log(pred) +(1 - passed) * np.log(1 - pred))

X = np.column_stack((attendance, hoursStudied, previousScore))
XT = X.T

def WGradDesc():
    return 1/studentNum*XT@(logisticPrediction(attendance,hoursStudied,previousScore)-passed)
def BGradDesc():
    return np.mean(logisticPrediction(attendance,hoursStudied,previousScore)-passed)
lossHistory=[]



#main training loop
for i in range(10000):
    wgrad=WGradDesc()
    bgrad=BGradDesc()
    w1=w1-learningSpeed*wgrad[0]
    w2=w2-learningSpeed*wgrad[1]
    w3=w3-learningSpeed*wgrad[2]
    b0=b0-learningSpeed*bgrad
    if i % 100 == 0:
        lossHistory.append(lossFunc())
plt.plot(lossHistory)
plt.xlabel("Training checkpoint")
plt.ylabel("Binary cross-entropy loss")
plt.show()
print(w1)
print(w2)
print(w3)
print(b0)
