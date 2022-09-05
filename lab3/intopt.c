#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Simplex_t Simplex_t;
int init(Simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var);
int select_nonbasic(Simplex_t* s);
void prepare(Simplex_t* s, int k);
int initial(Simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var);
void pivot(Simplex_t* s, int row, int col);
double xsimplex(int m, int n, double** a, double* b, double* c, double* x , double y, int* var, int h);
double simplex(int m, int n, double** a, double* b, double* c, double* x , double y);

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

int init(Simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var){
    int i, k;
    s->a = a;
    s->b = b;
    s->c = c;
    s->m = m;
    s->n = n;
    s->var = var;
    s->x = x;
    s->y = y;

    if(s->var == NULL){
        s->var = calloc(s->m + s->n + 1, sizeof(int));
        for(i = 0; i < s->m + s->n; i++){
            s->var[i] = i;
        }
    }

    for(k = 0, i = 1; i < m; i++){
        if(b[i] < b[k]){
            k = i;
        }
    }
    return k;
}

int select_nonbasic(Simplex_t* s){
    int i;
    for(i = 0; i < s->n; i++){
        if(s->c[i] > epsilon){
            return i;
        }
    }
    return -1;
}

void prepare(Simplex_t* s, int k){
    printf("Prepare verified!\n");
    int m = s->m;
    int n = s->n;
    int i;

    for(i = m + n; i > n; i--){
        s->var[i] = s->var[i - 1];
    }
    
    s->var[n] = m + n;
    n++;

    for(i = 0; i < m; i++){
        s->a[i][n - 1] = -1;
    }
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n - 1] = -1;
    s->n = n;
    pivot(s, k, n - 1);
}

int initial(Simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var){
    
    int i, j, k;
    double w;

    k = init(s, m, n, a, b, c, x, y, var);

    if(b[k] >= 0){
        return 1;
    }
    prepare(s, k);
    n = s->n;
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);

    for(i = 0; i < m + n; i++){
        if(s->var[i] == m + n - 1){
            if(fabs(s->x[i]) > epsilon){
                free(s->x);
                free(s->c);
                return 0;
            }
            else{
                break;
            }
        }
    }

    if(i >= n){
        for(j = k = 0; k < n; k++){
            if(fabs(s->a[i - n][k]) > fabs(s->a[i - n][j])){
                j = k;
            }
        }
        pivot(s, i - n, j);
        i = j;
    }
    if(i < n - 1){
        k = s->var[i];
        s->var[i] = s->var[n - 1];
        s->var[n - 1] = k;

        for(k = 0; k < m; k++){
            w = s->a[k][n - 1];
            s->a[k][n - 1] = s->a[k][i];
            s->a[k][i] = w;
        }
    }
    free(s->c);
    s->c = c;
    s->y = y;
    
    for(k = n - 1; k < n + m - 1; k++){
        s->var[k] = s->var[k + 1];
    }
    n = s->n = s->n - 1;
    double* t = calloc(n, sizeof(double));

    for(k = 0; k < n; k++){
        for(j = 0; j < n; j++){
            if(k == s->var[j]){
                t[j] = t[j] + s->c[k];
                continue;
            }
        }
        
        for(j = 0; j < m; j++){
            if(s->var[n + j] = k){
                break;
            }
        }
        
        s->y += s->c[k] * s->b[j];
        for(i = 0; i < n; i++){
            t[i] -= s->c[k] * s->a[j][i];
        }
    }

    for(i = 0; i < n; i++){
        s->c[i] = t[i];
    }
    free(t);
    free(s->x);

    return 1;
}

void pivot(Simplex_t* s, int row, int col){
    double** a = s->a;
    double* b = s->b;
    double* c = s->c;
    int m = s->m;
    int n = s->n;
    int i, j, t;

    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y += (c[col] * b[row]) / a[row][col];
    
    for(i = 0; i < n; i++){
        if(i != col){
            c[i] += -(c[col] * a[row][i]) / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];

    for(i = 0; i < m; i++){
        if(i != row){
            b[i] += -(a[i][col] * b[row]) / a[row][col];
        }
    }

    for(i = 0; i < m; i++){
        if(i != row){
            for(j = 0; j < n; j++){
                if(j != col){
                    a[i][j] += -(a[i][col] * a[row][j]) / a[row][col]; 
                }
            }
        }
    }

    for(i = 0; i < m; i++){
        if(i != row){
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    for(i = 0; i < n; i++){
        if(i != col){
            a[row][i] = a[row][i] / a[row][col];
        }
    }
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x , double y, int* var, int h){
    
    Simplex_t* s;
    int i, row, col;

    if(!initial(s, m, n, a, b, c, x, y, var)){
        free(s->var);
        return NAN;
    }
    while((col = select_nonbasic(s)) >= 0){
        row = -1;
        for(i = 0; i < m; i++){
            if(a[i][col] > epsilon && 
                (row < 0 || (b[i] / a[i][col]) < (b[row] / a[row][col]))){
                row = i;
            }
        }
        if(row < 0){
            free(s->var);
            return INFINITY;
        }
        pivot(s, row, col);
    }
    if(h == 0){
        for(i = 0; i < n; i++){
            if(s->var[i] < n){
                x[s->var[i]] = 0;
            }
        }
        for(i = 0; i < m; i++){
            if(s->var[n + i] < n){
                x[s->var[n + i]] = s->b[i];
            }
        }
        free(s->var);
    }
    else{
        for(i = 0; i < n; i++){
            x[i] = 0;
        }
        for(i = n; i < n + m; i++){
            x[i] = s->b[i - n];
        }
    }
    return s->y;
}

double simplex(int m, int n, double** a, double* b, double* c, double* x , double y){
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

void print_s(int m, int n, double** a, double* b, double* c, double* x){

    int i, j;

    printf("m = %d, n = %d\n", m, n);

    printf("a =\n[");
    for(i = 0; i < m; i++){
        printf("[");
        for(j = 0; j < n - 1; j++){
            printf("%10.3lf, ", a[i][j]);
        }
        if(i == m - 1){
            printf("%10.3lf]", a[i][n - 1]);
        }
        else{
            printf("%10.3lf],\n", a[i][n - 1]);
        }
    }
    printf("]\n");

    printf("b = [");
    for(i = 0; i < m; i++){
        if(i == m - 1){
            printf("%10.3lf", b[i]);
        }else{
            printf("%10.3lf, ", b[i]);
        }
    }
    printf("]\n");

    printf("c = [");
    for(int i = 0; i < n; i++){
        if(i == n - 1){
            printf("%10.3lf", c[i]);
        }else{
            printf("%10.3lf, ", c[i]);
        }
    }
    printf("]\n");
}

//Optimal value 16
int main()
{

    int m, n, i, j;
    double y = 0;
    scanf("%d %d", &m, &n);

    double* b = calloc(m, sizeof(double));
    double* x = calloc(n + 1, sizeof(double));
    double* c = calloc(n, sizeof(double));
    double** a = calloc(m, sizeof(double*));
    for(i = 0; i < m; i++){
        a[i] = calloc(n + 1, sizeof(double));
    }

    for(i = 0; i < n; i++){
        scanf("%lf", &c[i]);
    }
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            scanf("%lf", &a[i][j]);
        }
    }
    for(i = 0; i < m; i++){
        scanf("%lf", &b[i]);
    }

    print_s(m, n, a, b, c, x);

    double result = simplex(m, n, a, b, c, x, y);

    if(result != INFINITY){
        printf("Solution found, with z = %lf\n", result);
    }
    else{
        printf("Solution not found..\n");
    }
    
    for(i = 0; i < m; i++){
        free(a[i]);
    }
    free(a);
    free(b);
    free(c);
    free(x);

    return 0;
}