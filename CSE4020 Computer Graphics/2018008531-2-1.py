import numpy as np

#A
M = np.arange(27)
M = M[2:27]
print(M)
print()

#B
M = M.reshape(5,5)
print(M)
print()

#C
M[:,0] = 0
print(M)
print()

#D
M = M@M
print(M)
print()

#E
v = M[0,:]
tmp = v*v
sum = 0
for i in range(0,5):
    sum = sum + tmp[i]
print(np.sqrt(sum))