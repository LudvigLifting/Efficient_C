#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int m, n;
    scanf("%d %d\n", &m, &n);

    double** a;
    double* b;
    double* c;

    //Allocate space for A
    a = calloc(m, sizeof(double*));
    for(int i = 0; i < m; i++){
        a[i] = calloc(n, sizeof(double));
    }
    b = calloc(m, sizeof(double));
    c = calloc(n, sizeof(double));

    for(int i = 0; i < n; i++){
        scanf("%lf", &c[i]);
    }
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            scanf("%lf", &a[i][j]);
        }
    }
    for(int i = 0; i < m; i++){
        scanf("%lf", &b[i]);
    }

    printf("a =\n[");
    for(int i = 0; i < m; i++){
        printf("[");
        for(int j = 0; j < n-1; j++){
            printf("%10.3lf, ", a[i][j]);
        }
        if(i == m-1){
            printf("%10.3lf]", a[i][n-1]);
        }
        else{
            printf("%10.3lf],\n", a[i][n-1]);
        }
    }
    printf("]\n");

    printf("b = [");
    for(int i = 0; i < m; i++){
        if(i == m-1){
            printf("%10.3lf", b[i]);
        }else{
            printf("%10.3lf, ", b[i]);
        }
    }
    printf("]\n");

    printf("c = [");
    for(int i = 0; i < n; i++){
        if(i == n-1){
            printf("%10.3lf", c[i]);
        }else{
            printf("%10.3lf, ", c[i]);
        }
    }
    printf("]\n");

    for(int i = 0; i < m; i++){
        free(a[i]);
    }
    free(a);
    free(b);
    free(c);
    return 0;
}