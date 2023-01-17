import numpy as np
import matplotlib.pyplot as plt
import scipy.fft as scifft

# Purpose:
# To compare the LUT (Look Up Table) method of calculating sin(theta) versus inbuilt sin function for FM generation

# Sine LUT generation 
lut_pow = 15
lut_N = 2**lut_pow
dac_peak = 128
sin_lut = (dac_peak*np.sin(2*np.pi*np.arange(lut_N)/lut_N)).astype(np.int16)

# Message generation
msg_N = 100         # Number of message samples 
msg_peak = 128
msg_samp = (msg_peak*np.sin(2*np.pi*np.arange(msg_N)/msg_N)).astype(np.int16)

# Reassign some variables to follow formula convention 
scale = msg_peak    
N = lut_N         

# FM Parameters
fm = 50000          # Message rate
fs = 1000000        # Sampling rate
fc = 100000         # Carrier freq  (try 100kHz)
fd = 50000          # Frequency deviation (try 50kHz)

# Precompute some constants k1 and k2 to reduce number of calculations 
k1 = fc/fs
k2 = fd/fs

'''
Since DAC sample rate higher than message rate, message has to be oversampled fs/fm times
i.e. generate fs/fm FM signal samples for every message signal sample
'''
over_samp = int(fs/fm)   
# FM signal buffer calculated using LUT
fm_buff_opt = np.zeros(msg_N*over_samp)
# FM signal buffer calculated using sin function
fm_buff_nav = np.zeros(msg_N*over_samp)

# Integral calculated using rectangles => integral = (sum of message samples upto time t)*sample_rate
running_sum = 0

for i in range(0,msg_N):
    shift = over_samp*i
    for j in range(0,over_samp):
        n1 = k1*(shift+j)
        n2 = k2*running_sum/scale
        running_sum += msg_samp[i]
        n = int((n1+n2-int(n1+n2))*N)
        fm_buff_opt[shift+j] = sin_lut[n]
        fm_buff_nav[shift+j] = 128*np.sin(2*np.pi*(n1+n2))

t = np.arange(0, msg_N*over_samp)/fs
plt.step(t, fm_buff_opt)
plt.step(t, fm_buff_nav)
plt.step(np.arange(msg_N)/fm, msg_samp)
plt.show()

plt.close()
# Plotting frequency spectrum
num_samp = msg_N*over_samp
f_msg = fs/num_samp
bw_half = f_msg + fd
yf = scifft.rfft(fm_buff_opt)
xf = scifft.rfftfreq(num_samp, 1/fs)
plt.plot(xf, np.abs(yf))
# Lines to show frequency deviation
plt.axvline(x = fc-fd, color = 'r', label = 'axvline - full height')
plt.axvline(x = fc+fd, color = 'r', label = 'axvline - full height')
# Lines to show bandwidth
plt.axvline(x = fc-bw_half, color = 'g', label = 'axvline - full height')
plt.axvline(x = fc+bw_half, color = 'g', label = 'axvline - full height')
plt.show()

# Result:
# The LUT method produces almost the same result as calling the sin function.
# But it is computed much faster when done in C language 