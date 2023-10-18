import numpy as np

# Question 3
arr_one = np.array([1,2,3,4])
print(f"First Array: {arr_one}")

# Question 4
mat_ones = np.ones((3,4))
print(f"Ones Matrix: \n{mat_ones}")

# Question 5
mat_A = np.reshape(np.array([1, 0, 2, 3, -1, 6]), (2,3))
mat_B = np.reshape(np.array([-5, 3, 0, 0, 7, 0, -1, 1, 4, 0, 1, -2]), (3,4))
mat_AB = np.matmul(mat_A, mat_B)
print("Matrix Multiplication:")
print(f"A = \n{mat_A}")
print(f"B = \n{mat_B}")
print(f"AB = \n{mat_AB}")

# Question 6
mat_C = np.reshape(np.array([3, 1, 1, 2]), (2, 2))
(eigenvalues, eigenvectors) = np.linalg.eig(mat_C)
print("Findin Eigenpairs:")
print(f"C = \n{mat_C}")
print(f"lambda = \n{eigenvalues}")
print(f"ksi = \n{eigenvectors}")