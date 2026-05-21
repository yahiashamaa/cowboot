#include "common/types.h"

#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_ALT_FORM_FLAG			    1
#include "lib/nanoprintf.h"

#include "lib/string.h"

void *memset(void *s, int c, u32 n)
{
    u8 *p = (u8 *)s;
    while(n--) *p++ = (u8)c;
    return s;
}

int memcmp(const void *cs, const void *ct, unsigned int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

void * memcpy(void *dest, const void *src, unsigned int count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}

void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = (const unsigned char *)s;

	while (n--) {
		if (*p == (unsigned char)c) {
			return (void *)p;
		}

		++p;
	}

	return NULL;
}

int sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = npf_vsnprintf(buf, 1024, fmt, args);
	va_end(args);
	return i;
}

u32 strlen(const char * s) {
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        /* nothing hehe */;
    return sc - s;
}

char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

int strcmp(const char *cs, const char *ct)
{
    while (*cs == *ct) {
        if (!*cs)
            return 0;
        cs++;
        ct++;
    }
    return (unsigned char)*cs - (unsigned char)*ct;
}

int strncmp(const char *cs, const char *ct, unsigned int count)
{
    while (count) {
        if (*cs != *ct)
            return (unsigned char)*cs - (unsigned char)*ct;
        if (!*cs)
            break;
        cs++;
        ct++;
        count--;
    }
    return 0;
}

char *strchr(const char *s, int c)
{
    do {
        if (*s == (char)c)
            return (char *)s;
    } while (*s++);
    return NULL;
}

char *strrchr(const char *s, int c)
{
    const char *last = NULL;
    do {
        if (*s == (char)c)
            last = s;
    } while (*s++);
    return (char *)last;
}

void *memmove(void *dest, const void *src, unsigned int n)
{
    char *d = dest;
    const char *s = src;
    if (d == s)
        return dest;
    if (d < s) {
        while (n--)
            *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--)
            *--d = *--s;
    }
    return dest;
}