#ifndef STRING_H_
#define STRING_H_

#include "common/types.h"

void *memset(void *s, int c, u32 n);
int memcmp(const void *cs, const void *ct, unsigned int count);
void * memcpy(void *dest, const void *src, unsigned int count);
void *memchr (const void *s, int c, u32 n);

int sprintf(char *buf, const char *fmt, ...);
u32 strlen(const char * s);
char *strncpy(char *dest, const char *src, u32 count);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, unsigned int count);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
void *memmove(void *dest, const void *src, unsigned int n);

#endif // STRING_H_