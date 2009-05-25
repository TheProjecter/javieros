#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

/* The magic number for the Multiboot header. */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/* The flags for the Multiboot header. */
#ifdef __ELF__
# define MULTIBOOT_HEADER_FLAGS         0x00000003
#else
# define MULTIBOOT_HEADER_FLAGS         0x00010003
#endif

/* The magic number passed by a Multiboot-compliant boot loader. */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/* The size of our stack (16KB). */
#define BOOT_STACK_SIZE                      0x4000
/* The maximum size of mmap_table */
#define MMAP_TABSIZE		32
/* maximum size of multiboot info structure.
   As multiboot spec v0.7, it is 90 bytes. it is bigger than
   sizeof(multiboot_info_t)
 */
#define MBI_ROUNDEDUP_SIZE	(90) 


#ifndef __ASM__
/* Do not include here in boot.S. */

/* Types. */

/* The Multiboot header. */
typedef struct multiboot_header
{
    unsigned long magic;
    unsigned long flags;
    unsigned long checksum;
    unsigned long header_addr;
    unsigned long load_addr;
    unsigned long load_end_addr;
    unsigned long bss_end_addr;
    unsigned long entry_addr;
} multiboot_header_t;

/* The symbol table for a.out. */
typedef struct aout_symbol_table
{
    unsigned long tabsize;
    unsigned long strsize;
    unsigned long addr;
    unsigned long reserved;
} aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct elf_section_header_table
{
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long shndx;
} elf_section_header_table_t;

/* The Multiboot information. */
typedef struct multiboot_info
{
    unsigned long flags;		//32
    unsigned long mem_lower;		//32
    unsigned long mem_upper;		//32
    unsigned long boot_device;	//32
    unsigned long cmdline;		//32
    unsigned long mods_count;	//32
    unsigned long mods_addr;		//32
    union 				//32*4
    {
	aout_symbol_table_t aout_sym;
	elf_section_header_table_t elf_sec;
    } u;
    unsigned long mmap_length;	//32
    unsigned long mmap_addr;		//32
} multiboot_info_t;

/* The module structure. */
typedef struct module
{
    unsigned long mod_start;
    unsigned long mod_end;
    unsigned long string;
    unsigned long reserved;
} multiboot_module_t;

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
typedef struct memory_map
{
    unsigned long size;
    unsigned long base_addr_low;
    unsigned long base_addr_high;
    unsigned long length_low;
    unsigned long length_high;
    unsigned long type;
} memory_map_t;

#endif /* ! ASM */
#endif /* __MULTIBOOT_H */
