import numpy as np
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if

sigma = 1
mean = 0
num_samples = 1000000
n1 = np.random.normal(mean, sigma, num_samples)
n2 = np.random.normal(mean, sigma, num_samples)
X = n2-n1
Y = -n2-n1
rho = np.mean((X-np.mean(X))*(Y-np.mean(Y)))/np.sqrt(np.var(X)*np.var(Y))
print(f"Correlation coefficient is: {rho:.7f}")

plt.scatter(X[0:2500],Y[0:2500],s=10)
plt.grid()
plt.xlabel("X")
plt.ylabel("Y")

plt.savefig('../../figs/chapter6/zero_corr_verify.pdf')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter6/zero_corr_verify.pdf"))
#else
plt.show() #opening the plot window