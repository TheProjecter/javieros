#ifndef __BUDDY_H
#define __BUDDY_H

#ifndef __ASM__

#define MIN_PHY_MEMSIZE (32 << 20)
#define MAX_PHY_MEMSIZE (896 << 20)


#define MAX_PAGEFRAME_NR (MAX_PHY_MEMSIZE >> PAGE_SHIFT)

#endif

#endif
