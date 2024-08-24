import numpy as np

def function(t):
    pi = np.pi

    q = np.cos(2*pi*t)
    x = np.cos(2*pi*t)
    y = np.sin(2*pi*t)
    
    return [q,x,y]


n = 100
interval = np.linspace(0,1,n)

file = open("pointChargesAligned.txt","w")
file.write("charge\tx\ty\n")
for t in interval:
    [q,x,y] = function(t)
    file.write(str(q) + "\t" + str(x) + "\t" + str(y) + "\n")
file.close()