#ifndef __BUDDY_H
#define __BUDDY_H

#ifndef __ASM__

#define MIN_PHY_MEMSIZE (32 << 20)
#define MAX_PHY_MEMSIZE (896 << 20)


#define MAX_PAGEFRAME_NR (MAX_PHY_MEMSIZE >> PAGE_SHIFT)

#define MEMORY_BITMAP_SIZE (PHYSICALMEM_MAXSIZE / (PAGE_SIZE * 8) + 1)
#define BITMAP_SET_FREE 0
#define BITMAP_SET_BUSY 1

#endif

#endif
