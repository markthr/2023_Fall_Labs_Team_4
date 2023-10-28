import matplotlib.pyplot as plt
import numpy as np

# Question 1
(fig, ax) = plt.subplots()
omega = np.linspace(0, 2*np.pi, 250)
ax.plot(omega, np.sin(omega))
# Question 2
ax.set_xlabel('$\omega$')
ax.set_ylabel('$sin(\omega)$')
plt.show(block=False)


# Question 3
X = np.linspace(-10, 10, 100)
Y = np.linspace(-10, 10, 100)
X, Y = np.meshgrid(X, Y)
Z = np.sin(np.sqrt(np.power(X, 2) + np.power(Y, 2)))
fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
ax.plot_surface(X, Y, Z)
plt.show(block=True)