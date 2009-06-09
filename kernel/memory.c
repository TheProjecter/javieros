#include <multiboot.h>
#include <javieros/kernel.h>
#include <asm/page.h>
#include <asm/mm.h>
#include <javieros/buddy.h>


extern unsigned long total_mem_size;

/* stored multiboot information */
char mbi_buffer[MBI_ROUNDEDUP_SIZE];
multiboot_info_t *mbi = (multiboot_info_t *)mbi_buffer;
static memory_map_t * mmap_table;
static unsigned long mmap_table_length;

static void print_mmap_table(void)
{
	memory_map_t *mmap;

	unsigned long start_pf_nr, end_pf_nr;

	mmap_table = (memory_map_t *) mbi->mmap_addr;
	mmap_table_length = (unsigned long) mbi->mmap_length;

	printk("JavierOS mmap table (entry: 0x%x	length: 0x%x):\n",
			mmap_table, mmap_table_length);
return;
}

int init_memory(void)
{
//	print_mmap_table();
	init_memory_bitmap();

	return 0;
}
