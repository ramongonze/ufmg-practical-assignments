import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import math

X = np.array([(1,0,0), (1,0,1), (1,1,0), (1,1,1)]).astype('float')
Y = np.array([-1,1,1,-1]).astype('float')
W = np.array([0.5,0,0]).astype('float')
l = 0.01 # Learning rate
i = 0

def fw(Xx):
    global W
    SUM = 0
    for i in range(len(Xx)):
        SUM += (Xx[i]*W[i])

    return np.sign(SUM)

def perceptron(k):
    global W, X, Y, l, i

    p1.clear()
    p1.scatter([0,1],[1,0], color='blue')
    p1.scatter([0,1],[0,1], color='green')
    XX = np.arange(-0.5, 2, 0.01)
    YY = []
    for x in XX:
        if W[2] == 0:
            YY += [5]
        else:
            YY += [(-W[0] - (W[1]*x))/W[2]]
    print(W)
    # YY = [if W[2]==0: 5 else (-W[0] - (W[1]*x))/W[2] for x in XX]
    p1.plot(XX, YY, color='black')
    plt.xlim(-0.5,1.5)
    plt.ylim(-0.5,1.5)


    FW = fw(X[i])
    if FW > 0 and Y[i] < 0:
        W = W - l*X[i]
    elif FW < 0 and Y[i] > 0:
        W = W + l*X[i]

    i = (i+1)%len(X)

fig = plt.figure()
p1 = fig.add_subplot(111)
p1.scatter([0,1,0],[0,0,1], color='blue')
p1.scatter([1],[1], color='green')


ani = animation.FuncAnimation(fig, perceptron, interval=10)
plt.show()
