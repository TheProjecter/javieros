#define ASM 1

#include <multiboot.h>

.text

.globl start, _start
start:
_start:
	jmp multiboot_entry

.align 4

multiboot_header:
	.long   MULTIBOOT_HEADER_MAGIC
	.long   MULTIBOOT_HEADER_FLAGS
	.long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	.long   multiboot_header
	.long   _start
	.long   _edata
	.long   _end
	.long   multiboot_entry

multiboot_entry:
	/* init the stack pointer. */
	movl $(boot_stack + STACK_SIZE), %esp

	/* reset EFLAGS. */
	pushl $0
	popf

	/* push the pointer to multiboot info structure. */
	pushl %ebx
	/* push the magic value. */
	pushl %eax

	/* call C code */
	call cmain

	/* halt */
	pushl $halt_message
	call printf

boot_halt:
	hlt
	jmp boot_halt

halt_message:
	.asciz "Halted."

.comm boot_stack, STACK_SIZE
