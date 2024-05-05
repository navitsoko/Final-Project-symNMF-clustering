import math
import sys
import pandas as pd
import numpy as np
import random
import mysymnmfsp as f
from sklearn.metrics import silhouette_score
np.random.seed(0)



def vecExtractor(input):
    with open(input, "r") as file:
        vectors = []
        for line in file:
            if len(line) > 1:
                vectors.append([float(coordinate) for coordinate in line[:len(line)].split(',')])
        return vectors


def euclidean_dist(vec1, vec2):
    d = len(vec1)
    dist = 0
    for i in range(d):
        a = vec1[i]
        b = vec2[i]
        dist += ((vec1[i]-vec2[i])**2)
    dist = (math.sqrt(dist))
    return dist


def Assigncents(clust, vec, k):
    for i in range(k):
        clust.append([vec[i]])


def cent_maker(vec, k):
    centroids = []
    for i in range(k):
        centroids.append(vec[i])
    return centroids


def Assignvectors(clusts_arr, vec, centroids, index):
    for i in range(len(vec)):
        first_dist = euclidean_dist(vec[i], centroids[0])
        closest = 0
        for j in range(1, len(centroids)):
            curr_dist = euclidean_dist(vec[i], centroids[j])
            if curr_dist < first_dist:
                first_dist = curr_dist
                closest = j
        clusts_arr[closest].append(vec[i])
        index[i] = closest
            

def Assignvectors2(vectors, centroids, k, index):
    new_cluster = [[] for i in range(k)]
    cnt = 0
    for i in vectors:
        dis_from_first_cent = euclidean_dist(i, centroids[0])
        closest_cent = 0
        for j in range(1, len(centroids)):
            dis_from_curr_cent = euclidean_dist(i, centroids[j])
            if dis_from_curr_cent < dis_from_first_cent:
                dis_from_first_cent = dis_from_curr_cent
                closest_cent = j
        new_cluster[closest_cent].append(i)
        index[cnt] = closest_cent
        cnt += 1
    return new_cluster


def new_cents(cluster):
    centroids_new = [0 for i in range(len(cluster[0]))]
    for i in cluster:
        for j in range(len(cluster[0])):
            centroids_new[j] += i[j]
    for p in range(len(centroids_new)):
        c = centroids_new[p]
        d = len(cluster)
        centroids_new[p] = ((centroids_new[p]) / len(cluster))
        t = centroids_new[p]
        f = centroids_new[p]
    return centroids_new


def adjust_centroids(clusters, centroids, k):
    delta = [0 for i in range(k)]
    for i in range(k):
        new_cent = new_cents(clusters[i])
        delta[i] = euclidean_dist(centroids[i], new_cent)
        centroids[i] = new_cent
    return delta


def check_centroids(centroids):
    for i in centroids:
        if i >= 0.0001:
            return False
    return True


def print_centroids(centroids, d):
    for vector in centroids:
        centroid_len = d
        for i in range(centroid_len):
            if centroid_len > 1:
                print('{:.4f}'.format(vector[i]) + ",", end="")
                centroid_len -= 1
            else:
                print('{:.4f}'.format(vector[i]) + '\n', end="")


def k_means(K, iters, input_data):
    vectors = vecExtractor(input_data)
    index = [0 for i in range(len(vectors))]
    iter_num = 0
    k1 = int(K)
    centroids = cent_maker(vectors, k1)
    clusters = [[] for i in range(k1)]
    Assignvectors(clusters, vectors, centroids, index)
    deltas = adjust_centroids(clusters, centroids, k1)
    check = check_centroids(deltas)
    if check:
        for i in centroids:
            return centroids, index, vectors
    while iter_num < iters:
        clusters = Assignvectors2(vectors, centroids, k1, index)
        deltas = adjust_centroids(clusters, centroids, k1)
        check = check_centroids(deltas)
        if check:
            return centroids, index, vectors
        iter_num += 1
    return centroids, index, vectors


def table_creator(input_file1):
    df1 = pd.read_csv(input_file1, header=None)
    return df1


def mat_creator(vectors):
    vectors_mat = []
    for i in range(vectors.shape[0]):
        curr_vec = vectors.iloc[i, 0:].tolist()
        vectors_mat.append(curr_vec)
    return vectors_mat


def main_func(k, file2):
    epsilon = 0.0001
    max_iter = 300
    help_table = table_creator(file2)
    N = help_table.shape[0]
    H = [[0 for j in range(k)] for i in range(N)]
    vec_len = help_table.shape[1]
    vec_mat = mat_creator(help_table)
    norm_mat = f.func(vec_mat,H,4, k)
    m = 0
    for i in range(N):
        for j in range(N):
            m += norm_mat[i][j]
    m = m/(N*N)
    for i in range(N):
        for j in range(k):
            H[i][j] = np.random.uniform(0, 2*(math.sqrt(m/k)))
    symnmf_answer = f.func(vec_mat,H,1, k)
    
    
    npH = np.array(symnmf_answer)
    symnmf_array = np.argmax(npH, axis=1)
    kmeans_table, kmeans_cluster, vectors = k_means(k, 300, file2)
    symnmf_score = "{:.4f}".format(silhouette_score(vectors, symnmf_array))
    kmeans_score = "{:.4f}".format(silhouette_score(vectors, kmeans_cluster))
    print("nmf:", symnmf_score)
    print("kmeans:", kmeans_score)
    
    
args = sys.argv
args_len = len(args)
k1 = 0
file1 = None
if args_len != 3:
    print("An Error Has Occurred")
    exit()
try:
    k1 = int(args[1])
    file1 = args[2]
except Exception:
    print("An Error Has Occurred kkk")
    exit()

main_func(k1, file1)
