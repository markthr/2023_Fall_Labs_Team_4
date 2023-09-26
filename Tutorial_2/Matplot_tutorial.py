import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np


def f(t):
    return np.sin(t)

t = np.arange(0.0, 2*np.pi, 0.01)
plt.figure(1)
plt.plot(t,f(t))
plt.xlabel('time')
plt.ylabel('sine wave')
plt.show()

def f(x, y):
    return np.sin(np.sqrt(x ** 2 + y ** 2))

x = np.linspace(-6, 6, 30)
y = np.linspace(-6, 6, 30)

X, Y = np.meshgrid(x, y)
Z = f(X, Y)

fig = plt.figure(2)
ax = plt.axes(projection='3d')
ax.contour3D(X, Y, Z, 50, cmap='binary')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()