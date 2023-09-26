import scipy
import numpy as np
import matplotlib.pyplot as plt

# Part 1
fs, data = scipy.io.wavfile.read('Tutorial_3/Cafe_with_noise.wav')
print(len(data))

plt.figure(1)
plt.plot(np.linspace(0,1/fs*(len(data)-1),len(data)),data)
plt.show()

# Part 2
plt.figure(2)
plt.plot(np.linspace(0,fs,len(data)),abs(scipy.fftpack.fft(data)))
plt.show()

# Part 3
b, a = scipy.signal.butter(10, 800, 'low', analog=True)
w, h = scipy.signal.freqs(b, a)
filter = scipy.signal.butter(10,800,'low',fs=fs,output='sos')

filtered = scipy.signal.sosfilt(filter, data)
plt.figure(3)
plt.plot(np.linspace(0,fs,len(data)),abs(scipy.fftpack.fft(filtered)))
plt.show()

plt.figure(4)
plt.plot(np.linspace(0,1/fs*(len(data)-1),len(data)),filtered)
plt.show()
