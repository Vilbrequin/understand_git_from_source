/*************************************************************************************************
 * This header file contains all macros used by git as helper macros
 * we will try to re-implement them and understand what geeks try to do
 *************************************************************************************************/

#ifndef GIT_UTILS_H
#define GIT_UTILS_H

#include<limits.h>
#include<stdint.h>

/* first, we will try to "re-build" the macro that checks if the multiplication
 * of "a" and "b" overflows. Types of "a" and "b" must match and be unsigned
 *
 * the idea is if "a" and "b" are the same type then we should find the max value
 * that "b" could have, so the a * max_possible_b <= maximum_val_of_type_b
 *
 * e.g. let a = 10 and b = 110 both unigned char variables then to test if a *b overflows
 * 	- what is the maximum value that an unsigned char could take? - 255 
 * 	- lets take 255 and divide it by a => (255 / a) = 25 
 * 	- then the biggist unsigned char that could be multiplied by a without overflow is 25
 * 	- but in this case b is greater than 25 so the multiplication overflows
 * */


#define bitsizeof(x)				(CHAR_BIT * sizeof(x))

#define maximum_unsigned_value_of_type(a)	(ULLONG_MAX >> (bitsizeof(uintmax_t) - bitsizeof(a)))

#define unsigend_mult_overflows(a, b)		(a && (b > ( maximum_unsigned_value_of_type(a) / a)))

#endif /* GIT_UTILS_H */
