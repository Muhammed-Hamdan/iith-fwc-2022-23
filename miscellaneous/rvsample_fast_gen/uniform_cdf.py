import numpy as np
import matplotlib.pyplot as plt

file_name = "uni.dat"
num_bins = 100
num_samples = 1000000
rv_samples = np.fromfile(file_name, dtype=np.float32)
num_samples = rv_samples.shape[0]

rv_val = np.linspace(-4, 4, num_bins)
rv_dig = np.digitize(rv_samples, rv_val, right=True)
bin_index, rv_count = np.unique(rv_dig, return_counts=True)
rv_pmf = np.zeros((num_bins,))
for i in range(0, len(bin_index)):
    rv_pmf[bin_index[i]] = rv_count[i]
rv_pmf = rv_pmf/num_samples

rv_cdf = np.zeros((num_bins,))
rv_cdf[0] = rv_pmf[0]
for i in range(1, num_bins):
    rv_cdf[i] = rv_cdf[i-1] + rv_pmf[i]

plt.plot(rv_val, rv_cdf)
plt.grid()
plt.show()
