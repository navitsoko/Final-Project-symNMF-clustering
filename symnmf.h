#ifndef SYMNMF_H
#define SYMNMF_H

struct cord;
struct vector;
double  sumVec(double * vec, int n);
double euclidian_dist(double * arr1, double * arr2, int d);
double ** matMul(double ** A, double ** B, int n1, int k1 , int k2);
double ** trans(double ** mat, int n, int k );
double ** diff_two_matrix(double ** H1,double ** H0, int n, int k);
double Frob_norm(double ** H, int n, int k);
int epsilon_Finish(double ** H1, double ** H0, int n,int k, double eps);
double **pointsToArr(int Pnum, int d, struct vector *headVec);
double  * vecToArr(struct vector *v1, int d);
void freeLinkedList(struct vector* head);
double ** sym_c(double ** pointsArr, int n, int d);
void deallocate2DArray(double** arr, int rows);
double ** ddg_c(double ** pointsArr, int n , int d);
double ** norm_c(double ** pointsArr, int n , int d );
double ** symnmf_c(double ** H ,  double ** W , int n, int k);
double ** optH( double ** H, double ** W, int n,int k,  int maxIter, double eps );

#endif
