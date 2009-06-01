#ifndef __STDDEF_H
#define __STDDEF_H

#include <javieros/types.h>

#undef NULL
#define NULL ((void *)0)

#undef offsetof
#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)
#endif
