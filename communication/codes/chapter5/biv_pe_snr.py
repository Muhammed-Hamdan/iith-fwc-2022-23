import numpy as np
import matplotlib.pyplot as plt
import scipy.special as sp

#if using termux
#import subprocess
#import shlex
#end if

rng = np.random.default_rng()
num_samples = 1000000

s0 = np.array([1,0]).reshape(2,1)

max_snr = 10
snr_db = np.arange(0, max_snr+1)

p_error_est = np.zeros(snr_db.shape[0])
p_error_th = np.zeros(snr_db.shape[0])
for i in snr_db:
    N_var = rng.normal(size=(2, num_samples))
    snr = 10**(0.1*i)
    y_var = snr*s0 + N_var
    p_error_val_est = np.count_nonzero(np.where(y_var[0] < y_var[1], 1, 0))/num_samples
    p_error_est[i] = p_error_val_est
    p_error_val_th = 0.5*sp.erfc(snr/2)
    p_error_th[i] = p_error_val_th

plt.semilogy(snr_db, p_error_est, 'o')
plt.semilogy(snr_db, p_error_th)
plt.grid()
plt.xlabel('$SNR_{dB}$')
plt.ylabel('$P_e(SNR_{dB})$')
plt.legend(["Numerical","Theory"])

plt.savefig('../../figs/chapter5/biv_pe_vs_snr.pdf')
plt.savefig('../../figs/chapter5/biv_pe_vs_snr.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter5/biv_pe_vs_snr.pdf"))
#else
plt.show() #opening the plot window