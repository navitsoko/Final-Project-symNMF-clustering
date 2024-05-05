import math
import sys
import pandas as pd
import numpy as np
from numpy import random
import mysymnmfsp as f
np.random.seed(0)


def averegesM(M):
    num_cols = len(M[0])
    num_rows = len(M)
    sum = 0
    for i in range (num_rows):
        for j in range (num_cols):
            sum += M[i][j]
    return (sum/(num_cols*num_rows))


def outputer(vecs_arr):
    for vector in vecs_arr:
        for i in range(len(vector)):
            vector[i] = '{:.4f}'.format(vector[i])
        print(','.join(vector))


def main():
    dataframe = None
    data = None
    flag = 0
    args_lst = sys.argv
    if (len(args_lst) != 4 ):
        print("An Error Has Occurred")
        return 1
    arg_one = args_lst[1]
    if arg_one.isdigit():
        arg_one = int (arg_one)
    else:
        print("An Error Has Occurred")
        return 1
    arg_two = args_lst[2]
    if (arg_two == "symnmf"):
        flag = 1
    elif (arg_two == "sym"):
        flag = 2
    elif (arg_two == "ddg"):
        flag = 3
    elif (arg_two == "norm"):
        flag = 4
    else:
        print("An Error Has Occurred")
        return 1
    TXTfile = args_lst[3]
    if (TXTfile[-4:] == ".txt"):
        try:
            data = np.genfromtxt(TXTfile, dtype=float, encoding=None, delimiter=",")
        except:
            print("An Error Has Occurred")
            return 1
    else:
        print("An Error Has Occurred")
        return 1
    list = data.tolist()
    n = len(list)
    k = arg_one
    H = [[0.0 for i in range(k)] for j in range(n)]

    if (flag == 1):
        W = f.func(list,H,4,k)
        m = averegesM(W)
        max_val_interval = math.sqrt(m / k) * 2
        H_min = [0 for i in range(k)]
        H_max = [max_val_interval for i in range(k)]
        H = np.random.uniform(low=H_min, high=H_max, size=(n, k))
        H = H.tolist()
        X = f.func(list,H,1,k)
        outputer(X)
        return 0
    else:
        X = f.func(list, H, flag, k)
        outputer(X)
        return 0
    








if __name__ == "__main__":
    main()
