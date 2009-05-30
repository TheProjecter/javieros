#ifndef __LINKAGE_H
#define __LINKAGE_H

/* can not define ALIGN directly */
#define __ALIGN	.align 16,0x90

#define ALIGN	__ALIGN

#define SYMBOL_NAME(X) X
#define SYMBOL_NAME_LABEL(X)	X:

#define ENTRY(name) \
	.globl SYMBOL_NAME(name); \
	ALIGN; \
	SYMBOL_NAME_LABEL(name)

#endif
