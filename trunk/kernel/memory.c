#include <multiboot.h>
#include <javieros/kernel.h>
#include <asm/page.h>
#include <asm/mm.h>
#include <javieros/buddy.h>


/* stored multiboot information */
char mbi_buffer[MBI_ROUNDEDUP_SIZE];
multiboot_info_t *mbi = (multiboot_info_t *)mbi_buffer;
static memory_map_t * mmap_table;
static unsigned long mmap_table_length;

static void print_mmap_table(void)
{
//	char *mmap_type_str[] = 
	memory_map_t *mmap;

//	unsigned long start_pf_nr, end_pf_nr;

	mmap_table = (memory_map_t *) mbi->mmap_addr;
	mmap_table_length = (unsigned long) mbi->mmap_length;

	printk("JavierOS mmap table (entry: 0x%x	length: 0x%x):\n",
			mmap_table, mmap_table_length);
#if 0
	for (mmap = mmap_table;
			mmap < (memory_map_t *)((unsigned long)mmap_table + mmap_table_length);
			mmap = (memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size)))
	{
		if ((unsigned long)mmap->base_addr_high != 0)
			continue;
		printk("base = 0x%x,", (unsigned long)mmap->base_addr_low);
		set_screen_pos_x(25);
		printk("length = %d KB,", (unsigned long)mmap->length_low / 1024);
		set_screen_pos_x(45);
		printk("type = %s\n",
				(unsigned long)mmap->type <= (sizeof(mmap_type_str)
	}
#endif
}

int init_memory(void)
{
	print_mmap_table();
//	init_memory_bitmap();

	return 0;
}
