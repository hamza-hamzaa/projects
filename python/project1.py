#introdcutory project to machine learning where i try to implement linear regression and learn
# about cost function, forward desc and backpropgation through partial differentials, then using the backdesc
#and the cost function to reduce the error

import numpy as np
import matplotlib.pyplot as plt
size=100
learningStep=0.001
#here i create a random variable and random noise to try and create a best fit line for 
#using linear reggresion 
X_vals=np.random.uniform(0,10,size)
noise=np.random.uniform(0,0.5,size)
Y_vals=5*X_vals+noise
#i initalize a weight and bias which will be changed by backprop
w=0.8
b=0.5

def linearPrediction(X):
    return w*X+b

#implemented the loss function using mean square error 
def costFunc():
    diffSquared=(Y_vals-linearPrediction(X_vals))**2
    cost=(1/size)*sum(diffSquared,0)
    return cost

def WGradDesc():
    diffSquared=X_vals*(linearPrediction(X_vals)-Y_vals)
    WGrad=(2/size)*sum(diffSquared,0)                
    return WGrad
def BGradDesc():                                           
    diffSquared=(linearPrediction(X_vals)-Y_vals)
    WGrad=(2/size)*sum(diffSquared,0)                      
    return WGrad  

costs=[]
generations=10000
# main training loop where i calculate change in bias and weight for slected number of gens.
for i in range(generations):
    if(i%10==0):
        costs.append(costFunc())
    wGrad = WGradDesc()
    bGrad = BGradDesc()
    w -= learningStep * wGrad
    b -= learningStep * bGrad

#here i visualize the actual line and the random noise values
plt.scatter(X_vals, Y_vals)
plt.plot(X_vals, linearPrediction(X_vals), color="red")
plt.show()
print("final bias: " , b)  
print("final Weight: ",w)  
print("final loss", costFunc())