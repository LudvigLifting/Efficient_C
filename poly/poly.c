#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "error.h"
#include "poly.h"

/*
330x^799 - 980x^729 + 742x^682 + 808x^652 - 522x^562 - 985x^462 + 670x^382 - 885x^300 - 782x^245 - 778x^181 + 460x^166 - 440x^82 + 535,
388x^343 - 338x^324 + 973x^320 - 795x^294 - 32x^216 + 815x^183 - 945x^87 - 406x^24 - 846
*/

struct Node{
    int coeff;
    int exp;
    Node* next;
};

struct poly_t{
    Node* head;
};

poly_t* new_poly_from_string(const char* str){
    poly_t* poly = calloc(1, sizeof(*poly));
    poly->head = calloc(1, sizeof(*poly->head));
    int coeff = 0, isexp = 0, exp = 0, sign = 1;

    Node* node = poly->head;
    for(int i = 0; str[i] != 0; i++){
        if(isdigit(str[i])){
            if(!isexp){
                coeff = (coeff*10 + (str[i] - '0') * sign);
                sign = 1;
            }
            else{
                exp = exp*10 + (str[i] - '0');
            }
        }
        switch(str[i]){
            case '^':
                isexp = 1;
                exp = 0;
                break;
            case ' ':
                if(coeff != 0){
                    node->coeff = coeff;
                    node->exp = exp;
                    node->next = calloc(1, sizeof(*poly->head));
                    node = node->next;
                }
                isexp = 0;
                coeff = 0;
                exp = 0;
                break;
            case 'x':
                if(coeff == 0) coeff = 1;
                exp = 1;
                break;
            case '-':
                sign = -1;
            default:
                break;
        }
    }
    if(coeff != 0) node->coeff = coeff;
    node->next = NULL;
    //free(node);
    return poly;
}

poly_t* mul(poly_t* poly_1, poly_t* poly_2){
    
    poly_t* result = calloc(1, sizeof(*result));
    result->head = calloc(1, sizeof(*result->head));
    Node* node_1 = poly_1->head;
    Node* node_2 = poly_2->head;
    Node* resnode = result->head;

    while(node_1 != NULL){
        node_2 = poly_2->head;
        while(node_2 != NULL){
            resnode->exp = node_1->exp + node_2->exp;
            resnode->coeff = node_1->coeff * node_2->coeff;
            node_2 = node_2->next;
            if(node_1->next == NULL && node_2 == NULL){
                resnode->next = NULL;
            }
            else{
                resnode->next = calloc(1, sizeof(*resnode->next));
                resnode = resnode->next;
            }
        }
        node_1 = node_1->next;
    }

    Node* temp = result->head;
    while(temp != NULL){
        printf("%dx^%d->", temp->coeff, temp->exp);
        temp = temp->next;
    }
    printf("\n");

    temp = result->head;
    Node* prev = temp;
    while(temp != NULL){
        prev = temp;
        temp = temp->next;
        if(temp->exp == prev->exp){
            prev->coeff += temp->coeff;
            prev->next = temp->next;
            temp = prev->next;
        }
    }

    temp = result->head;
    while(temp != NULL){
        printf("%dx^%d->", temp->coeff, temp->exp);
        temp = temp->next;
    }
    free(temp);
    printf("\n");
    return result;
}

void free_poly(poly_t* poly){
    Node* node = poly->head;
    Node* temp;
    while(node != NULL){
        temp = node;
        node = node->next;
        free(temp);
    }
    free(node);
    free(poly->head);
    free(poly);
}

void print_poly(poly_t* poly){
    Node* node = poly->head;
    int first = 1;
    char sign = ' ';
    
    while(node != NULL){
        if(node->coeff > 0) sign = '+';
        else if(node->coeff < 0) sign = '-';
        if(node->coeff == 1){
            if(node->exp == 1){
                if(first){
                    printf("x");
                }else{
                    printf(" %c x", sign);
                } 
            }else if(node->exp == 0){
                if(first){
                    printf("%d", node->coeff);
                }else{
                    printf(" %c %d", sign, abs(node->coeff));
                }

            }else{
                if(first){
                    printf("x^%d", node->exp);
                }else{
                    printf(" %c x^%d", sign, node->exp);
                }
            }
        }else{
            if(node->exp == 1){
                if(first){
                    printf("%dx", node->coeff);
                }else{
                    printf(" %c %dx", sign, abs(node->coeff));
                }
            }else if(node->exp == 0){
                if(first){
                    printf("%d", node->coeff);
                }else{
                    printf(" %c %d", sign, abs(node->coeff));
                }

            }else{
                if(first){
                    printf("%dx^%d", node->coeff, node->exp);
                }else{
                    printf(" %c %dx^%d", sign, abs(node->coeff), node->exp);
                }
            }
        }
        first = 0;
        node = node->next;
    }
    printf("\n");
}

// static void poly_test(char* a, char* b)
// {
//     poly_t* p;
//     poly_t* q;
//     poly_t* r;
//     printf("Begin polynomial test of (%s) * (%s)\n", a, b);
//     p = new_poly_from_string(a);
//     q = new_poly_from_string(b);
//     print_poly(p);
//     print_poly(q);
//     r = mul(p, q);
//     print_poly(r);
//     free_poly(p);
//     free_poly(q);
//     free_poly(r);
//     printf("End polynomial test of (%s) * (%s)\n\n\n", a, b);
// }

// int main(void)
// {
//         poly_test("x^2 - 7x + 1", "3x + 2");
//         poly_test("x^10000000 + 2", "2x^2 + 3x + 4");
//         return 0;
// }
