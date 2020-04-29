import matplotlib.pyplot as plt
import numpy as np
import serial
import time

Fs = 10;  # sampling rate
Ts = 1.0/Fs; # sampling interval
t = np.arange(0,10,Ts) # time vector; create Fs samples between 0 and 1.0 sec.
x = np.zeros(100) # signal vector; create Fs samples
y = np.zeros(100) # signal vector; create Fs samples
z = np.zeros(100) # signal vector; create Fs samples
over = np.zeros(100) # signal vector; create Fs samples

n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # a vector of frequencies; two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev,115200)
#s = serial.Serial(serdev)

for i in range(0, 400):
    line=s.readline() # Read an echo string from K66F terminated with '\n'
    if i%4 ==0 :
        i = int (i/4)
        x[i] = line
        #x[i] = float(line)
    elif i%4 ==1 :
        i = int (i/4) 
        y[i] = line
        #y[i] = float(line)
    elif i%4 ==2 :
        i = int (i/4)
        z[i] = line
        #z[i] = float(line)
    elif i%4 ==3 :
        i = int (i/4)
        over [i] = line
        #over[i] = int(line)
    # print line
    
   
   
    


fig, ax = plt.subplots(2, 1)
ax[0].plot(t,x,label='x')
ax[0].plot(t,y,label='y')
ax[0].plot(t,z,label='z')
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Acc Vector')
ax[0].legend()

ax[1].stem(t,over)
ax[1].set_xlabel('Time')
ax[1].set_ylabel('Over')

plt.show()
s.close()