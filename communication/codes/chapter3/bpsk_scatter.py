import numpy as np
import scipy.special as sp
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

num_samples = 500
x_var = np.random.binomial(1, 0.5, num_samples)*2-1
n_var = np.random.normal(0, 1, num_samples)
A_db = 3
A = 10**(0.1*A_db)
y_var = A*x_var + n_var

x0 = np.extract(x_var==1, x_var)
x1 = np.extract(x_var==-1, x_var)
n0 = np.extract(x_var==1, n_var)
n1 = np.extract(x_var==-1, n_var)
y0 = A*x0 + n0
y1 = A*x1 + n1

plt.plot(y0, np.zeros(y0.shape[0]), 'o', mfc='none')
plt.plot(y1, np.zeros(y1.shape[0]), 'o', mfc='none')
plt.plot(np.zeros(10), np.linspace(-0.1,0.1,10), color="red")
plt.grid()
plt.legend(["$y|0$","$y|1$"])

plt.savefig('../../figs/chapter3/bpsk_scatter.pdf')
plt.savefig('../../figs/chapter3/bpsk_scatter.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter3/bpsk_scatter.pdf"))
#else
plt.show() #opening the plot window
