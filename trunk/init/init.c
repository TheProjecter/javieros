#include <multiboot.h>

unsigned long multiboot_magic = -1;

int init_memory(void);


void start_kernel()
{
	clear_screen();

	if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printk("invalid multiboot magic number:`0x%x'.\n", multiboot_magic);
		return;
	}

	printk("Welcom to JavierOS :)\n");

}
