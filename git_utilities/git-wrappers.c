/***********************************************************************************************
 * In this file we will explore how git used to wrap almost all standard glibC APIs.
 * First we will take a look on xcalloc the git calloc wrapper api.
 ***********************************************************************************************/

#include <stdio.h>
#include "git-utils.h"
#include <strings.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

int git_parse_unsigned(const char *value, uintmax_t *ret, uintmax_t max);

unsigned long git_env_ulong(const char *env);

static int get_unit_factor(const char* factor);

static int memory_limit_check(size_t size, int gentle);

void *xcalloc(size_t nmemb, size_t size);

int main(char agrc, char **argv){

	char *arr = xcalloc(1025, sizeof(char));
	if(!arr){
		printf("Cant allocate memory\n");
	}
	else {
		printf("All good chef\n");
	}
	free(arr);
	return 0;
}

void *xcalloc(size_t nmemb, size_t size){
	
	void *ret;

	// first check if "nmemb * size" will overflows
	if(unsigend_mult_overflows(nmemb, size)){
		//die function to do later
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
		exit(1);
	}
	return ret;
}

static int get_unit_factor(const char* factor) {
	if(!*factor){
		return 1;
	}
	// we will use the glibc api strcasecmp, that compars two strings byte-by-byte
	// with case ignored, and returns 0 in case of match
	else if (!strcasecmp(factor, "k")){
		return 1024;
	}
	
	else if (!strcasecmp(factor, "m")){
		return 1024 * 1024;
	}
	
	else if (!strcasecmp(factor, "g")){
		return 1024 * 1024 *1024;
	}
	
	return 0;
}


int git_parse_unsigned(const char *value, uintmax_t *ret, uintmax_t max){

	/* This API is used to parse the value that we set in an env var 
	 * that limits out the amount of allocated memory that we can use
	 * and retrun tha amount in bytes*/

	if(value && *value){
		// if value set in the env var is negativ we return 0 and 
		// skip the memory limit 
		uintmax_t val;
		char *end;
		int factor;
		
		if(strchr(value, '-')) {
			return 0;
		}
		// the input that we git expect from the user is in the following form 
		// 100m, or 20k or 15g or 200, where the digit value is the amount we
		// want to limit allocation to multiply by factor (nothing, k for kill
		// m for mega and g for gega),for that the strtoumax glibc is used, that 
		// converts the initial part of a string value to unsigned long int according 
		// to the base choosed (0 = decimal base), the conversion stopped at the first 
		// character wich is non valid digit in given base.
		//
		// e.g. in base 10  if value = 1500k => end = "k" and strtoumax returns (ul)1500
		val = strtoumax(value, &end, 0);
		
		// check if value has no digits 
		if(end == value){
			return 0;
		}

		factor = get_unit_factor(end);
		if(!factor){
			return 0;
		}
		// Check if the value that we set is not greater than a max threshold
		// or the result in bytes of factor and value will overflow 
		if (unsigend_mult_overflows(factor, val) || factor*val > max){
			return 0;
		}
		val *= factor;
		*ret = val;
		return 1;
	}
	return 0;
}


unsigned long git_env_ulong(const char *env){

	char *var = getenv(env);
	uintmax_t ret = 0;
	
	if(var && !git_parse_unsigned(var, &ret, maximum_unsigned_value_of_type(unsigned long))){
		// TODO exit the current process and return to parent process 
		exit(1);
	}
	return ret;
}

static int memory_limit_check(size_t size, int gentle){
	size_t limit = 0;
	limit = git_env_ulong("GIT_ALLOC_LIMIT");
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
			exit(1);
		}
	}
	return 0;
}


