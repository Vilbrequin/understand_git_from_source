#include <stdio.h>
#include <stdlib.h>
#include "git-wrappers.h"

int main(char agrc, char **argv){

    size_t nmemb = 0xffffffffffffffff;
    size_t size = 2;
	char *arr = xcalloc(nmemb, size);
	if(!arr){
		printf("Cant allocate memory\n");
	}
	else {
		printf("All good chef\n");
	}
	free(arr);
	return 0;
}