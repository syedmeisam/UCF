import numpy as np
import sys

def are_orthogonal(X, Z):
    shape1 = np.shape(X)
    shape2 = np.shape(Z)

    if shape1 != shape2:
        return False
    if shape1[0] != shape1[1]:
        return False

    # get D
    d = shape1[0]

    for i in range(0, d):
        for j in range(0, d):
            if (i == j):
                continue
            x = X[i]
            z = Z[j]
            x = np.transpose(x)        # take conjugate transpose of x
            inner_product = np.matmul(x,z)
            if inner_product != 0:
                return False

    return True

def inner_product(dimension, a, b):
    # a and b are the powers of the matrices, these are important because X is dependent on a
    print("Dimension: %d" % (dimension) )
    Z = np.identity(dimension)
    X = np.identity(dimension)
    Z = np.power(Z, b)

    ket = [0]*(dimension)
    for i in range(0, dimension):
        ket[i] = (a+i) % dimension

    #print(ket)
    print(are_orthogonal(X, Z))

def main(dimension):
    inner_product(dimension, 5, 1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python q3.py dimension")
        exit(0)
    main(int(sys.argv[1]))
