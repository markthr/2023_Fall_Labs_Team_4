import scipy as sp
import numpy as np
import scipy.optimize
import matplotlib.pyplot as plt

# Question 1
coeffs = np.array([[3, 1], [1, 2]])
values = np.array([9, 8])
soln = sp.linalg.solve(coeffs, values)
print("Solving System of Equations:")
var_names = ["x", "y"]

for i, row in enumerate(coeffs):
    print(' + '.join([f"{x}{y}" for (x,y) in zip(row, var_names)]), end='')
    print(f" = {values[i]}")

print(f"[x y] = {soln}")

# Question 2
opt_res = sp.optimize.minimize(lambda x: np.power(x,2) + 2*x, 0)
print("y(x) = x^2 + 2x")
print(f"y(x) minimized by x = {opt_res.x[0]:.4f}")

# Question 3
print("f(x) = sin(100*pi*x) + 1/2*sin(160*pi*x)")
f_x = lambda x: np.sin(100*np.pi*x) + np.sin(160*np.pi*x)/2
f_samp = 800
X = np.arange(0, 1, 1/f_samp)
F = np.linspace(0, 1, f_samp, endpoint=False)*f_samp
Y = sp.fft.fft(f_x(X))
(fig, ax) = plt.subplots()
ax.plot(F[:f_samp//2], 10*np.log10(np.abs(np.multiply(np.conj(Y[:f_samp//2]), Y[:f_samp//2]))))
ax.set_xlabel('frequency (Hz)')
ax.set_ylabel('dB')
plt.show(block=True)