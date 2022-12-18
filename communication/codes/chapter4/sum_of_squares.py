#Importing numpy, scipy, mpmath and pyplot
import numpy as np
import matplotlib.pyplot as plt

#if using termux
#import subprocess
#import shlex
#end if


maxrange=100
maxlim=10.0
x = np.linspace(0,maxlim,maxrange)    #points on the x axis
simlen = int(1e6) #number of samples
err = [] #declaring probability list
pdf = [] #declaring pdf list
h = 2*maxlim/(maxrange-1);
x1_var = np.random.normal(0, 1, simlen)
x2_var = np.random.normal(0, 1, simlen)
v_var = x1_var**2 + x2_var**2

for i in range(0,maxrange):
	err_ind = np.nonzero(v_var < x[i]) #checking probability condition
	err_n = np.size(err_ind) #computing the probability
	err.append(err_n/simlen) #storing the probability values in a list
	
pdf = np.gradient(err, x, edge_order=2)

plt.plot(x.T,err)   #plotting the CDF
plt.grid()          #creating the grid
plt.xlabel('$x$')
plt.ylabel('$F_X(x)$')

plt.savefig('../../figs/chapter4/chisq_cdf.pdf')
plt.savefig('../../figs/chapter4/chisq_cdf.png')

plt.show()
plt.close()
	
plt.plot(x,pdf)             # plotting estimated PDF
plt.grid() #creating the grid
plt.xlabel('$x_i$')
plt.ylabel('$p_X(x_i)$')

plt.savefig('../../figs/chapter4/chisq_pdf.pdf')
plt.savefig('../../figs/chapter4/chisq_pdf.png')

plt.show()

#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter4/chisq_cdf.pdf"))
#if using termux
#subprocess.run(shlex.split("termux-open ../../figs/chapter4/chisq_pdf.pdf"))
#else
#plt.show() #opening the plot window