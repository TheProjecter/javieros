#include <asm/string.h>
#include <javieros/kernel.h>

inline char *
strcpy(char *dst, const char *src)
{
	int d0, d1, d2;
	__asm__ __volatile__(
		"1:\tlodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b"
		: "=&S"(d0), "=&D"(d1), "=&a"(d2)
		: "0"(src), "1"(dst)
		: "memory"
	);
	return dst;
}

inline char *
strncpy(char *dst, const char *src, size_t nr)
{
	int d0, d1, d2, d3;
	__asm__ __volatile__(
		"1:\tdecl %2\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"rep\n\t"
		"stosb\n"
		"2:"
		: "=&S"(d0), "=&D"(d1), "=&c"(d2), "=&a"(d3)
		: "0"(src), "1"(dst), "2"(nr)
		: "memory"
	);
	return dst;
}

inline char *
strcat(char *dst, const char *src)
{
	int d0, d1, d2, d3;
	__asm__ __volatile__(
		"repne\n\t"
		"scasb\n\t"
		"decl %1\n"
		"1:\tlodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b"
		: "=&S"(d0), "=&D"(d1), "=&a"(d2), "=&c"(d3)
		: "0"(src), "1"(dst), "2"(0), "3"(0xffffffff)
		: "memory"
	);
	return dst;
}

inline char *
strncat(char *dst, const char *src, size_t nr)
{
	int d0, d1, d2, d3;
	__asm__ __volatile__(
		"repne\n\t"
		"scasb\n\t"
		"decl %1\n\t"
		"movl %8, %3\n"
		"1:\tdecl %3\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"stosb\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"2:\txorl %2, %2\n\t"
		"stosb"
		: "=&S"(d0), "=&D"(d1), "=&a"(d2), "=&c"(d3)
		: "0"(src), "1"(dst), "2"(0), "3"(0xffffffff), "g"(nr)
		: "memory"
	);
	return dst;
}

inline int
strcmp(const char *cs, const char *ct)
{
	int d0, d1;
	register int __res;
	__asm__ __volatile__(
		"1:\tlodsb\n\t"
		"scasb\n\t"
		"jne 2f\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"xorl %%eax, %%eax\n\t"
		"jmp 3f\n"
		"2:\tsbbl %%eax, %%eax\n\t"
		"orb $1, %%al\n"
		"3:"
		: "=a"(__res), "=&S"(d0), "=&D"(d1)
		: "1"(cs), "2"(ct)
	);
	return __res;
}

inline int
strncmp(const char *cs, const char *ct, size_t nr)
{
	register int __res;
	int d0, d1, d2;
	__asm__ __volatile__(
		"1:\tdecl %3\n\t"
		"js 2f\n\t"
		"lodsb\n\t"
		"scasb\n\t"
		"jne 3f\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n"
		"2:\txorl %%eax, %%eax\n\t"
		"jmp 4f\n"
		"3:\tsbbl %%eax, %%eax\n\t"
		"orb $1, %%al\n"
		"4:"
		: "=a"(__res), "=&S"(d0), "=&D"(d1), "=&c"(d2)
		: "1"(cs), "2"(ct), "3"(nr)
		: "memory"
	);
	return __res;
}

inline char *
strchr(const char *str, int c)
{
	int d0;
	register char * __res;
	__asm__ __volatile__(
		"movb %%al, %%ah\n"
		"1:\tlodsb\n\t"
		"cmpb %%ah, %%al\n\t"
		"je 2f\n\t"
		"testb %%al, %%al\n\t"
		"jne 1b\n\t"
		"movl $1, %1\n"
		"2:\tmovl %1, %0\n\t"
		"decl %0"
		: "=a"(__res), "=&S"(d0)
		: "1"(str), "0"(c)
	);
	return __res;
}

inline char *
strrchr(const char *str, int c)
{
	int d0, d1;
	register char * __res;
	__asm__ __volatile__(
		"movb %%al, %%ah\n\t"
		"1:\tlodsb\n\t"
		"cmpb %%ah, %%al\n\t"
		"jne 2f\n\t"
		"leal -1(%%esi), %0\n"
		"2:\ttestb %%al, %%al\n\t"
		"jne 1b"
		: "=g"(__res), "=&S"(d0), "=&a"(d1)
		: "0"(0), "1"(str), "2"(c)
	);
	return __res;
}

inline size_t
strlen(const char *str)
{
	int d0;
	register size_t __res;
	__asm__ __volatile__(
		"repne\n\t"
		"scasb\n\t"
		"notl %0\n\t"
		"decl %0"
		: "=c"(__res), "=&D"(d0)
		: "1"(str), "a"(0), "0"(0xffffffff)
	);
	return __res;
}

inline size_t
strnlen(const char *str, size_t nr)
{
	int d0;
	register size_t __res;
	__asm__ __volatile__(
		"movl %2, %0\n\t"
		"jmp 2f\n\t"
		"1:\tcmpb $0, (%0)\n\t"
		"je 3f\n\t"
		"incl %0\n"
		"2:\tdecl %1\n\t"
		"cmpl $-1, %1\n\t"
		"jne 1b\n"
		"3:\tsubl %2, %0"
		: "=a"(__res), "=&d"(d0)
		: "c"(str), "1"(nr)
	);
	return __res;
}

inline char *
strstr(const char *cs, const char *ct)
{
	int d0, d1;
	register char * __res;
 	__asm__ __volatile__(
		"movl %6,%%edi\n\t"
		"repne\n\t"
		"scasb\n\t"
		"notl %%ecx\n\t"
	        /* NOTE! This also sets Z if searchstring='' */
		"decl %%ecx\n\t"
		"movl %%ecx,%%edx\n"
		"1:\tmovl %6,%%edi\n\t"
		"movl %%esi,%%eax\n\t"
		"movl %%edx,%%ecx\n\t"
		"repe\n\t"
		"cmpsb\n\t"
		/* also works for empty string, see above */
		"je 2f\n\t"
		"xchgl %%eax,%%esi\n\t"
		"incl %%esi\n\t"
		"cmpb $0,-1(%%eax)\n\t"
		"jne 1b\n\t"
		"xorl %%eax,%%eax\n\t"
		"2:"
		: "=a"(__res), "=&c"(d0), "=&S"(d1)
		: "0"(0), "1"(0xffffffff), "2"(cs), "g"(ct)
		: "dx", "di"
	);
	return __res;
}


inline void *
memset(void *s, char c, size_t count)
{
	int d0, d1;
	__asm__ __volatile__(
		"rep\n\t"
		"stosb"
		:"=&c"(d0), "=&D"(d1)
		:"a"(c), "1"(s), "0"(count)
		:"memory"
	);
	return s;
}

inline void *
memcpy(void *dst, const void *src, size_t nr)
{
	int d0, d1, d2;
	__asm__ __volatile__(
		"rep; movsl\n\t"
		"testb $2, %b4\n\t"
		"je 1f\n\t"
		"movsw\n"
		"1:\ttestb $1, %b4\n\t"
		"je 2f\n\t"
		"movsb\n"
		"2:"
		: "=&c"(d0), "=&D"(d1), "=&S"(d2)
		: "0"(nr/4), "q"(nr), "1"((long)dst), "2"((long)src)
		: "memory"
	);
	return dst;
}

inline void *
memmove(void *dst, const void *src, size_t nr)
{
	int d0, d1, d2;

	if(dst < src)
		__asm__ __volatile__(
			"rep\n\t"
			"movsb"
			: "=&c"(d0), "=&S"(d1), "=&D"(d2)
			: "0"(nr), "1"(src), "2"(dst)
			: "memory"
		);
	else
		__asm__ __volatile__(
			"std\n\t"
			"rep\n\t"
			"movsb\n\t"
			"cld"
			: "=&c"(d0), "=&S"(d1), "=&D"(d2)
			: "0"(nr),
			  "1"(nr-1+(const char*)src),
			  "2"(nr-1+(char *)dst)
			: "memory"
		);

	return dst;
}

inline void *
memchr(const void *cs, int c, size_t nr)
{
	int d0;
	register void * __res;
	if(!nr)
		return NULL;
	__asm__ __volatile__(
		"repne\n\t"
		"scasb\n\t"
		"je 1f\n\t"
		"movl $1, %0\n"
		"1:\tdecl %0"
		: "=D"(__res), "=&c"(d0)
		: "a"(c), "0"(cs), "1"(nr)
	);
	return __res;
}

inline void *
memscan(void * addr, int c, size_t size)
{
	if(!size)
		return addr;
	__asm__ __volatile__(
		"repnz; scasb\n\t"
		"jnz 1f\n\t"
		"dec %%edi\n"
		"1:"
		: "=D"(addr), "=c"(size)
		: "0"(addr), "1"(size),"a"(c)
	);
	return addr;
}
#if 0
/* string.c testing code*/
void
string_test(void)
{
	char str1[32], str2[32], str3[64];

	/* strcpy*/
	strcpy(str1, "string");
	printk("strcpy \"string\" to str1: %s\n", str1);

	/* memset */
	memset(str2, 1, sizeof(str2));
	printk("memset str2 to 1, str2[0]:%d\n"
		"                  str2[1]:%d\n"
		"                  str2[2]:%d\n"
		"                  str2[3]:%d\n"
		"                  str2[4]:%d\n"
		"                  str2[5]:%d\n"
		"                  str2[6]:%d\n"
		"                  ... ...   \n", 
		str2[0],str2[1],str2[2],str2[3],
		str2[4],str2[5],str2[6]);

	DELAY
	/* strncpy */
	memset(str2, 0, sizeof(str2));
	strncpy(str2, str1, 3);
	printk("strncpy (n=%d) \"%s\" to str2: \"%s\"\n", 3, str1, str2);

	/*strcat*/
	memset(str3, 0, sizeof(str3));
	strcpy(str3, "hello, ");
	strcpy(str1, "world");
	strcat(str3, str1);
	printk("strcat \"hello, \" and \"world\" : \"%s\"\n", str3);

	/* strncat */
	memset(str3, 0, sizeof(str3));
	strcpy(str3, "hello, ");
	strcpy(str1, "world !");
	strncat(str3, str1, 5);
	printk("strncat \"hello, \" and \"world !\", n=%d, result: \"%s\"\n", 5, str3);

	/* strcmp */
	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	strcpy(str1, "abc");
	strcpy(str2, "abcd");
	printk("strcmp, \"%s\" %s \"%s\"\n", str1, strcmp(str1, str2)? "not equal":"equal", str2);

	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	strcpy(str1, "abc");
	strcpy(str2, "abc");
	printk("strcmp, \"%s\" %s \"%s\"\n", str1, strcmp(str1, str2)? "not equal":"equal", str2);

	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	strcpy(str1, "Abc");
	strcpy(str2, "abc");
	printk("strcmp, \"%s\" %s \"%s\"\n", str1, strcmp(str1, str2)? "not equal":"equal", str2);

	/* strncmp */
	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	strcpy(str1, "abcd");
	strcpy(str2, "abc");
	printk("strncmp(n=%d), \"%s\" %s \"%s\"\n", 3, str1, strncmp(str1, str2, 3)? "not equal":"equal", str2);
	printk("strncmp(n=%d), \"%s\" %s \"%s\"\n", 4, str1, strncmp(str1, str2, 4)? "not equal":"equal", str2);

	/* strchr */
	memset(str1, 0, sizeof(str1));
	strcpy(str1, "abc12345abc");
	printk("strchr:  `%c' in position %d of \"%s\"\n", 'd', strchr(str1, 'd')-str1+1, str1);
	printk("strchr:  `%c' in position %d of \"%s\"\n", '5', strchr(str1, '5')-str1+1, str1);

	/* strrchr */
	memset(str1, 0, sizeof(str1));
	strcpy(str1, "abc12345abc");
	printk("strrchr:  `%c' in position %d of \"%s\"\n", 'd', strrchr(str1, 'd')-str1+1, str1);
	printk("strrchr:  `%c' in position %d of \"%s\"\n", '5', strrchr(str1, '5')-str1+1, str1);
	printk("strrchr:  char '%c' string \"%s\", result: \"%s\".\n", '1', str1, strrchr(str1, '1'));
	printk("strrchr:  char '%c' string \"%s\", result: \"%s\".\n", 'w', str1, strrchr(str1, 'w'));

	/* strlen */
	strcpy(str1, "1234567");
	printk("strlen of \"%s\" is %d.\n", str1, strlen(str1));
	strcpy(str1, "");
	printk("strlen of \"%s\" is %d.\n", str1, strlen(str1));
	
	/* strnlen */
	strcpy(str1, "1234567");
	printk("strnlen of \"%s\" (n=%d)is %d.\n", str1, 5, strnlen(str1, 5));
	printk("strnlen of \"%s\" (n=%d)is %d.\n", str1, 100, strnlen(str1, 100));

	/* strstr */
	strcpy(str3, "this is a strstr testing string.");
	strcpy(str1, "strstr");
	printk("strstr result (cs:\"%s\", ct:\"%s\") : \"%s\"\n", str3, str1, strstr(str3, str1));
	strcpy(str1, "hello");
	printk("strstr result (cs:\"%s\", ct:\"%s\") : \"%s\"\n", str3, str1, strstr(str3, str1));
	strcpy(str1, "");
	printk("strstr result (cs:\"%s\", ct:\"%s\") : \"%s\"\n", str3, str1, strstr(str3, str1));

	/* memcpy*/
	memset(str3, '?', sizeof(str3));
	str3[sizeof(str3)-1] = 0;
	strcpy(str1, "hello");
	memcpy(str3, str1, strlen(str1));
	printk("memcpy: \"%s\"\n", str3);
	memcpy(str3, str1, sizeof(str1));
	printk("memcpy: \"%s\"\n", str3);

	/* memmove*/
	memset(str3, 0, sizeof(str3));
	memset(str1, 0, sizeof(str1));
	strcpy(str3, "0123456789abcdefghijklmnopqrstuvwxyz");
	memmove(str1, str3, 10);
	printk("memmove: result: \"%s\"\n", str1);
	memmove(&str3[10], &str3[1], 5);
	printk("memmove: result: \"%s\"\n", str3);

	/* memchr */
	memset(str1, 0, sizeof(str1));
	strcpy(str1, "abc12345abc");
	printk("memchr:  `%c' in \"%s\", result: \"%s\". position is %d\n", 
		'c', str1, (char *)memchr(str1, 'c', strlen(str1)),
		(char *)memchr(str1, 'c', strlen(str1))-str1+1);

	/* memscan */
	memset(str1, 0, sizeof(str1));
	strcpy(str1, "abc12345abc");
	printk("memscan:  `%c' in \"%s\", result: \"%s\". position is %d\n", 
		'c', str1, (char *)memscan(str1, 'c', strlen(str1)),
		(char *)memscan(str1, 'c', strlen(str1))-str1+1);	
	return;
}
#endif
