import numpy as np
from scipy import linalg
from scipy.optimize import minimize
from scipy.fftpack import fft
import matplotlib.pyplot as plt

# Part 1
A = np.array([[3,1],[1,2]])
b = np.array([[9],[8]])

print(linalg.inv(A).dot(b))

# Part 2
def y(x):
    return x**2 + 2*x

x0 = np.array([0])
res = minimize(y,x0,options={'disp':True})
print(res.x)

# Part 3
def f(x):
    return np.sin(100*np.pi*x) + 0.5*np.sin(160*np.pi*x)

t = np.arange(0, 1, 0.00125)


plt.figure()

plt.plot(t,abs(fft(f(t))))
plt.show()




