/***********************************************************************************************
 * In this file we will explore how git used to wrap almost all standard glibC APIs.
 * First we will take a look on xcalloc the git calloc wrapper api.
 ***********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for calloc

#include "git-utils.h"
#include "git-parsers.h"
#include "git-wrappers.h"

static int memory_limit_check(size_t size, int gentle);

void *xcalloc(size_t nmemb, size_t size){
	
	void *ret;

	// first check if "nmemb * size" will overflows
	if(unsigend_mult_overflows(nmemb, size)){
		//die function to do later
		printf("xcalloc : Multiplication overflow.\n");
		exit(1);
	}

	/* git offers an additional memory allocation safety check
	 * with the api "memory_limit_check" that uses an env var
	 * GIT_ALLOC_LIMIT that can be set to an ammount of memory
	 * that we are allowed to use it ! */
	memory_limit_check(nmemb * size, 0);

	/* calloc function allocat memory of an array of nmemb elements of
	 * "size" bytes each, the big difference between malloc and calloc
	 * is the returend allocated memory is zeroed, like a malloc and memset
	 * in the other hand if the "nmemb * size" overdlows, calloc return error
	 * but malloc doesn't'*/
	ret = calloc(nmemb, size);
	
	/* if ret, we shall test if we do not have memory left to allocate, or
	 * one of "nmemb" or "size" is zero 
	 * - if "nmemb" or "size" we allocate one byte to return a pointer that can be
	 * deallocated safely with free.
	 *  - in the other case we must exit the current process and return to the parent process*/
	if(!ret && (!nmemb || !size)) {
		ret = calloc(1, 1);

	}
	if(!ret) {
		// if we still got NULL then no memory space available
		// we should exit, TODO implement die
		printf("xcalloc : No Memory Space available.\n");
		exit(1);
	}
	return ret;
}



static int memory_limit_check(size_t size, int gentle){
	size_t limit = 0;
	limit = git_env_ulong("GIT_ALLOC_LIMIT", 0);
	if (!limit){
		limit = SIZE_MAX;
	}
	if(size > limit){
		// in case gentle is set we only return -1 no process stoped
		// else we exit 
		if(gentle == 1){
			return -1;
		}
		else {
			printf("memory_limit_check : The requested memory bigger that what is allowed.\n");
			exit(1);
		}
	}
	return 0;
}


