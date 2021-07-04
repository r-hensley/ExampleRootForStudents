import numpy as np
from matplotlib import pyplot as plt

N = 1024
limit = 10

t = np.linspace(-limit, limit, N)
# array([-10.        ,  -9.98044966,  -9.96089932, ...,   9.96089932,
#          9.98044966,  10.        ])

dt = t[1] - t[0]  # 0.019550342130987275

y = np.sin(2 * np.pi * 5 * t) + np.sin(2 * np.pi * 1 * t)  # two sine frequencies of 5 Hz and 1 Hz

yhat = np.fft.fft(y)
Y = np.abs(yhat)

min_freq = 0
max_freq = (1.0/dt) / 2  # 25.575
freq = np.linspace(min_freq, max_freq, N//2)

plt.plot(freq, Y[0:N//2])  # unnormalized FFT
plt.plot(freq, Y[0:N//2] * (2/N))  # properly scaled FFT
plt.show()

