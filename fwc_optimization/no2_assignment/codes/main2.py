#Python libraries for math and graphics
import numpy as np
import cvxpy  as cp

#Declaring variables
l = cp.Variable(pos="true", name="l")
b = cp.Variable(pos="true", name="b")
h = cp.Variable(pos="true", name="h")

#Objective function
V =(l*b*h) ** -1

#End point for iteration
delta = 0.0001
error = 0.05
num_iter = 0

#Initial guess
h.value = 0.01; l.value = 8 - 2*h.value; b.value = 3 - 2*h.value
hk = h.value - 10*delta; lk = 8 - 2*hk; bk = 3 - 2*hk

while(np.abs(h.value - hk) > delta and num_iter < 100):
    hk = h.value; lk = l.value; bk = b.value
    f_val = lk + 2*hk
    ceq1 = f_val * (l/lk) ** (lk/f_val) * (h/hk) ** (2*hk/f_val)
    f_val = bk + 2*hk
    ceq2 = f_val * (b/bk) ** (bk/f_val) * (h/hk) ** (2*hk/f_val)
    constraints = [ceq1 == 8, ceq2 == 3, h >= hk/(1+error) , hk <= min([(1+error)*hk, 1.5])]
    prob = cp.Problem(cp.Minimize(V), constraints)
    if(prob.is_dgp() == False):
        print("Not DGP")
        break
    prob.solve(gp = True)
    num_iter += 1

print("Number iterations:", num_iter)
print("Max volume:", 1/prob.value)
print("Square side:", h.value)
