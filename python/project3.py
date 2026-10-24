import torch 
import numpy as np
from parse import parseImage, parseLabel,expected
import matplotlib.pyplot as plt


image=torch.tensor(parseImage("numTraing-images.idx3-ubyte"), dtype=torch.float32)
label=torch.tensor(parseLabel("numTraing-labels.idx1-ubyte"),dtype=torch.long)
image_test=torch.tensor(parseImage("t10k-images.idx3-ubyte"), dtype=torch.float32)
label_test=torch.tensor(parseLabel("t10k-labels.idx1-ubyte"),dtype=torch.long)    

layer_one_size=(128,784)
layer_two_size=(128,128)
layer_three_size=(10,128)
learning_rate=0.001 

epoch=40
size=60000    
lossHistory=[]
lossSum=0 
              
              
parameters = torch.load("parameters.pth", weights_only=True)
layer_one_weights = parameters["W1"].detach().requires_grad_()
layer_two_weights = parameters["W2"].detach().requires_grad_()
layer_three_weights = parameters["W3"].detach().requires_grad_()
layer_one_biases = parameters["b1"].detach().requires_grad_()
layer_two_biases = parameters["b2"].detach().requires_grad_()
layer_three_biases = parameters["b3"].detach().requires_grad_()                     
inputs=torch.zeros(784,1)
                         
#layer_one_weights=(torch.randn(layer_one_size)*0.0505).requires_grad_()
#layer_two_weights=(torch.randn(layer_two_size)*0.125).requires_grad_()
#layer_three_weights=(torch.randn(layer_three_size)*0.125).requires_grad_()
#layer_one_biases = (torch.zeros(128,1)).requires_grad_()
#layer_two_biases = (torch.zeros(128,1)).requires_grad_()
#layer_three_biases=(torch.zeros(10,1)).requires_grad_()



 
def forwardProp():
    z1=layer_one_weights@inputs+layer_one_biases
    a1=torch.relu(z1)
    z2=layer_two_weights@a1+layer_two_biases
    a2=torch.relu(z2) 
    z3=layer_three_weights@a2+layer_three_biases
    prediction = torch.argmax(z3).item()
    return z3,prediction                  
def lossFunction(answer):
    FP=forwardProp()
    outputs=FP[0].T
    loss=torch.nn.CrossEntropyLoss()
    result=loss(outputs,answer)
    return result
def backProp(ans):
    output=lossFunction(ans)
    output.backward()
    return output.item()

def train():
    global inputs
    global layer_one_weights, layer_two_weights, layer_three_weights
    global layer_one_biases, layer_two_biases, layer_three_biases
      
   
   
                                                                                                                                                            
#def plot():                                
#    plt.plot(lossHistory)                  
#    plt.xlabel("Training checkpoint")      
#    plt.ylabel("cross-entropy loss")       
#    plt.show()                             
def correctness(ans):
    FP=forwardProp()
    target=FP[1]
    if ans==target:
        return True
    else:
        return False

def test():
    global inputs
    count=0                                           
    for i in range(10000):                        
        inputs=image_test[i].reshape(784,1)       
        if(correctness(label_test[i])):                
            count+=1                                                                      
    print(count*100/10000, "% accuracy")          


for j in range(epoch):                                                  
    for i in range(size):                                               
        inputs=image[i].reshape(784,1)                                  
        lossSum+=backProp(label[i].reshape(1,))                         
        with torch.no_grad():                                           
            layer_one_weights -= learning_rate*layer_one_weights.grad   
            layer_two_weights -= learning_rate*layer_two_weights.grad   
            layer_three_weights-=learning_rate*layer_three_weights.grad 
            layer_one_biases -=learning_rate*layer_one_biases.grad      
            layer_two_biases -=learning_rate*layer_two_biases.grad      
            layer_three_biases-=learning_rate*layer_three_biases.grad   
        layer_one_weights.grad=None                                     
        layer_two_weights.grad=None                                     
        layer_three_weights.grad=None                                   
        layer_one_biases.grad=None                                      
        layer_two_biases.grad=None                                      
        layer_three_biases.grad=None                                    
        if(i%3000==0):                                                  
            print(f"{((100*(size*j+i))/(epoch*size)):.2f} % completed ")                   
    lossHistory.append(lossSum/60000)                                   
    lossSum=0     


torch.save({                                                                         
    "W1": layer_one_weights,   
    "W2": layer_two_weights,   
    "W3": layer_three_weights, 
    "b1": layer_one_biases,    
    "b2": layer_two_biases,    
    "b3": layer_three_biases   
}, "parameters.pth")     


plt.plot(lossHistory)                
plt.xlabel("Training checkpoint")    
plt.ylabel("cross-entropy loss")     
plt.show()                           

print(lossHistory[-1])
test()