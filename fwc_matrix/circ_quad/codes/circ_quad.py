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
P1 = h

sigma = (V@h+u)@(V@h+u).T - (h.T@V@h + 2*u.T@h + f)*V
lamda, gamma = LA.eigh(sigma)
n1 = gamma.T@np.array([np.sqrt(np.abs(lamda[0])), np.sqrt(np.abs(lamda[1]))])
n1 = n1.reshape(2,1)
n2 = gamma.T@np.array([np.sqrt(np.abs(lamda[0])), -np.sqrt(np.abs(lamda[1]))])
n2 = n2.reshape(2,1)
#print("Roots are:", mu)
m1 = omat@n1
m2 = omat@n2
T1 = h - ((m1.T@(V@h+u))/(m1.T@V@m1))*m1
T2 = h - ((m2.T@(V@h+u))/(m2.T@V@m2))*m2
print(T1)
print(T2)
dist = LA.norm(T1 - h)
k1 = dist*(1+0.4)  # 0.4 is user input for distance from pt P1
P2 = h + (k1/LA.norm(m1))*m1
n3 = np.array([[1, 0],[0, -1]])@LA.inv(gamma.T)@n1
PE_A = np.vstack((n2.T, n3.T))
PE_c = np.array([n2.T@P1, n3.T@P2]).reshape(2,1)
PE = LA.inv(PE_A)@PE_c
dist = LA.norm(T2-PE)
k2 = dist/(1+0.6) # 0.6 is user input for distance from pt T2  
P3 = T2 + (k2/LA.norm(m2))*m2
n4 = np.array([[1, 0],[0, -1]])@LA.inv(gamma.T)@n2
P4_A = np.vstack((n3.T, n4.T))
P4_c = np.array([n3.T@P2, n4.T@P3]).reshape(2,1)
P4 = LA.inv(P4_A)@P4_c
'''
alpha = (V@P3+u)@(V@P3+u).T - (P3.T@V@P3 + 2*P3.T@h + f)*V
m3 = alpha[0,0]/(alpha[1,1]*m1)
n2 = omat@m2
n3 = omat@m3
k = np.array([n2.T@P2, n3.T@P3]).reshape(2,1)
A = np.vstack((n2.T, n3.T))
P4 = LA.inv(A)@k
'''
print(P1)
print(P2)
print(P3)
print(P4)
