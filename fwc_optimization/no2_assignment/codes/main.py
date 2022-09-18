#Python libraries for math and graphics
import numpy as np
import cvxpy  as cp

#Declaring variables
l = cp.Variable(pos="true", name="l")
b = cp.Variable(pos="true", name="b")
h = cp.Variable(pos="true", name="h")

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

#Method 2
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
