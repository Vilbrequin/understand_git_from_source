#include <inttypes.h> // for strtoumax
#include <strings.h> // for strcasecmp
#include <string.h> // for strchr
#include <stdlib.h> // for getenv & exit 
#include <stdio.h> // for printf
#include "git-utils.h"
#include "git-parsers.h"


int get_unit_factor(const char* factor) {
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

inline int git_parse_ulong(const char *value, unsigned long *ret){

	// in the official git implementation this api make use of the generic parse api
	// but without the inline keyword, i found that this api has to be tagged inline
	// in order to remove the stack pop/push overhead operations
	unsigned long tmp;
	if(!git_parse_unsigned(value, &tmp, maximum_unsigned_value_of_type(unsigned long))) {
		return 0;
	}
	*ret = tmp;
	return 1;
}

unsigned long git_env_ulong(const char *env, unsigned long val){

	char *var = getenv(env);
	if(var && !git_parse_ulong(var, &val)){
		// TODO exit the current process and return to parent process 
		printf("git_env_ulong : Failed to parse the envirement variable %s.\n", env);
		exit(1);
	}
	return val;
}