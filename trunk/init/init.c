#include <multiboot.h>

unsigned long multiboot_magic = -1;

int init_memory(void);
//int init_

void start_kernel()
{
	if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		return;
	}
}
