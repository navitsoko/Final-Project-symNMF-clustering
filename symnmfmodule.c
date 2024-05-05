#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

static PyObject* func(PyObject *self, PyObject *args) {
    PyObject *point_list;
    PyObject *H_C;
    PyObject *curr_vec;
    int d;
    int N; 
    int k;
    int flag;
    if (!PyArg_ParseTuple(args, "OOii", &point_list, &H_C, &flag,&k)) {
        Py_RETURN_NONE;
    }
    N = PyObject_Length(point_list);
    d = PyObject_Length(PyList_GetItem(point_list, 0));
    int i;
    int j;
    double **point_arr_c = (double**)calloc(N,sizeof (double*));
    double **W;
    double **H = (double**)calloc(N,sizeof (double*));
    for( i = 0; i < N; i++){
        point_arr_c[i] = (double *) calloc(d, sizeof(double));
        curr_vec = PyList_GetItem(point_list, i);
        for(j = 0; j < d; j++){
            point_arr_c[i][j] = PyFloat_AsDouble(PyList_GetItem(curr_vec, j));
        }
    }
    for( i = 0; i < N; i++){
        H[i] = (double *) calloc(k, sizeof(double));
        curr_vec = PyList_GetItem(H_C, i);
        for(j = 0; j < k; j++){
            H[i][j] = PyFloat_AsDouble(PyList_GetItem(curr_vec, j));
        }
    }

    if (flag == 1){
        W = (double**)calloc(N,sizeof (double*));
        W = norm_c(point_arr_c,N,d);
        H = symnmf_c(H,W,N,k);
    }
    else if (flag == 2){
        point_arr_c = sym_c(point_arr_c,N,d);
    }
    else if (flag == 3){
        point_arr_c = ddg_c(point_arr_c,N,d);
    }
    else {
        point_arr_c = norm_c(point_arr_c,N,d);
    }
    PyObject* answer_py = PyList_New(N);
    if (flag == 1){
        for (i = 0; i < N; i++){
            PyObject* lst = PyList_New(k);
            for(j = 0; j < k; j++){
                PyList_SetItem(lst, j, Py_BuildValue("d", H[i][j]));
            }
            PyList_SetItem(answer_py, i, lst);
        }
    }
    else {
        for (i = 0; i < N; i++){
            PyObject* lst = PyList_New(N);
            for(j = 0; j < N; j++){
                PyList_SetItem(lst, j, Py_BuildValue("d", point_arr_c[i][j]));
            }
            PyList_SetItem(answer_py, i, lst);
        }
    }
  for(i=0;i<N;i++){
    free(point_arr_c[i]);
    free(H[i]);
  }  
  free(point_arr_c);
  free(H);
    return answer_py;
}

static PyMethodDef symnmfMethods[]={
        {"func",
                (PyCFunction) func,
                   METH_VARARGS,
                     PyDoc_STR("The argument order:\n\n\
                                \t^X=matrixX\n\
                            \tarray of array of doubles\n\
                            \n\
                            \t^H=matrixH\n\
                            \tin which array of arrays of doubles too\n\
                            \n\
                             t^flag = detetmine execution func\n\
                            \n\
                            \t^k = num of clusters required")
        },
        {NULL,NULL,0,NULL}
};

static struct PyModuleDef symnmfModule = {
        PyModuleDef_HEAD_INIT,
        "mysymnmfsp",
        NULL,
        -1,
        symnmfMethods
};

PyMODINIT_FUNC
PyInit_mysymnmfsp(void)
{
    PyObject *m;
    m=PyModule_Create(&symnmfModule);
    if(!m)
    {
        Py_RETURN_NONE;
    }
    return m;
}
