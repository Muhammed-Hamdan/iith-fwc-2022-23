import numpy as np
import numpy.linalg as LA

omat = np.array([[0,-1],[1,0]])
# Input parameters
O = np.array([np.sqrt(2),0])
r = 1
d = np.sqrt(2) - 1
R = r + d
theta = 180*(np.pi/180)
h = np.array([R*np.cos(theta), R*np.sin(theta)]) + O
h = h.reshape(2,1)
V = np.array([[1, 0],[0, 1]])
u = -np.array([O[0], O[1]]).reshape(2,1)
f = LA.norm(O)**2-r**2
sigma = (V@h+u)@(V@h+u).T - (h.T@V@h + 2*u.T@h + f)*V
#coeffs = np.array([sigma[1,1], sigma[0,1]+sigma[1,0], sigma[0,0]])
#mu = np.roots(np.array([sigma[1,1], sigma[0,1]+sigma[1,0], sigma[0,0]]))
#print("Roots are:", mu)
m1 = np.array([1, mu[0]]).reshape(2,1)
m2 = np.array([1, mu[1]]).reshape(2,1)
P1 = h - (m1.T@(V@h+u)/m1.T@V@m1)*m1
P2 = h - (m2.T@(V@h+u)/m2.T@V@m2)*m2
n = P1 - h
k = LA.norm(n)*(1+0.4)  # 0.4 is user input for distance from pt P1
P3 = h + k*n
alpha = (V@P3+u)@(V@P3+u).T - (P3.T@V@P3 + 2*P3.T@h + f)*V
m3 = alpha[0,0]/(alpha[1,1]*m1)
n2 = omat@m2
n3 = omat@m3
k = np.array([n2.T@P2, n3.T@P3]).reshape(2,1)
A = np.vstack((n2.T, n3.T))
P4 = LA.inv(A)@k
print(P1)
print(P2)
print(P3)
print(P4)
