from sympy import *
x = Symbol('x')
# m is the vector perpendicular to normal chord ie m^tx = c
m = Matrix([-x, 1])
omat = Matrix(([0, -1], [1, 0]))
# n is the vector along the normal chord ie h + kn = x 
n = omat*m
# Conic parameters
V = Matrix(([Rational(1,24), 0],[0, Rational(-1,18)]))
u = Matrix([0, 0])
f = Matrix([-1])
# Point from which normal drawn
h = Matrix([0, 7*sqrt(3)])
# Equation solving
eq1 = n.T*((V*h + u)*(V*h + u).T - (h.T*V*h + 2*u.T*h + f)[0,0]*V)*n
eq2 = (m.T*V*n)**2
eq3 = ((V*h + u).T*(n*(m.T*V*n) - m*(n.T*V*n)))**2
eq = eq1[0,0]*eq2[0,0] - eq3[0,0]
print(expand(eq))
print(solveset(eq, x))
