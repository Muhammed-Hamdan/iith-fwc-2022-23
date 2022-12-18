import numpy as np
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

rng = np.random.default_rng()

min_gamma = 0
max_gamma = 10
num_samples = 1000000

min_bin = -5
max_bin = 5
num_bins = 100
hist_bins = np.linspace(min_bin, max_bin, num_bins)
integ_limit = np.where(hist_bins > 0)[0][0]

prob_error_num = []
prob_error_th = []
gamma_db = np.arange(min_gamma, max_gamma + 1)
for i in gamma_db:
    gamma = 10**(0.1*i)
    A_samples = rng.rayleigh((gamma/2)**0.5, num_samples) 
    N_samples = rng.normal(size=num_samples) 
    prob_error_num.append(np.count_nonzero(A_samples+N_samples < 0)/num_samples)
    prob_error_th.append(0.5-0.5*np.sqrt(gamma)/np.sqrt(2+gamma))
    #Alternative pe calculation using pdf generated from np.histogram
    #X_samples = A_samples + N_samples
    #X_pdf, ret_bins = np.histogram(X_samples, hist_bins, density=True)
    #prob_error_num.append(np.trapz(X_pdf[0:integ_limit], hist_bins[0:integ_limit]))

plt.semilogy(gamma_db, prob_error_num, 'o')
plt.semilogy(gamma_db, prob_error_th)
plt.grid()
plt.xlabel('$\gamma$')
plt.ylabel('$P_e(\gamma)$')
plt.legend(["Simulated","Theory"])

plt.savefig('../../figs/chapter4/prob_error.pdf')
plt.savefig('../../figs/chapter4/prob_error.png')
plt.show()

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter4/prob_error.pdf"))
#else
#plt.show() #opening the plot window

# Inference: Easier computation, switching of operations pe and convolution (instead of convolution, doing expected val)
