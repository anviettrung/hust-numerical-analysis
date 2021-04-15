from random import randint, random
import numpy as np

def create_matrix(num_rows, num_cols, num_pivots):
    A = np.zeros((num_rows, num_cols), dtype=int)
    
    list_rows = list(range(len(A)))
    list_cols = list(range(len(A[0])))
    for _ in range(num_pivots):
        # random row and col
        i = randint(0, len(list_rows) - 1)
        j = randint(0, len(list_cols) - 1)
        random_row = list_rows[i]
        random_col = list_cols[j]
        
        # remove from list rows and list cols
        list_rows.pop(i)
        list_cols.pop(j)
        
        # print(f"list cols: {list_cols}")
        # print(f"list rows: {list_rows}")
        
        A[random_row][random_col] = 1
    return A 


def random_matrix(num_rows, num_cols, num_pivots):
    m = num_rows
    n = num_cols
    U = np.random.randint(3, size=(m, m))
    V = np.random.randint(3, size=(n, n))
    P = create_matrix(m, n, num_pivots)
    return U @ P @ V


def save(matrix):
    print(len(matrix[0]), end=" ")
    print(len(matrix))
    for row in matrix:
        for e in row:
            print(e, end=" ")
        print()


if __name__=='__main__':
    num_rows = 10
    num_cols = 25
    num_pivots = 10 
    
    X = random_matrix(num_rows, num_cols, num_pivots)
    # print(len(X[0]), end=" ")
    # print(len(X))
    # for row in X:
    #     for e in row:
    #         print(e, end=" ")
    #     print()
    
    A = X[:, :-5]
    save(A)
    
    print()
    B = X[:, -5:]
    save(B)