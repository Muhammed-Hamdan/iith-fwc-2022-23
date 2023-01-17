import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as scint

# Purpose:
# To compute the integral of message signal for use in FM equation using rectangle and trapezium method.

t_samp = np.arange(1000)
time_step = 1/1000000 # 1 us
fc = 50000  # Carrier freq
fm = 1000   # Message freq
fd = 20000  # freq deviation ie fc +- fd

# message signal: (discrete in time only, amplitude is continuous)
msg_dig = np.sin(2*np.pi*fm*t_samp*time_step)

# running integral of DIGITAL message signal (discrete in time and amplitude) using rectangle method  
msg_intr = np.cumsum((127*msg_dig).astype(np.int16))*time_step/127

# running integral (compute integral upto each sample instant) of message signal using trapezoidal method 
msg_intb = scint.cumulative_trapezoid(msg_dig, dx=time_step, initial=0)

# FM signal generation: fm(t) = sin(2*pi*fm*t + 2*pi*fd*integral(m(t)))
fm_digr = np.sin(2*np.pi*(fc*t_samp*time_step + fd*msg_intr))
fm_digb = np.sin(2*np.pi*(fc*t_samp*time_step + fd*msg_intb))
fm_anl = np.sin(2*np.pi*fc*t_samp*time_step - fd*np.cos(2*np.pi*fm*t_samp*time_step)/fm)


plt.plot(t_samp*time_step, msg_dig+6)
plt.plot(t_samp*time_step, fm_anl+4)
plt.plot(t_samp*time_step, fm_digr+2)
plt.plot(t_samp*time_step, fm_digb)
plt.grid()
plt.legend(["message","integral_equation", "integral_rect", "integral_trapz"])
plt.show()

# Result:
# When the sampling frequency of message signal is high enough, the rectangle method is very close to trapezium method 