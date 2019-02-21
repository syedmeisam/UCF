import numpy as np
import sys

def contruct(d):
    pass


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
        x = X[i]
        z = Z[i]
        x = x.getH()        # take conjugate transpose of x
        inner_product = np.matmul(x,z)
        if inner_product != 0:
            return False

    return True

def main(dimension):
    print("Dimension: %d" % (dimension) )
    pass

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python q3.py dimension")
        exit(0)
    main(int(sys.argv[1]))
