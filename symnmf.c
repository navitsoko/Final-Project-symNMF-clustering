
# include "symnmf.h"
# include <stdio.h>
# include <stdlib.h>
#include <math.h>
#include <string.h>
struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

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
double ** ddg_c(double ** pointsArr, int n , int d);
void deallocate2DArray(double** arr, int rows);
double ** norm_c(double ** pointsArr, int n , int d );
double ** symnmf_c(double ** H ,  double ** W , int n, int k);
double ** optH( double ** H, double ** W, int n,int k,  int maxIter, double eps );


int main(int argc, char **argv) {
int i;
int j;
struct vector *head_vec, *curr_vec;
struct cord *head_cord, *curr_cord;
int flag=0;
double **M; 
double num;
int d=0;
int Pnum = 0;
double **arr;
char * goal;
int n;


FILE *file;

char * fileName = argv[2];
goal = argv[1];

    /*create new linked list*/
    head_cord = malloc(sizeof(struct cord));
    if (head_cord==NULL){
        printf("An Error Has Occurred\n");
        return 0;
    }

    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if (head_vec==NULL){
        printf("An Error Has Occurred\n");
        return 0;
    }
    curr_vec = head_vec;
    curr_vec->next = NULL;


    if (argc != 3 ){
        printf("An Error Has Occurred\n");
        return 0;
    }


    /*read points  from file to linked list */
    file = fopen(fileName, "r");

    if (file == NULL) {
        printf("An Error Has Occurred\n");
        return 0;
    }



   /*read every cordinate into num */

    while (fscanf(file, "%lf", &num) == 1) {

        if (flag == 0) {
            d++;
        }

      /*reach to end of line */
        if (( fgetc(file)) != ',') {
            flag = 1;
            Pnum++;
            curr_cord->value = num;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if (head_vec->next == NULL) {
                printf("An Error Has Occurred\n");
                return 0;
            }
            curr_vec = curr_vec->next;
            curr_vec->cords = NULL;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL) {
                printf("An Error Has Occurred\n");
                return 0;
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = num;
        curr_cord->next = malloc(sizeof(struct cord));
        if (curr_cord->next == NULL) {
            printf("An Error Has Occurred\n");
            return 0;
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;


    }
    fclose(file);

        /* change linked list of points to an array of points */

        arr = pointsToArr(Pnum, d, head_vec);
        if(arr == NULL){
            printf("An Error Has Occurred\n");
            return 0;
        }
        /*
        freeLinkedList(head_vec);
         */
        free(head_cord);
   
           /* read the value of goal */
             if (strcmp(goal, "sym") == 0){
                M = sym_c(arr,  Pnum,   d);
            }
            else if (strcmp(goal, "ddg") == 0) {
                M = ddg_c(arr, Pnum, d);
            }
            else if (strcmp(goal, "norm") == 0){
                M = norm_c(arr,Pnum , d );
            }
            else{
                return 0;
            }

    n=Pnum;
    /*
    while(M[i]!=NULL){
    n  = n  +1;
    i++;
    }
    */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%.4f",M[i][j]);
            if (j < n-1) {
                printf(",");
            }
        }
        printf("\n");
    }
    for (i = 0; i < Pnum; i++) {
        free(arr[i]);

    }
    free(arr);

        for (i = 0; i < n; i++) {
        free(M[i]);
    
       
        }
free(M);
    
return 0;
}

double ** symnmf_c(double ** H ,  double ** W , int n, int k){
    double eps = 1e-4;
    int maxIter = 300;


    H =  optH(   H,   W,   n,  k,  maxIter, eps);

return H;
}



/* input : vector and num of cord.
 * output : array of doubles in size d */
double  * vecToArr(struct vector *v1, int d){
    int i;
    struct cord *curCord = v1->cords;

    double *arr = (double *)calloc(d,sizeof (double ));

    if (arr==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for(i=0 ; i<d; i++){
        arr[i] = curCord->value;
        curCord  = curCord->next;
    }
    

    return arr;
}


/* input : linked list of points, num of points, dimention.
 * output : 2D array of points   d - dimentional */
double **pointsToArr(int Pnum, int d, struct vector *headVec){
    int j;
    struct vector * curVec;

    double **PointsArr = (double **)malloc(Pnum*sizeof (double* ));

    if (PointsArr==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }
    curVec = headVec;
    for(j=0;j<Pnum;j++){
        PointsArr[j] = vecToArr(curVec, d);
        if(PointsArr[j] == NULL){
            return NULL;
        }
        curVec = curVec->next;
    }
    freeLinkedList(headVec);
    return PointsArr;
}
/* given list of  n points returns metrix A    */
double ** sym_c(double ** pointsArr, int n, int d){
    int i;
    int j;
    int t;

    double ** A = (double **)calloc(n,sizeof (double* ));

    if (A==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for(t=0;t<n;t++) {
         A[t] = (double *) calloc(n, sizeof(double));
        if (A[t]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }


    for (i=0;i<n;i++){
       for (j=0;j<n;j++){
           if(i==j){
               A[i][i] = 0;
           }
           else{
               A[i][j] = exp( euclidian_dist(pointsArr[i],pointsArr[j], d) / -2);
           }
       }
    }

   return A;
}

/* given list of  n points  returns the degree matrix D  */
double ** ddg_c(double ** pointsArr, int n , int d){

    int i;

    double ** A = sym_c(pointsArr, n ,d);

    double ** D = (double **)calloc(n,sizeof (double* ));
    if (D==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for(i=0;i<n;i++) {
       D[i] = (double *) calloc(n, sizeof(double));
        if (D[i]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }

   for(i=0;i<n;i++){
       D[i][i] = sumVec(A[i], n);
           }
    for(i=0;i<n;i++){
       free(A[i]);
           }
    free(A);              
   return D;
}

/* given list of n points returns normalized matrix W   */
double ** norm_c(double ** pointsArr, int n , int d ) {

    int i;
    int j;
    double ** W;
    double ** newD;
    double  ** D;
    double **A;

   D = ddg_c(pointsArr, n ,d);

   newD = (double **)calloc(n,sizeof (double* ));
    if (newD==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for(i=0;i<n;i++) {
        newD[i] = (double *) calloc(n, sizeof(double));
        if (newD[i]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }

    A = sym_c( pointsArr, n, d);
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++){
            if (D[i][j] !=0 ){
                newD[i][j] = pow(D[i][j], -0.5);
            }
        }
    }
    W = matMul(matMul(newD, A, n, n,n),newD, n, n,n);
    
    for (i = 0; i < n; i++) {
        free(newD[i]);
        free(D[i]);
        free(A[i]);

    }
    free(newD);
    free(D);
    free(A);

    return W;

}
/* given matrixes A n1xk1 and B k1xk2 returns their multiplication. returns new mat C n1xk2   */
double ** matMul(double ** A, double ** B, int n1, int k1, int k2){
 int i;
 int j;
 int k;
 int t;
 /*double sum=0;*/
 double ** C = (double **)calloc(n1 ,sizeof (double* ));
    if (C==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for(t=0;t<n1;t++) {
        C[t] = (double *) calloc(k2, sizeof(double));
        if (C[t]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }

    for(i=0;i<n1;i++){
        for(j=0;j<k2;j++){
            C[i][j] = 0;
          for(k=0;k<k1;k++){
              C[i][j] += A[i][k] * B[k][j];
          }

        }

    }
    return C;

}

void deallocate2DArray(double **arr, int rows) {
        int i;
        if (arr == NULL) {
            return;
            for (i = 0; i < rows; i++) {
                free(arr[i]);
            }
            free(arr);
        }
}


/* given matrix  mat nxk returns new transposed matrix named matT kxn   */
double ** trans(double ** mat, int n, int k ) {
    int i;
    int j;
    int t;
    double **matT = (double **) calloc(k, sizeof(double *));
    if (matT==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for (t = 0; t < k; t++) {
        matT[t] = (double *) calloc(n, sizeof(double));
        if (matT==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < k; j++) {
            matT[j][i] = mat[i][j];
        }
    }

    return matT;
}


double ** optH( double ** H, double ** W, int n,int k,  int maxIter, double eps ){
    double b=0.5;
    int j;
    int i;
    int t;
    int iter=0;
    double **newH;

    double ** tempH = (double **) calloc(n, sizeof(double *));
    if (tempH==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for (t = 0; t < n; t++) {
        tempH[t] = (double *) calloc(k, sizeof(double));
        if (tempH[t]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }
    newH = (double **) calloc(n, sizeof(double *));
    if (newH==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for (t = 0; t < n; t++) {
        newH[t] = (double *) calloc(k, sizeof(double));
        if (newH[t]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }


    do{
        /* tempH = copy of H */
        for(i=0;i<n;i++){
            for (j=0;j<k;j++){
                tempH[i][j] = H[i][j];
            }
        }

        /* update H  */
        for(i=0;i<n;i++) {
            for (j = 0; j < k; j++) {

                newH[i][j] = ( H[i][j] *
                        (1 - b + ((b * matMul(W, H, n,n,k)[i][j]) / matMul(matMul(H, trans(H, n, k), n, k ,n), H, n,n,k)[i][j])));


            }
        }
        for(i=0;i<n;i++){
            for (j=0;j<k;j++){
                 H[i][j] = newH[i][j];
            }
        }
        iter++;

    }


    while(!epsilon_Finish( H, tempH, n, k, eps)  && iter < maxIter);


    for (i = 0; i < n; i++) {
        free(tempH[i]);
        free(newH[i]);
    }
    free(tempH);
    free(newH);
return   H;

}


/* given two matrixes , returns:
 * 1 : finish iterations
 * 0 :  continue */
int epsilon_Finish(double ** H1, double ** H0, int n,int k, double eps){
    double res=0;
    int i;

    double ** H = diff_two_matrix( H1,  H0,  n, k) ;
    res = Frob_norm( H,  n, k);

    if (res < eps){
        return 1;
    }
    for(i=0;i<n;i++){
        free(H[i]);
    }
    free(H);
    return 0;
}

/* given two matrixes , returns their difference:
 * res = || H1 - H0 ||
  */
double ** diff_two_matrix(double ** H1,double ** H0, int n, int  k){

    int t;
    int i;
    int j;
    double **res = (double **) calloc(n, sizeof(double *));
    if (res==NULL){
        printf("An Error Has Occurred\n");
        return NULL;
    }

    for (t = 0; t < n; t++) {
       res[t] = (double *) calloc(k, sizeof(double));
        if (res[t]==NULL){
            printf("An Error Has Occurred\n");
            return NULL;
        }
    }


    for (i = 0; i < n; i++) {
        for (j = 0; j <k; j++) {
            res[i][j] = H1[i][j] - H0[i][j];
        }
    }

    return res;
}


/* given matrix H size nxk , returns its Frobenius norm */
double Frob_norm(double ** H, int n , int k){
    double sum = 0;
    int i;
    int j;
    for ( i = 0; i <n; i++) {
        for ( j = 0; j < k; j++) {
            sum += (H[i][j] * H[i][j]);
        }
    }
 return sum;
}




/* given an array vector of doubles returns the sum of all cordinates  */
double sumVec(double * vec, int n){
    int i;
    double sum=0;
    for(i=0; i<n; i++ ){
        sum += vec[i];
    }
    return sum;
}

/* given two points of degree d returns the squared Euclidean distance */
    double euclidian_dist(double * arr1, double * arr2, int d){
        double sum = 0;
        int i;
        for (i=0; i<d; i++ ){
            sum += pow((arr1[i] - arr2[i] ), 2);
        }

        return sum;
    }




/* free allocated memory for linked list */
    void freeLinkedList(struct vector* head) {
        struct vector* outerCurrent = head;
        struct vector* outerNext;
        struct cord* currCord;
        struct cord* nextCord;


        while (outerCurrent != NULL) {
            currCord = outerCurrent->cords;

            while (currCord != NULL) {
                nextCord = currCord->next;
                free(currCord);
                currCord = nextCord;
            }

            outerNext = outerCurrent->next;
            free(outerCurrent);
            outerCurrent = outerNext;
        }
    }








