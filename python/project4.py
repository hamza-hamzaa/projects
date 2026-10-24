from sklearn.datasets import fetch_california_housing
import matplotlib.pyplot as plt
import torch
import numpy

data = fetch_california_housing(as_frame=True)
df = data.frame
prices=torch.tensor(df.MedHouseVal)
Medinc=torch.tensor(df.MedInc).reshape(20640,1)
HouseAge_input =torch.tensor(df.HouseAge).reshape(20640,1)
AveRooms_input =torch.tensor(df.AveRooms ).reshape(20640,1)
AveBedrms_input=torch.tensor(df.AveBedrms).reshape(20640,1)
Populatin_input=torch.tensor(df.Population).reshape(20640,1)
AveOccup_input =torch.tensor(df.AveOccup ).reshape(20640,1)
Latitude_input =torch.tensor(df.Latitude ).reshape(20640,1)
Longitude_input=torch.tensor(df.Longitude).reshape(20640,1)
 

Medinc=(Medinc-torch.mean(Medinc))/torch.std(Medinc)
HouseAge_input =(HouseAge_input -torch.mean(HouseAge_input ))/torch.std(HouseAge_input )
AveRooms_input =(AveRooms_input -torch.mean(AveRooms_input ))/torch.std(AveRooms_input )
AveBedrms_input=(AveBedrms_input-torch.mean(AveBedrms_input))/torch.std(AveBedrms_input)
Populatin_input=(Populatin_input-torch.mean(Populatin_input))/torch.std(Populatin_input)
AveOccup_input =(AveOccup_input -torch.mean(AveOccup_input ))/torch.std(AveOccup_input )
Latitude_input =(Latitude_input -torch.mean(Latitude_input ))/torch.std(Latitude_input )
Longitude_input=(Longitude_input-torch.mean(Longitude_input))/torch.std(Longitude_input)


                                

layer_one_size=(64,8) 
layer_two_size=(32,64) 
layer_three_size=(1,32)
learning_rate=0.001      

inputs=torch.zeros(8,1)
layer_one_weights=(torch.randn(layer_one_size)*0.5).requires_grad_()   
layer_two_weights=(torch.randn(layer_two_size)*0.177).requires_grad_()    
layer_three_weights=(torch.randn(layer_three_size)*0.25).requires_grad_()
layer_one_biases = (torch.zeros(64,1)).requires_grad_()                  
layer_two_biases = (torch.zeros(32,1)).requires_grad_()                  
layer_three_biases=(torch.zeros(1,1)).requires_grad_()                   


    







def forwardProp():
    z1=layer_one_weights@inputs+layer_one_biases
    a1=torch.relu(z1)
    z2=layer_two_weights@a1+layer_two_biases
    a2=torch.relu(z2)
    z3=layer_three_weights@a2+layer_three_biases
    return z3


def MSELoss(answer):
    answer=torch.tensor(answer, dtype=torch.float32).reshape(1,1)
    loss=torch.nn.MSELoss()
    l=loss(forwardProp(),answer)
    return l

def backProp(ans):          
    output=MSELoss(ans)
    output.backward()
    return output.item() 

losshistory=[]   
lossSum=0   
for j in range(8):
    for i in range(20640):
        inputs=torch.tensor([Medinc[i],HouseAge_input[i],AveRooms_input[i],AveBedrms_input[i],Populatin_input[i],AveOccup_input[i],Latitude_input[i],Longitude_input[i]], dtype=torch.float32).reshape(8,1)
        lossSum+=backProp(prices[i].item())
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
            print((100*(20640*j+i))/(8*20640)," %")

    losshistory.append(lossSum/20640)
    lossSum=0               

plt.plot(losshistory)                
plt.xlabel("Training checkpoint")    
plt.ylabel("cross-entropy loss")     
plt.show()                           