import numpy as np
import torch
from parse import parseImage,parseLabel
import matplotlib.pyplot as plt




image=parseImage("fashionTraing-images-idx3-ubyte")
label=parseLabel("fashionTraing-labels-idx1-ubyte")



 
epoch=10
size=60000
batch_size=32
lossHistory=[]
loss=0



first_conv_layer=torch.nn.Conv2d(1,16,(3,3))
second_conv_layer=torch.nn.Conv2d(16,32,(3,3))

relu=torch.nn.ReLU()                        
pool = torch.nn.MaxPool2d(kernel_size=2, stride=2)
flatten = torch.nn.Flatten()


def convolute(idx):
    input = torch.tensor(image[idx:idx+32], dtype=torch.float32)
    input = input.reshape(-1, 1, 28, 28)

    first_output = first_conv_layer(input)
    first_output = relu(first_output)
    first_output = pool(first_output)

    second_output = second_conv_layer(first_output)
    second_output = relu(second_output)     

    
    conv_inputs = flatten(second_output).T

    return conv_inputs


first_layer_weights=(torch.randn(128,3872)*0.0227).requires_grad_()
second_layer_weights=(torch.randn(10,128)*0.125).requires_grad_()

layer_one_biases = (torch.zeros(128,1)).requires_grad_()
layer_two_biases = (torch.zeros(10,1)).requires_grad_()

def forwardProp():
    z1=first_layer_weights@conv_inputs+layer_one_biases
    
    a1=torch.relu(z1)
    z2=second_layer_weights@a1+layer_two_biases
    return z2

def lossFunction(answer):         
    FP=forwardProp()                
    outputs=FP.T                
    loss=torch.nn.CrossEntropyLoss()
    result=loss(outputs,answer) 
    
    return result                   

def backProp(answer):
    output=lossFunction(answer)
    output.backward()
    return output.item()

learning_rate = 0.001
conv_inputs=convolute(0)
for i in range(epoch):
    for j in range(0,size,batch_size):

        conv_inputs=convolute(j)
        ans=torch.tensor(label[j:j+32],dtype=torch.long)



        first_conv_layer.zero_grad()
        second_conv_layer.zero_grad()

        first_layer_weights.grad = None
        second_layer_weights.grad = None
        layer_one_biases.grad = None
        layer_two_biases.grad = None

        loss += backProp(ans)
        with torch.no_grad():
            first_conv_layer.weight -= learning_rate * first_conv_layer.weight.grad
            first_conv_layer.bias -= learning_rate * first_conv_layer.bias.grad
            second_conv_layer.weight -= learning_rate * second_conv_layer.weight.grad
            second_conv_layer.bias -= learning_rate * second_conv_layer.bias.grad
            first_layer_weights -= learning_rate * first_layer_weights.grad
            second_layer_weights -= learning_rate * second_layer_weights.grad
            layer_one_biases -= learning_rate * layer_one_biases.grad
            layer_two_biases -= learning_rate * layer_two_biases.grad
            
        if (j // batch_size + 1) % 100 == 0:
            progress = 100 * (i * size + j + batch_size) / (epoch * size)
            print(f"{progress:.2f} % completed")



    lossHistory.append(32*loss/60000)
    loss=0


plt.plot(lossHistory)
plt.show()

