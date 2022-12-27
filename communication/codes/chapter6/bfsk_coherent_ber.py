import numpy as np
import scipy.special as sp
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

def pe_bfsk(snr):
    return 0.5*sp.erfc(np.sqrt(snr/2))

rng = np.random.default_rng()
num_samples = 1000000

s0 = np.array([1,0]).reshape(2,1)

# snr = Eb/No, No/2 = 1 (constant), A = sqrt(Eb) = (2*snr)**0.5 
max_snr = 10
snr_db = np.arange(0, max_snr+1)

p_error_est = np.zeros(snr_db.shape[0])
p_error_th = np.zeros(snr_db.shape[0])
for i in snr_db:
    N_var = rng.normal(size=(2, num_samples))
    snr = 10**(0.1*i)
    A = (2*snr)**0.5
    y_var = A*s0 + N_var
    p_error_val_est = np.count_nonzero(np.where(y_var[0] < y_var[1], 1, 0))/num_samples
    p_error_est[i] = p_error_val_est
    p_error_val_th = pe_bfsk(snr)
    p_error_th[i] = p_error_val_th

plt.semilogy(snr_db, p_error_th)
plt.semilogy(snr_db, p_error_est, 'o')
plt.grid()
plt.xlabel("$SNR(\\frac{E_b}{N_0})$")
plt.ylabel("$P_e$")
plt.legend(['Analysis', 'Sim'])

plt.savefig('../../figs/chapter6/bfsk_coherent_ber.pdf')
plt.savefig('../../figs/chapter6/bfsk_coherent_ber.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter6/bfsk_coherent_ber.pdf"))
#else
plt.show() #opening the plot window