from mpmath import *
import numpy as np
import matplotlib.pyplot as plt

a = 1
b = -1
c = -1
roots = np.roots([a,b,c])
alpha = roots[0] 
beta = roots[1]

def a_n(n):
    if(n<1):
        return None
    else:
        return (alpha**n - beta**n)/(alpha-beta)

def b_n(n):
    if(n<1):
        return None
    elif(n==1):
        return 1
    else:
        return a_n(n-1) + a_n(n+1)

# Proof 1
num_iter = 100
lhs = np.empty(num_iter)
rhs = np.empty(num_iter)

lhs[0] = a_n(1) 
rhs[0] = a_n(1+2)-1
for i in range(2,num_iter+1):
    lhs[i-1] = lhs[i-2] + a_n(i)
    rhs[i-1] = a_n(i+2)-1
lhs = lhs.astype(np.float32)
rhs = rhs.astype(np.float32)

print("Proof 1")
print("Sum of error: ", np.sum(np.abs(lhs-rhs)))

n = np.arange(1, num_iter+1)
plt.plot(n, lhs)
plt.plot(n, rhs, 'o')
plt.show()

# Proof 2
rhs = 10/89

sum_length = 100
lhs = 0
for i in range(0, sum_length):
    lhs += a_n(i+1)/10**(i+1)

print("Proof 2")
print("LHS value: ", lhs)
print("RHS value:", rhs)
print("Error: ", np.abs(lhs-rhs))

# Proof 3
num_iter = 50
lhs = np.empty(num_iter)
rhs = np.empty(num_iter)
for i in range(1, num_iter+1):
    lhs[i-1] = b_n(i)
    rhs[i-1] = alpha**i + beta**i
lhs = lhs.astype(np.float32)
rhs = rhs.astype(np.float32)

print("Proof 3")
print("Sum of error: ", np.sum(np.abs(lhs-rhs)))

# Proof 4
rhs = 12/89

sum_length = 100
lhs = 0
for i in range(0, sum_length):
    lhs += b_n(i+1)/10**(i+1)

print("Proof 2")
print("LHS value: ", lhs)
print("RHS value:", rhs)
print("Error: ", np.abs(lhs-rhs))