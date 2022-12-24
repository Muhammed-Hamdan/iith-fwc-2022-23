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

plt.show()
plt.close()

def pe_bpsk(x):
    return 0.5*sp.erfc(x/np.sqrt(2))

num_samples = 1000000
n_var = np.random.normal(0, 1, num_samples)
x_var = np.random.choice([-1, 1], num_samples)
A_db = np.arange(0, 10+1)
pe_th = []
pe_num = []
for i in A_db:
    A = 10**(0.1*i)
    pe_est = (np.count_nonzero((x_var == 1) & (A*x_var + n_var < 0)) + np.count_nonzero((x_var == -1) & (A*x_var + n_var > 0)))/num_samples
    pe_num.append(pe_est)
    pe_th.append(pe_bpsk(A))

plt.semilogy(A_db, pe_num, 'o')
plt.semilogy(A_db, pe_th)
plt.grid()
plt.xlabel("$A_{dB}$")
plt.ylabel("$P_e(A_{dB})$")
plt.legend(['Numerical', 'Theory'])

plt.savefig('../../figs/chapter3/bpsk_pe_snr.pdf')
plt.savefig('../../figs/chapter3/bpsk_pe_snr.png')

plt.show()

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter3/bpsk_scatter.pdf"))
#subprocess.run(shlex.split("termux-open ../../figs/chapter3/bpsk_pe_snr.pdf"))
#else
#plt.show() #opening the plot window