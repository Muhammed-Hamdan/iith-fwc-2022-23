import numpy as np
import matplotlib.pyplot as plt

maxlim = 20
maxrange = 100
x = np.linspace(-1, maxlim, maxrange) #points on x-axis
simlen = int(1e6) #number of samples
cdf = [] #declaring probability list
uni_randvar = np.loadtxt('../rvsample/uni.dat',dtype='double')
tran_randvar = -2*np.log(1-uni_randvar)

for i in range(0,maxrange):
	cdf_ind = np.nonzero(tran_randvar < x[i]) #checking probability condition
	cdf_n = np.size(cdf_ind) #computing the probability
	cdf.append(cdf_n/simlen) #storing the probability values in a list

plt.plot(x, cdf) #plotting CDF
plt.grid() #creating the grid
plt.xlabel('$x_i$')
plt.ylabel('$F_X(x_i)$')

plt.savefig('../../figs/chapter2/log_uni_cdf.pdf')
plt.savefig('../../figs/chapter2/log_uni_cdf.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter2/log_uni_cdf.pdf"))
#else
plt.show()
