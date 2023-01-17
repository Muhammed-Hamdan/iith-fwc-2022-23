import numpy as np
import matplotlib.pyplot as plt

num_pts = 20
n_axis = np.arange(num_pts)
x_n = np.zeros(num_pts)

x_n[0] = 0
x_n[1] = 1
for i in range(2, num_pts):
    x_n[i] = x_n[i-1] + x_n[i-2]

plt.stem(n_axis, x_n)
plt.show()

plt.close()
x_n_plus_1 = np.roll(x_n, -1); x_n_plus_1[-1] = x_n_plus_1[-2] + x_n_plus_1[-3]
x_n_minus_1 = np.roll(x_n, 1); x_n_minus_1[0] = 0
y_n = x_n_minus_1 + x_n_plus_1
plt.stem(n_axis, y_n)
plt.show()