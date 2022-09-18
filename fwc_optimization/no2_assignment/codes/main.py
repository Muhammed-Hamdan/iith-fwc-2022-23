#Python libraries for math and graphics
import numpy as np
import cvxpy  as cp

#Declaring variables
l = cp.Variable(pos="true", name="l")
b = cp.Variable(pos="true", name="b")
h = cp.Variable(pos="true", name="h")

#Objective function
V =l*b*h

#Constraints
constraints = [l + 2*h == 8, b + 2*h == 3, h <= 1.5]

#Problem formulation
prob = cp.Problem(cp.Maximize(V), constraints)

#Checking curvature of log function
#print(V.log_log_curvature)

#Checking if the problem is DGP
print("Is the problem DGP ?", prob.is_dgp())

#solution
#prob.solve(gp=True)
#print("status:", prob.status)
#print("optimal value:", prob.value)
#print("optimal var:", x.value)
