import os
import numpy as np

def parseLabel(file_name):
    labels = np.array([], dtype=np.uint8)
    with open(file_name, 'rb') as f:
        content = f.read()
    labelNum = int.from_bytes(content[4:8], byteorder="big")
    currentByte = 8
    for i in range(labelNum):
        label = np.array(content[currentByte])
        labels = np.append(labels, label)
        currentByte += 1
    return labels

def parseImage(file_name):
    images = []

    with open(file_name, 'rb') as f:
        content = f.read()

    imageNum = int.from_bytes(content[4:8], byteorder="big")

    currentByte = 16

    for i in range(imageNum):
        pixels = np.frombuffer(content[currentByte:currentByte + 784],dtype=np.uint8)/255
        images.append(pixels)
        currentByte += 784

    return np.array(images)

def expected(expectedNum):
    expected=np.zeros(10,dtype=int)
    expected[expectedNum]=1
    return expected


#image=parseImage("train-images.idx3-ubyte")
#label=parseLabel("train-labels.idx1-ubyte")
#print(type(image))





