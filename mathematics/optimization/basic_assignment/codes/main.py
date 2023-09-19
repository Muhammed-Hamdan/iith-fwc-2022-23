#Python libraries for math and graphics
import numpy as np
import mpmath as mp
import matplotlib.pyplot as plt
from numpy import linalg as LA
from pylab import *
import cvxpy  as cp


import sys, os                                          #for path to external scripts
script_dir = os.path.dirname(__file__)
lib_relative = '../../../../CoordGeo'
fig_relative = '../figs/fig1.pdf'
sys.path.insert(0,os.path.join(script_dir, lib_relative))

#local imports
from line.funcs import *
from triangle.funcs import *
from conics.funcs import *

#if using termux
import subprocess
import shlex
#end if

# ax+by+cz = d
EQ = np.array(( [1.0, 0.0, -3.0], [1.0, -1.0, 0.0]))
EQ_b = np.array([0.0,2.0]).reshape(2,1)
# ax+by+cz >= d
GE = np.array([1.0,1.0,1.0])
GE_b = np.array([61.0]).reshape(1,1)
# objective function coeffs
c = np.array([0.0, 0.0, 1.0])

x = cp.Variable((3,1),nonneg=True)
#Cost function
f = c@x
obj = cp.Minimize(f)
#Constraints
constraints = [EQ@x == EQ_b, GE@x >= GE_b]

#solution
prob = cp.Problem(obj, constraints)
prob.solve()
print("status:", prob.status)
print("optimal value:", f.value)
print("optimal var:", x.value.T)

#if using termux
#plt.savefig(os.path.join(script_dir, fig_relative))
#subprocess.run(shlex.split("termux-open "+os.path.join(script_dir, fig_relative)))
#else
#plt.show()
