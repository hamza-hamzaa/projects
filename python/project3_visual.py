# after completing project3 i wanted to visualize how the numbers actually looked like in the MNIST dataset
# so here i used a window creating,visualizing lib like tkinter since it allows me to preview many window 
#simultaneously then based on the images dim i printed each image 
import tkinter as tk
from parse import parseImage,parseLabel



root = tk.Tk()
root.configure(bg="lightblue")
image=parseImage("fashionTraing-images-idx3-ubyte")
label=parseLabel("fashionTraing-labels-idx1-ubyte")
#create 8 canvas that can hold 8 MNIST images at once then all 60,000 will loop over the 8 images
ca1 = tk.Canvas(root, width=280, height=280)
ca2 = tk.Canvas(root, width=280, height=280)
ca3 = tk.Canvas(root, width=280, height=280)
ca4 = tk.Canvas(root, width=280, height=280) 
ca5 = tk.Canvas(root, width=280, height=280)
ca6 = tk.Canvas(root, width=280, height=280)
ca7 = tk.Canvas(root, width=280, height=280)
ca8 = tk.Canvas(root, width=280, height=280)
# here this positions the imges in a grid like structure
ca1.grid(row=0, column=0, padx=10, pady=10) 
ca2.grid(row=0, column=1, padx=10, pady=10) 
ca3.grid(row=0, column=2, padx=10, pady=10) 
ca4.grid(row=0, column=3, padx=10, pady=10) 
ca5.grid(row=1, column=0, padx=10, pady=10) 
ca6.grid(row=1, column=1, padx=10, pady=10) 
ca7.grid(row=1, column=2, padx=10, pady=10) 
ca8.grid(row=1, column=3, padx=10, pady=10) 
canvases=[ca1,ca2,ca3,ca4,ca5,ca6,ca7,ca8] 
index=0

# created a function that runs for a set time then goes to the next itteration 
def update():
    global index
    for canvas in canvases:
        canvas.delete("all")

        for i in range(784):
            x = (i % 28) * 10
            y = (i // 28) * 10
            c =255-int(255 * image[index][i])
            colour = f"#{c:02x}{c:02x}{c:02x}"
            canvas.create_rectangle(x, y, x + 10, y + 10,fill=colour,outline="")
        index=(index+1)%60000

        
        
    root.after(1, update)

update()
root.mainloop()