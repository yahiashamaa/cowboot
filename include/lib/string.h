#ifndef STRING_H_
#define STRING_H_

#include "common/types.h"

void *memset(void *s, int c, u32 n);
int memcmp(const void *cs, const void *ct, unsigned int count);
void * memcpy(void *dest, const void *src, unsigned int count);

int sprintf(char *buf, const char *fmt, ...);
u32 strlen(const char * s);

#endif // STRING_H_