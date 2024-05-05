# Final Project symNMF clustering
Implementation of  a clustering algorithm that is based on symmetric Non-negative Matrix Factorization (symNMF).
Appplying it to several datasets and comparing to K-means, using Python C API wrapper.
## k-means
The K-means algorithm is a popular clustering method for finding a partition of N unlabeled observations into K distinct clusters, where K is a parameter of the method.

Given a set of N datapoints x1, x2, . . . , xN ∈ Rd , the goal is to group the data into K ∈ N clusters, each datapoint is assigned to exactly one cluster and the number of clusters K is such that 1 < K < N. Each cluster k is represented by it’s centroid which is the mean µk ∈ Rd of the cluster’s members.

 ## SymNMF Algorithm
1. Form the similarity matrix A from X.
2. Compute the Diagonal Degree Matrix.
3. Compute the normalized similarity W.
4. Find H<sub>n×k</sub> that solves: min <sub>H≥0</sub>||W − HH<sup>T</sup>||<sup>2</sup> <sub>F</sub> .

Where k is a parameter denoting the required number of clusters and the norm is the squared Frobenius norm.

## Description
1. symnmf.py: Python interface of the code.
2. symnmf.h: C header file.
3. symnmf.c: C interface of the code.
4. symnmfmodule.c: Python C API wrapper.
5. analysis.py: Analyze the algorithm.
6. setup.py: The setup file.
7. Makefile: The make script to build the C interface.

### Python Program (symnmf.py)
1. Reading user CMD arguments.
2. Implementation of H initialization when the goal=symnmf.
3. Interfacing with the  C extension:
(a) Import C module symnmf
(b) if the goal=’symnmf’, call the symnmf() method with passing the initial H, the W and
other arguments if needed, and get the final H.
(c) if the goal=’sym’, call the sym() method with passing the datapoints X, and get
similarity matrix.
(d) if the goal=’ddg’, call the ddg() method with passing the datapoints X, and get diagonal degree matrix.
(e) if the goal=’norm’, call the norm() method with passing the datapoints X, and get
normalized similarity matrix.
4. Output the required matrix separated by a comma, such that each row is in a line of its
own.

For example run:  `python3 symnmf.py 2 symnmf input_1.txt`.
### C Program (symnmf.c)
This is the C implementation program, with the following requirements:
1. Reading user CMD arguments.
2. Output the required matrix separated by a comma, such that each row is in a line of its
own.
 To compile run : `make`.
After successful compilation the program can run for Example:
`./symnmf sym input_1.txt`.

### The Python C API (symnmfmodule.c)
In this file we define the C extension which will serve the functions: symnmf,sym,ddg,norm for Python.

### C Header file (symnmf.h)
This header defines all functions prototypes that is being used in symnmfmodule.c and
implemented at symnmf.c.

### analysis.py
Compare SymNMF to Kmeans. Apply both methods to given dataset and report
the silhouette_score from the sklearn.metrics.
The silhouette score measures clustering quality by comparing the between-cluster distance
against within-cluster distance. A higher score indicates better-defined clusters.

For example run : `python3 analysis.py 5 input_k5_d7.txt`.

### Setup (setup.py)
This is the build used to create the *.so file that will allow symnmf.py to import symnmfmodule.

### Makefile
Make script for building symnmf executable, considering all it’s dependency. The compilation
command should include all the flags: `gcc -ansi -Wall -Wextra -Werror -pedantic-errors`.

### Build and Running
The extension must build cleanly (no errors, no warnings) when running the following
command: `python3 setup.py build_ext --inplace`.
After successful build, the program can run.

Don’t compile the C module with gcc.



 
