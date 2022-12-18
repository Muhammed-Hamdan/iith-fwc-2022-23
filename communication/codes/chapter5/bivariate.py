import numpy as np
import matplotlib.pyplot as plt
import scipy.special as sp

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
plt.grid()

plt.savefig('../../figs/chapter5/biv_scatter.pdf')
plt.savefig('../../figs/chapter5/biv_scatter.png')

plt.show()
plt.close()

num_samples = 1000000
max_snr = 10
snr_db = np.arange(0, max_snr+1)
p_error_est = []
p_error_th = []
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

plt.savefig('../../figs/chapter5/pe_vs_snr.pdf')
plt.savefig('../../figs/chapter5/pe_vs_snr.png')

plt.show()