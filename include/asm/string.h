#ifndef __STRING_H
#define __STRING_H

#include <javieros/types.h>        /* for size_t */
#include <javieros/stddef.h>       /* for NULL */

inline char * strcpy(char *dst, const char *src);
inline char * strncpy(char *dst, const char *src, size_t nr);
inline char * strcat(char *dst, const char *src);
inline char * strncat(char *dst, const char *src, size_t nr);
inline int strcmp(const char *cs, const char *ct);
inline int strncmp(const char *cs, const char *ct, size_t nr);
inline char * strchr(const char *str, int c);
inline char * strrchr(const char *str, int c);
inline size_t strlen(const char *s);
inline size_t strnlen(const char *str, size_t nr);
inline char * strstr(const char *cs, const char *ct);
inline void * memset(void *s, char c, size_t count);
inline void * memcpy(void *dst, const void *src, size_t nr);
inline void * memmove(void *dst, const void *src, size_t nr);
inline void * memchr(const void *cs, int c, size_t nr);
inline void * memscan(void * addr, int c, size_t size);

#endif
