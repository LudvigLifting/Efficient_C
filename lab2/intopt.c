#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Simplex_t Simplex_t;

const double epsilon = 1e-6;
double glob = 0;

struct Simplex_t{
    int m;/* Constraints. */
    int n;/* Decision variables. */
    int* var;  /*0..n1are nonbasic. */
    double** a;    /*A. */
    double* b;/*b. */
    double* x;/*x. */
    double* c;/*c. */
    double y;/* y. */
};

int init(Simplex_t* params){
    printf("%lf",epsilon);
    int k = 0;
    scanf("%d %d", &(*params).m, &(*params).n);

    (*params).var = calloc((*params).m + (*params).n + 1, sizeof(int));
    (*params).b = calloc((*params).m, sizeof(double));
    (*params).x = calloc((*params).n + 1, sizeof(double));
    (*params).c = calloc((*params).n, sizeof(double));
    (*params).a = calloc((*params).m, sizeof(double*));
    for(int i = 0; i < (*params).m; i++){
        (*params).a[i] = calloc((*params).n + 1, sizeof(double));
    }

    for(int i = 0; i < (*params).n; i++){
        scanf("%lf", &(*params).c[i]);
    }
    for(int i = 0; i < (*params).m; i++){
        for(int j = 0; j < (*params).n; j++){
            scanf("%lf", &(*params).a[i][j]);
        }
    }
    for(int i = 0; i < (*params).m; i++){
        scanf("%lf", &(*params).b[i]);
        glob += 1;
    }

    for(int i = 0; i < (*params).m + (*params).n; i++){
        (*params).var[i] = i;
    }

    for(int i = 0; i < (*params).m; i++){
        if((*params).b[i] < (*params).b[k]){
            k = i;
        }
    }
    return k;
}

void print_params(Simplex_t* params){
    printf("m = %d, n = %d\n", (*params).m, (*params).n);

    printf("a =\n[");
    for(int i = 0; i < (*params).m; i++){
        printf("[");
        for(int j = 0; j < (*params).n-1; j++){
            printf("%10.3lf, ", (*params).a[i][j]);
        }
        if(i == (*params).m-1){
            printf("%10.3lf]", (*params).a[i][(*params).n-1]);
        }
        else{
            printf("%10.3lf],\n", (*params).a[i][(*params).n-1]);
        }
    }
    printf("]\n");

    printf("b = [");
    for(int i = 0; i < (*params).m; i++){
        if(i == (*params).m-1){
            printf("%10.3lf", (*params).b[i]);
        }else{
            printf("%10.3lf, ", (*params).b[i]);
        }
    }
    printf("]\n");

    printf("c = [");
    for(int i = 0; i < (*params).n; i++){
        if(i == (*params).n-1){
            printf("%10.3lf", (*params).c[i]);
        }else{
            printf("%10.3lf, ", (*params).c[i]);
        }
    }
    printf("]\n");
}

void free_params(Simplex_t* params){
    for(int i = 0; i < (*params).m; i++){
        free((*params).a[i]);
    }
    free((*params).a);
    free((*params).b);
    free((*params).c);
    free(params);
}

int select_nonbasic(Simplex_t* params){
    for(int i = 0; i < (*params).n; i++){
        if((*params).c[i] > epsilon){
            return i;
        }
    }
    return -1;
}

void pivot(Simplex_t* params, int row, int col){
    double** a = (*params).a;
    double* b = (*params).b;
    double* c = (*params).c;
    int m = (*params).m;
    int n = (*params).n;

    int t = (*params).var[col];
    (*params).var[col] = (*params).var[n + row];
    (*params).var[n + row] = t;
    (*params).y += (c[col] * b[row]) / a[row][col];
    
    for(int i = 0; i < n; i++){
        if(i != col){
            c[i] += -(c[col] * a[row][i]) / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];

    for(int i = 0; i < m; i++){
        if(i != row){
            b[i] += -(a[i][col] * b[row]) / a[row][col];
        }
    }

    for(int i = 0; i < m; i++){
        if(i != row){
            for(int j = 0; j < n; j++){
                if(j != col){
                    a[i][j] += -(a[i][col] * a[row][j]) / a[row][col]; 
                }
            }
        }
    }

    for(int i = 0; i < m; i++){
        if(i != row){
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    for(int i = 0; i < n; i++){
        if(i != col){
            a[row][i] = a[row][i] / a[row][col];
        }
    }
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
}

double xsimplex(Simplex_t* params, int h){
    int col, row;
    while((col = select_nonbasic(params)) >= 0){
        row = -1;
        for(int i = 0; i < (*params).m; i++){
            if((*params).a[i][col] > epsilon && 
                (row < 0 || ((*params).b[i] / (*params).a[i][col]) < ((*params).b[row] / (*params).a[row][col]))){
                row = i;
            }
        }
        if(row < 0){
            free((*params).var);
            return INFINITY;
        }
        pivot(params, row, col);
    }
    if(h == 0){
        for(int i = 0; i < (*params).n; i++){
            if((*params).var[i] < (*params).n){
                (*params).x[(*params).var[i]] = 0;
            }
        }
        for(int i = 0; i < (*params).m; i++){
            if((*params).var[(*params).n + i] < (*params).n){
                (*params).x[(*params).var[(*params).n + i]] = (*params).b[i - (*params).n];
            }
        }
        free((*params).var);
    }
    else{
        for(int i = 0; i < (*params).n; i++){
            (*params).x[i] = 0;
        }
        for(int i = (*params).n; i < (*params).n + (*params).m; i++){
            (*params).x[i] = (*params).b[i - (*params).n];
        }
    }
    return (*params).y;
}

double simplex(Simplex_t* params){
    return xsimplex(params, 0);
}

//Optimal value 16
int main()
{
    Simplex_t* params = calloc(1, sizeof(*params));
    init(params);
    print_params(params);
    double result = simplex(params);
    //double result = 0;

    if(result != INFINITY){
        printf("Solution found, with z = %lf\n", result);
    }
    else{
        printf("Solution not found..\n");
    }
    free_params(params);
    return 0;
}