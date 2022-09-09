#Python libraries for math and graphics
import numpy as np
import matplotlib.pyplot as plt
from numpy import linalg as LA

import sys, os                                          #for path to external scripts
script_dir = os.path.dirname(__file__)
lib_relative = '../../../CoordGeo'
fig_relative = '../figs/fig1.pdf'
sys.path.insert(0,os.path.join(script_dir, lib_relative))

#local imports
from line.funcs import *
from triangle.funcs import *
from conics.funcs import circ_gen

#if using termux
import subprocess
import shlex
#end if

#Input parameters
a = 6   #AB = 6
b = 8   #BC = 8
A = np.array(([0,a]))
B = np.array(([0,0]))
C = np.array(([b,0]))
O = (B+C)/2
m = A - C
D = A + (((m.T)@(B-A))/LA.norm(m)**2)*m
At = A - O  #Point A when origin shifted to O
r = b/2
#omega = np.arcsin(r/np.sqrt(At[0]**2+At[1]**2))
omega = np.arctan(At[0]/At[1])
phi = np.arcsin(r/LA.norm(At))
theta = phi - omega
Et = np.array(([r*np.cos(theta), r*np.sin(theta)]))
E = Et + O  #Shift back origin to (0,0)

##Generating all shapes
x_AB = line_gen(A,B)
x_BC = line_gen(C,B)
x_CA = line_gen(A,C)
x_BD = line_gen(B,D)
x_AE = line_gen(A,E)
c_rO = circ_gen(O,r)

#Plotting all shapes
plt.plot(x_AB[0,:],x_AB[1,:])
plt.plot(x_BC[0,:],x_BC[1,:])
plt.plot(x_CA[0,:],x_CA[1,:])
plt.plot(x_BD[0,:],x_BD[1,:])
plt.plot(x_AE[0,:],x_AE[1,:])
plt.plot(c_rO[0,:],c_rO[1,:])


#Labeling the coordinates
plot_coords = np.vstack((A,B,C,D,E,O)).T
plt.scatter(plot_coords[0,:], plot_coords[1,:])
vert_labels = ['A','B','C','D','E','O']
for i, txt in enumerate(vert_labels):
    plt.annotate(txt, # this is the text
                 (plot_coords[0,i], plot_coords[1,i]), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(0,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center

plt.xlabel('$x$')
plt.ylabel('$y$')
plt.grid() # minor
plt.axis('equal')

#if using termux
plt.savefig(os.path.join(script_dir, fig_relative))
#subprocess.run(shlex.split("termux-open "+os.path.join(script_dir, fig_relative)))
#else
#plt.show()
