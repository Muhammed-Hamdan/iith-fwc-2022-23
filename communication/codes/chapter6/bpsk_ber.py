import numpy as np
import scipy.special as sp
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

def pe_bpsk(snr):
    return 0.5*sp.erfc(np.sqrt(snr))

num_samples = 5000000
# snr = Eb/No, No/2 = 1 (constant), A = sqrt(Eb) = (2*snr)**0.5 
n_var = np.random.normal(0, 1, num_samples)
x_var = np.random.choice([-1, 1], num_samples)
snr_db = np.arange(0, 10+1)
pe_th = []
pe_num = []
for i in snr_db:
    snr = 10**(0.1*i)
    A = (2*snr)**0.5     
    pe_est = (np.count_nonzero((x_var == 1) & (A*x_var + n_var < 0)) + np.count_nonzero((x_var == -1) & (A*x_var + n_var > 0)))/num_samples
    pe_num.append(pe_est)
    pe_th.append(pe_bpsk(snr))

plt.semilogy(snr_db, pe_th)
plt.semilogy(snr_db, pe_num, 'o')
plt.grid()
plt.xlabel("$SNR(\\frac{E_b}{N_0})$")
plt.ylabel("$P_e$")
plt.legend(['Analysis', 'Sim'])

plt.savefig('../../figs/chapter6/bpsk_ber.pdf')
plt.savefig('../../figs/chapter6/bpsk_ber.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter6/bpsk_ber.pdf"))
#else
plt.show() #opening the plot window