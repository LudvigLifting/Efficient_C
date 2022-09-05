#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
	int c = '0', i;
	size_t size = 17;

	char* str = calloc(size, sizeof(char));
	char* biggest = calloc(size, sizeof(char));

	size_t big = 0;
	

	while(c != EOF){
		i = 0;

		while(isalpha(c = getchar())){
			if(i >= size){
				size *= 2;
				str = realloc(str, size);
				biggest = realloc(biggest, size);
			}
			str[i] = c;
			i++;
		}

		if(i > big){
			memcpy(biggest, str, i);
			big = i;
		}

		memset(str, 0, strlen(str));
	}
	
	free(str);
	printf("%ld characters in longest word: %s\n", big, biggest);
	free(biggest);
	return 0;
}
