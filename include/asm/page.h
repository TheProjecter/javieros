#ifndef __PAGE_H
#define __PAGE_H

#define BOOT_USER_PGD_PTRS		768
#define BOOT_KERNEL_PGD_PTRS	256

#define PAGE_OFFSET		(0xC0000000)

#define TO_PA(x)	((x) - PAGE_OFFSET)
#define TO_VA(x)	((x) + PAGE_OFFSET)

#endif /* PAGE_H */
