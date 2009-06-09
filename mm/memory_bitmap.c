#include <javieros/kernel.h>
#include <asm/page.h>
#include <javieros/buddy.h>


static unsigned long memory_bitmap_length = MEMORY_BITMAP_SIZE;
static unsigned char memory_bitmap[MEMORY_BITMAP_SIZE];


void init_memory_bitmap(void)
{
	int i;
	/* set all physical memory as busy */
	for (i = 0; i < memory_bitmap_length; i++)
		memory_bitmap[i] = (unsigned char)0xff;
}
