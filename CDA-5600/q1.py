import numpy as np
from numpy.linalg import eig


def mutually_unbiased(vec1, vec2):
    # assertions on input
    shape1 = np.shape(vec1)
    shape2 = np.shape(vec2)

    if shape1 != shape2:
        return False
    if shape1[0] != shape1[1]:
        return False

    # get D
    dimension = shape1[0]
    # for each basis vector in vector1
    for i in range(0,dimension):
        # for each basis vector in vector2
        for j in range(0, dimension):
            e1 = vec1[i]
            e2 = vec2[j]
            s = np.square(np.absolute(np.matmul(e1, np.transpose(e2))))
            # rounded since the values were not exact 0.5 ~ 0.499999
            if np.round(s.item(0, 0), decimals=4) != (1/dimension):
                #print(s.item(0, 0))
                return False
    return True


pauliX = np.mat('[0 1;1 0]')
pauliY = np.mat('[0 -1j;1j 0]')
pauliZ = np.mat('[1 0;0 -1]')

valsX, vecsX = eig(pauliX)
valsY, vecsY = eig(pauliY)
valsZ, vecsZ = eig(pauliZ)


print(mutually_unbiased(vecsX, vecsY))
print(mutually_unbiased(vecsX, vecsZ))
print(mutually_unbiased(vecsY, vecsZ))
