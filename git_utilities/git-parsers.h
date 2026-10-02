#ifndef GIT_PARSERS_H
#define GIT_PARSERS_H

#include <stdint.h>

int get_unit_factor(const char* factor);

int git_parse_unsigned(const char *value, uintmax_t *ret, uintmax_t max);

int git_parse_ulong(const char *value, unsigned long *ret);

unsigned long git_env_ulong(const char *env, unsigned long val);

#endif
