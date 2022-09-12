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
from conics.funcs import parab_gen

#if using termux
import subprocess
import shlex
#end if

def affine_transform(P,c,x):
    return P@x + c

def delete_zero_rows(a,b):
    zero_row = np.zeros((a.shape[1],))
    num_deleted = 0
    for i in range(0,a.shape[0]):
        if(np.array_equal(a[i,:],zero_row) and b[i]==0):
            a = np.delete(a, i-num_deleted, axis=0)
            b = np.delete(b, i-num_deleted, axis=0)
            num_deleted+=1
    return a,b

#Input parameters
V = np.array([[1,0],[0,0]])
u = np.array(([0,-6]))
f = 0
lamda,P = LA.eigh(V)
if(lamda[1] == 0):      # If eigen value negative, present at start of lamda 
    lamda = np.flip(lamda)
    P = np.flip(P,axis=1)
eta = u@P[:,0]
a = np.vstack((u.T + eta*P[:,0].T, V))
b = np.hstack((-f, eta*P[:,0]-u)) 
a,b = delete_zero_rows(a,b)
c = LA.solve(a,b)
fl = np.abs(2*eta/lamda[1]) # Focal length
O_std = np.array(([0,0]))
F_std = np.array(([fl,0]))
A_std = np.array(([fl,fl]))
B_std = np.array(([fl,-fl]))

num_points = 50
delta = 2*fl/10
p_y = np.linspace(-fl-delta,fl+delta,num_points)
a = -2*eta   # y^2 = ax => y'Dy = (-2eta)e1'y

##Generating all shapes
p_x = parab_gen(p_y,a)
p_std = np.vstack((p_x,p_y)).T

##Affine transformation
p = np.array([affine_transform(P,c,p_std[i,:]) for i in range(0,num_points)]).T
A = affine_transform(P,c,A_std)
B = affine_transform(P,c,B_std)
F = affine_transform(P,c,F_std)
O = affine_transform(P,c,O_std)

# Generating lines after transforming points
x_AB = line_gen(A,B)
x_AO = line_gen(A,O)
x_BO = line_gen(B,O)

#Plotting all shapes
plt.plot(x_AB[0,:],x_AB[1,:])
plt.plot(x_AO[0,:],x_AO[1,:])
plt.plot(x_BO[0,:],x_BO[1,:])
plt.plot(p[0,:], p[1,:])


#Labeling the coordinates
plot_coords = np.vstack((A,B,F,O)).T
plt.scatter(plot_coords[0,:], plot_coords[1,:])
vert_labels = ['A','B','F','O']
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
