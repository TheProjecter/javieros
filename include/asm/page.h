#ifndef __PAGE_H
#define __PAGE_H

#define BOOT_USER_PGD_PTRS		768
#define BOOT_KERNEL_PGD_PTRS	256

#define PAGE_OFFSET		(0xC0000000)
#define PHYSICALMEM_MAXSIZE (0xFFFFFFFF)

#define TO_PA(x)	((x) - PAGE_OFFSET)
#define TO_VA(x)	((x) + PAGE_OFFSET)

#ifndef __ASM__

#define PAGE_SHIFT (12)
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#endif

#endif /* PAGE_H */
