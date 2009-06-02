#include <multiboot.h>
#include <javieros/kernel.h>
#include <asm/page.h>
#include <asm/mm.h>
#include <javieros/buddy.h>


/* stored multiboot information */
char mbi_buffer[MBI_ROUNDEDUP_SIZE];
multiboot_info_t *mbi = (multiboot_info_t *)mbi_buffer;
memory_map_t mmap_table[MMAP_TABSIZE] = {{0,0,0,0,0,0},};
int mmap_table_length = 0;

static void print_mmap_table(void)
{
//	char *mmap_type_str[] = 
	memory_map_t *mmap;

//	unsigned long start_pf_nr, end_pf_nr;

//	mmap_table = (memory_map_t *) mbi->mmap_addr;
//	mmap_table_length = (unsigned long) mbi->mmap_length;

	printk("JavierOS mmap table (entry: 0x%x	length: 0x%x):\n",
			mmap_table, mmap_table_length);
}

int init_memory(void)
{
	print_mmap_table();
//	init_memory_bitmap();

	return 0;
}
