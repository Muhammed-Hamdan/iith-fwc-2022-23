#Importing numpy, scipy, mpmath and pyplot
import numpy as np
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if


maxrange=50
maxlim=10.0
x = np.linspace(0,maxlim,maxrange)    #points on the x axis
simlen = int(1e6) #number of samples
err = [] #declaring probability list
pdf = [] #declaring pdf list
h = 2*maxlim/(maxrange-1);
x1_var = np.random.normal(0, 1, simlen)
x2_var = np.random.normal(0, 1, simlen)
v_var = np.sqrt(x1_var**2 + x2_var**2)

for i in range(0,maxrange):
	err_ind = np.nonzero(v_var < x[i]) #checking probability condition
	err_n = np.size(err_ind) #computing the probability
	err.append(err_n/simlen) #storing the probability values in a list
	
pdf = np.gradient(err, x, edge_order=2)

def rayleigh_pdf(x):
	return np.piecewise(x, [x<0, x>=0], [0, lambda x: x*np.exp(-x**2/2)])

def rayleigh_cdf(x):
	return np.piecewise(x, [x<0, x>=0], [0, lambda x: 1-np.exp(-x**2/2)])

plt.plot(x.T,err,'o')   #plotting the CDF numerical
plt.plot(x, rayleigh_cdf(x)) #plotting CDF theory
plt.grid()          #creating the grid
plt.xlabel('$x$')
plt.ylabel('$F_X(x)$')
plt.legend(["Numerical","Theory"])

plt.savefig('../../figs/chapter4/rayleigh_cdf.pdf')
plt.savefig('../../figs/chapter4/rayleigh_cdf.png')

plt.show()
plt.close()
	
plt.plot(x, pdf, 'o')             # plotting estimated PDF
plt.plot(x, rayleigh_pdf(x))             # plotting theory PDF
plt.grid() #creating the grid
plt.xlabel('$x_i$')
plt.ylabel('$p_X(x_i)$')
plt.legend(["Numerical","Theory"])

plt.savefig('../../figs/chapter4/rayleigh_pdf.pdf')
plt.savefig('../../figs/chapter4/rayleigh_pdf.png')

plt.show()

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter4/rayleigh_cdf.pdf"))
#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter4/rayleigh_pdf.pdf"))
#else
#plt.show() #opening the plot window