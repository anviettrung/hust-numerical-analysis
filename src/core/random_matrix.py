from random import randint, random
import numpy as np

def create_matrix(num_rows, num_cols, num_pivots):
    random_list = list(range(num_rows))
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
    U = np.random.randint(5, size=(m, m))
    V = np.random.randint(5, size=(n, n))
    P = create_matrix(m, n, num_pivots)
    return U @ P @ V

    