import numpy as np
import scipy.special as sp
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

num_samples = 500
x_var = np.random.choice([-1, 1], num_samples)
n_var = np.random.normal(0, 1, num_samples)
A_db = 3
A = 10**(0.1*A_db)
y_var = A*x_var + n_var

plt.scatter(x_var, y_var)
plt.plot(np.linspace(-1-0.5, 1+0.5, 20), np.zeros(20), color='red')
plt.grid()

plt.savefig('../../figs/chapter3/bpsk_scatter.pdf')
plt.savefig('../../figs/chapter3/bpsk_scatter.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter3/bpsk_scatter.pdf"))
#else
plt.show() #opening the plot window