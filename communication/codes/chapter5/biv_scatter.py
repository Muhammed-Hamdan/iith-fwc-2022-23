import numpy as np
import matplotlib.pyplot as plt
import scipy.special as sp

#if using termux
#import subprocess
#import shlex
#end if

rng = np.random.default_rng()

A = 4
s0 = np.array([1,0]).reshape(2,1)
s1 = np.array([0,1]).reshape(2,1)

num_samples = 500
N_var = rng.normal(size=(2, num_samples))

y_s0 = A*s0 + N_var
y_s1 = A*s1 + N_var

# Condition is x^ = s0 if y>x, x^ = s1 if y<x
plt.scatter(y_s0[0], y_s0[1])
plt.scatter(y_s1[0], y_s1[1])
plt.plot(np.linspace(-1, A+1, 100), np.linspace(-1, A+1, 100), color='red')
plt.xlabel('x')
plt.ylabel('y')
plt.legend(["$y|s_0$","$y|s_1$"])
plt.grid()

plt.savefig('../../figs/chapter5/biv_scatter.pdf')
plt.savefig('../../figs/chapter5/biv_scatter.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter5/biv_scatter.pdf"))
#else
plt.show() #opening the plot window