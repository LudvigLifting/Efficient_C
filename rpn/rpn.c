#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

#define N		(10)

int calc(int a, int* bop){
	char c = bop[1];
	switch(c){
		case '+':
			return a + bop[0];
			break;
		case '-':
			return a - bop[0];
			break;
		case '*':
			return a * bop[0];
			break;
		case '/':
			if(bop[0] == 0){
				return INT_MAX;
			}
			return a / bop[0];
			break;
		default:
			return INT_MIN;
	}
}

int * build(int r[2], int* first, int* s_point, char* op){
	r[1] = *(op-1);

	if(first == s_point - 1){
		r[0] = *first;
		return r;
	}

	r[0] = calc(*first, build(r, first + 1, s_point, op - 1));
	return r;
}

int main(void)
{
	// char stack[N], operand[N - 1];
	int* s_pointer = calloc(N, sizeof(int));
	int* s_first = s_pointer;
	char* o_pointer = calloc(N - 1, sizeof(char));
	char* o_first = o_pointer;

	int c;
	int i = 1, result;
	int temp = 0;
	int r[2];


	while((c = getchar() - '0') != EOF){
		
		while(isdigit((char) (c + '0'))){
			if(c == EOF) return 0;
			temp = temp*10 + c;
			c = getchar() - '0';			
		}
		if(temp != 0 && (char) (c + '0') == ' ') c = temp;
		temp = 0;

		printf("c = %c, cd = %d\n", (char) c + '0', c);

		if(c > 9 || isdigit((char) (c + '0'))){
			if(s_pointer < s_first + N - 1){
				*s_pointer = c;
				s_pointer++;
			}
			else{
				printf("line %d: error at %c\n", i, (char) (c + '0'));
				i++;
				s_pointer = s_first;
				o_pointer = o_first;
			}
		}
		else {
			if((char) (c + '0') == '\n'){

				if(s_pointer - s_first > 1 && o_pointer - o_first == s_pointer - s_first - 1){
					result = calc(*(s_first), build(r, s_first + 1, s_pointer, o_pointer));
					printf("line %d: %d\n", i, result);
				}
				else{
					printf("line %d: error at %c\n", i, (char) (c + '0'));
				}
				i++;
				s_pointer = s_first;
				o_pointer = o_first;
			}
			else if((char) (c + '0') == '+' || (char) (c + '0') == '-' || (char) (c + '0') == '*' || (char) (c + '0') == '/'){
				if(o_pointer < o_first + N - 2){
					*o_pointer = c + '0';
					o_pointer++;
				}
			}
			else if((char) (c + '0') == ' '){
			}
			else{
				
				printf("line %d: error at %c\n", i, (char) (c + '0'));
				s_pointer = s_first;
				o_pointer = o_first;
			}
		}
	}
	s_pointer = s_first;
	o_pointer = o_first;
	free(s_pointer);
	free(o_pointer);
	return 0;
}
