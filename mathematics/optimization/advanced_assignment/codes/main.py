#Python libraries for math and graphics
import numpy as np
import matplotlib.pyplot as plt
import cvxpy  as cp

import sys, os                                          #for path to external scripts
script_dir = os.path.dirname(__file__)
lib_relative = '../../../../CoordGeo'
fig_relative = '../figs/fig1.pdf'
sys.path.insert(0,os.path.join(script_dir, lib_relative))

#if using termux
import subprocess
import shlex
#end if

#Declaring variables
l = cp.Variable(pos="true", name="l")
b = cp.Variable(pos="true", name="b")
h = cp.Variable(pos="true", name="h")

#GP 1: Use 3 variables, approximate equality constraints
#Objective function
V =l*b*h

#End point for iteration
delta = 0.001
error = 0.005
max_iter = 100

#Initial guess
h.value = 1; l.value = 8 - 2*h.value; b.value = 3 - 2*h.value
hk = h.value - 10*delta; lk = 8 - 2*hk; bk = 3 - 2*hk

num_iter = 0
while(np.abs(h.value - hk) > delta and num_iter < max_iter):
    hk = h.value; lk = l.value; bk = b.value
    f_val = lk + 2*hk
    ceq1 = f_val * (l/lk) ** (lk/f_val) * (h/hk) ** (2*hk/f_val)
    f_val = bk + 2*hk
    ceq2 = f_val * (b/bk) ** (bk/f_val) * (h/hk) ** (2*hk/f_val)
    constraints = [ceq1 == 8, ceq2 == 3, h >= hk/(1+error) , h <= min([(1+error)*hk, 1.5])]
    prob = cp.Problem(cp.Maximize(V), constraints)
    if(prob.is_dgp() == False):
        print("Not DGP")
        break
    prob.solve(gp = True)
    num_iter += 1

print("Number iterations:", num_iter)
print("Max volume:", prob.value)
print("Square side:", h.value)

#GP 2: Use 1 variable, approximate objective function
x = cp.Variable(pos="true", name="x")

#End point for iteration
delta = 0.001
error = 0.005
max_iter = 100

#Initial guess
x.value = 1 
xk = x.value - 10*delta 

num_iter = 0
while(np.abs(x.value - xk) > delta and num_iter < 100):
    xk = x.value
    f_val = 4*xk**3 - 22*xk**2 + 24*xk
    V = f_val * (x/xk) ** ((xk/f_val)*(12*xk**2 - 44*xk + 24))
    constraints = [x >= xk/(1+error) , x <= min([(1+error)*xk, 1.5])]
    prob = cp.Problem(cp.Maximize(V), constraints)
    if(prob.is_dgp() == False):
        print("Not DGP")
        break
    prob.solve(gp = True)
    num_iter += 1
    
print("Number iterations:", num_iter)
print("Max volume:", prob.value)
print("Square side:", x.value)

#Gradient ascent

#Defining f(x)
def f(x,a,b,c,d):
	return a * x**3 + b * x**2 + c * x + d
a = 4
b = -22
c = 24
d = 0
label_str = "$4x^3 - 22x^2 + 24x$"

#For maxima using gradient ascent
cur_x = 0.5
alpha = 0.001 
precision = 0.00000001 
previous_step_size = 1
max_iters = 100000000 
iters = 0

#Defining derivative of f(x)
df = lambda x: 3*a*x**2 + 2*b*x + c           

#Gradient ascent calculation
while (previous_step_size > precision) & (iters < max_iters) :
    prev_x = cur_x             
    cur_x += alpha * df(prev_x)   
    previous_step_size = abs(cur_x - prev_x)   
    iters+=1  

max_val = f(cur_x,a,b,c,d)
print("Maximum value of f(x) is ", max_val, "at","x =",cur_x)

#Plotting f(x)
x=np.linspace(-1,5,100)
y=f(x,a,b,c,d)
plt.plot(x,y,label=label_str)
#Labelling points
plt.plot(cur_x,max_val,'o')
plt.text(cur_x, max_val,f'P({cur_x:.4f},{max_val:.4f})')

plt.xlabel("x-axis")
plt.ylabel("y-axis")
plt.grid()
plt.legend()

#if using termux
plt.savefig(os.path.join(script_dir, fig_relative))
#subprocess.run(shlex.split("termux-open "+os.path.join(script_dir, fig_relative)))
#else
#plt.show()
