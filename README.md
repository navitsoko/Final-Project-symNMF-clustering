# Final-Project-symNMF-clustering
Implementation of  a clustering algorithm that is based on symmetric Non-negative Matrix Factorization (symNMF).
Appplying it to several datasets and comparing to K-means, using Python C API wrapper.
## k-means
The K-means algorithm is a popular clustering method for finding a partition of N unlabeled observations into K distinct clusters, where K is a parameter of the method.

Given a set of N datapoints x1, x2, . . . , xN ∈ Rd , the goal is to group the data into K ∈ N clusters, each datapoint is assigned to exactly one cluster and the number of clusters K is such that 1 < K < N. Each cluster k is represented by it’s centroid which is the mean µk ∈ Rd of the cluster’s members.

 ## SymNMF Algorithm
1: Form the similarity matrix A from X.
2: Compute the Diagonal Degree Matrix.
3: Compute the normalized similarity W.
4: Find Hn×k that solves: min <sub>H≥0</sub>||W − HH<sup>T</sup>||<sup>2</sup> <sub>F</sub> .

 
