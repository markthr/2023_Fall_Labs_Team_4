import numpy as np

a = np.array([1, 2, 3, 4])
print(a)

b = np.ones((3,4))
print(b)

c = np.zeros((4,3))
print(c)

A = np.array(([1, 2, 3],[4,5,6]))
print(A)
B = np.array(((1,2,3,4),(5,6,7,8),(9, 10, 11, 12)))
print(B)
print(A@B)

c = np.array(((3,1),(1,2)))
[D,V] = np.linalg.eig(c)
print(D)
print(V)
