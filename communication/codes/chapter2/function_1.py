import numpy as np
import matplotlib.pyplot as plt

maxlim = 20
maxpts = 50
x = np.linspace(-1, maxlim, maxpts) #points on x-axis
simlen = int(1e6) #number of samples
cdf = [] #declaring probability list
uni_randvar = np.loadtxt('../rvsample/uni.dat',dtype='double')
tran_randvar = -2*np.log(1-uni_randvar)

for i in range(0,maxpts):
	cdf_ind = np.nonzero(tran_randvar < x[i]) #checking probability condition
	cdf_n = np.size(cdf_ind) #computing the probability
	cdf.append(cdf_n/simlen) #storing the probability values in a list

def exp_cdf(x):
	return np.piecewise(x, [x<0, x>=0], [0, lambda x: 1-np.exp(-x/2)])

plt.plot(x, cdf, 'o') #plotting CDF numerical
plt.plot(x, exp_cdf(x)) #plottong CDF theory
plt.grid()
plt.xlabel('$x_i$')
plt.ylabel('$F_X(x_i)$')
plt.legend(["Numerical","Theory"])

plt.savefig('../../figs/chapter2/log_uni_cdf.pdf')
plt.savefig('../../figs/chapter2/log_uni_cdf.png')

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter2/log_uni_cdf.pdf"))
#else
plt.show()
