#include <multiboot.h>


/* stored multiboot information */
char mbi_buffer[MBI_ROUNDEDUP_SIZE];
multiboot_info_t *mbi = (multiboot_info_t *)mbi_buffer;
memory_map_t mmap_table[MMAP_TABSIZE] = {{0,0,0,0,0,0},};
int mmap_table_length = 0;

