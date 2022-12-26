import numpy as np
import scipy.special as sp
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

def gaussian_function(x, mean, stdev):
    return np.exp(-(x-mean)**2/(2*stdev**2))/(stdev*np.sqrt(2*np.pi))

p = 0.3
A_db = 3
A = 10**(0.1*A_db)
delta = np.log((1-p)/p)/(2*A)
x = np.linspace(-A-3, A+3, 100)
y_x0 = p*gaussian_function(x, A, 1)
y_x1 = (1-p)*gaussian_function(x, -A, 1)

plt.plot(x, y_x0)
plt.plot(x, y_x1)
plt.vlines(delta, np.min(y_x1), np.max(y_x1), colors=['red'])
plt.grid()
plt.xlabel("$y$")
plt.ylabel("$P_X(x)P_Y(y|x)$")
plt.legend(['$pP_Y(y|x=1)$', '$(1-p)P_Y(y|x=-1)$', "$y=\\frac{1}{2A}\ln\left(\\frac{1}{p}-1\\right)$"])

plt.savefig('../../figs/chapter3/bpsk_map_density.pdf')
plt.savefig('../../figs/chapter3/bpsk_map_density.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter3/bpsk_map_density.pdf"))
#else
plt.show() #opening the plot window