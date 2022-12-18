#Importing numpy, scipy, mpmath and pyplot
import numpy as np
import mpmath as mp
import scipy 
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if


maxrange=50
maxlim=6.0
x = np.linspace(-maxlim,maxlim,maxrange)    #points on the x axis
simlen = int(1e6) #number of samples
err = [] #declaring probability list
pdf = [] #declaring pdf list
h = 2*maxlim/(maxrange-1);
randvar = np.loadtxt('../rvsample/gau.dat', dtype='double')

for i in range(0,maxrange):
	err_ind = np.nonzero(randvar < x[i]) #checking probability condition
	err_n = np.size(err_ind) #computing the probability
	err.append(err_n/simlen) #storing the probability values in a list
	
pdf = np.gradient(err, x, edge_order=2)

def gauss_pdf(x):
	return 1/mp.sqrt(2*np.pi)*np.exp(-x**2/2.0)

vec_gauss_pdf = scipy.vectorize(gauss_pdf)

plt.plot(x.T,err)   #plotting the CDF
plt.grid()          #creating the grid
plt.xlabel('$x$')
plt.ylabel('$F_X(x)$')

plt.savefig('../../figs/chapter2/gau_cdf.pdf')
plt.savefig('../../figs/chapter2/gau_cdf.png')

plt.show()
plt.close()
	
plt.plot(x,pdf,'o')             # plotting estimated PDF
plt.plot(x,vec_gauss_pdf(x))    # plotting theoretical PDF
plt.grid() #creating the grid
plt.xlabel('$x_i$')
plt.ylabel('$p_X(x_i)$')
plt.legend(["Numerical","Theory"])

plt.savefig('../../figs/chapter2/gau_pdf.pdf')
plt.savefig('../../figs/chapter2/gau_pdf.png')

plt.show()

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter2/gau_cdf.pdf"))
#subprocess.run(shlex.split("termux-open ../../figs/chapter2/gau_pdf.pdf"))
#else
#plt.show() #opening the plot window