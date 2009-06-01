/* REF: docs/multiboot_spec.txt */

#include <multiboot.h>
#include <asm/page.h>
#include <javieros/tty.h>
#include <asm/desc.h>
#include <asm/linkage.h>
#include <asm/segment.h>


.text

.globl _va_start
_va_start:
	jmp multiboot_entry

.align 4

multiboot_header:
	.long   MULTIBOOT_HEADER_MAGIC
	.long   MULTIBOOT_HEADER_FLAGS
	.long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	.long   TO_PA(multiboot_header)
	.long   _pa_start
	.long   TO_PA(_edata)
	.long   TO_PA(_end)
	.long   TO_PA(multiboot_entry)
	nop
multiboot_entry:
	/* init the stack pointer */
	movl $TO_PA(boot_stack + BOOT_STACK_SIZE), %esp

	/* reset EFLAGS */
	pushl $0
	popf

	/* save the magic value */
	movl %eax, TO_PA(multiboot_magic)
	/* save the info structure */
	movl %ebx, %esi
	movl $TO_PA(mbi_buffer), %edi
	movl $MBI_ROUNDEDUP_SIZE, %ecx
	rep movsb

	/* clear BSS */
	xorl %eax, %eax
	movl $TO_PA(__bss_start), %edi
	movl $TO_PA(_end), %ecx
	subl %edi, %ecx
	rep
	stosb

	/* save mmap table */
	movl 11*4(%ebx), %ecx	/* mmap_length: see multiboot_info structure */
	movl 12*4(%ebx), %esi	/* mmap_addr */
	movl %ecx, TO_PA(mmap_table_length)
	movl $TO_PA(mmap_table), %edi
	rep movsl
	
	/* init page tables */
	movl $TO_PA(pg0), %edi
	movl $007, %eax		/* PRESENT+RW+USER */
2:
	stosl
	addl $0x1000, %eax
	cmp	$TO_PA(empty_zero_page), %edi
	jne	2b
	/* enable paging */
	movl $TO_PA(swapper_pg_dir), %eax
	movl %eax, %cr3
	movl %cr0, %eax
	orl	$0x80000000, %eax
	movl %eax, %cr0
	jmp	3f		/* flush the prefetch-queue */
3:
	movl $4f, %eax
	jmp	*%eax		/* make sure EIP is relocated */
4:
	/* set stack right */
	movl $(boot_stack + BOOT_STACK_SIZE), %esp
	/* reset gdtr:2 (grub:1) */
	lgdt gdt_descr
	/* set segments */
	ljmp $(__KERNEL_CS), $5f
5:	movl $(__KERNEL_DS), %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs

	/* setup idt
	 *
	 * Remember that before the kernel enables the interrupts, 
	 * it must load the initial address of the IDT table 
	 * into the idtr register and initialize all the entries of that table.
	 *
	 * Invokes setup_idt to fill the IDT with null interrupt handlers
	 */
	call setup_idt
	lidt idt_descr

	/* call C code
	 * The start_kernel completes the initialization of the kernel
	 */
	call start_kernel

boot_halt:
	hlt
	jmp boot_halt
	
/* default interrupt handler(just a iret) */
ignore_init:
	iret
setup_idt:
	lea ignore_init, %edx
	movl $(__KERNEL_CS << 16), %eax
	movw %dx, %ax /* now,eax contains low 32-bit of Interrupt Gate */
	movw $0x8E00, %dx /* edx: high 32-bit of Interrupt Gate */
	lea idt_table, %edi
	mov $256, %ecx
rp_sidt:
	movl %eax, (%edi)
	movl %edx, 4(%edi)
	addl $8, %edi
	dec %ecx
	jne rp_sidt
	ret


.globl idt
.globl gdt

	.align 16, 0x90
	.word 0
idt_descr:
	.word IDTENTRIES * 8 - 1
idt:
	.long idt_table

	.align 16, 0x90	
	.word 0
gdt_descr:
	.word GDTENTRIES * 8 - 1
gdt:
	.long gdt_table


.org 0x1000
ENTRY(swapper_pg_dir)
	.long 0x00102007
	.long 0x00103007
	.long 0x00104007
	.long 0x00105007
	.long 0x00106007
	.long 0x00107007
	.long 0x00108007
	.long 0x00109007
	/* default: 760 entries(768 - 8) */
	.fill BOOT_USER_PGD_PTRS-8, 4, 0
	.long 0x00102007
	.long 0x00103007
	.long 0x00104007
	.long 0x00105007
	.long 0x00106007
	.long 0x00107007
	.long 0x00108007
	.long 0x00109007
	/* default: 248 entries(256 - 8) */
	.fill BOOT_KERNEL_PGD_PTRS-8, 4, 0
.org 0x2000
ENTRY(pg0)
.org 0x3000
ENTRY(pg1)
.org 0x4000
ENTRY(pg2)
.org 0x5000
ENTRY(pg3)
.org 0x6000
ENTRY(pg4)
.org 0x7000
ENTRY(pg5)
.org 0x8000
ENTRY(pg6)
.org 0x9000
ENTRY(pg7)
.org 0xa000
ENTRY(empty_zero_page)
.org 0xb000


.data

	.align 16, 0x90
.globl gdt_table
gdt_table:
	.quad 0x0000000000000000        /* 0, NULL descriptor */
	.quad 0x0000000000000000        /* 1, not used */
	.quad 0x00cf9a000000ffff        /* 2, 0x10 kernel 4GB code at 0x00000000 */
	.quad 0x00cf92000000ffff        /* 3, 0x18 kernel 4GB data at 0x00000000 */
	.quad 0x00cffa000000ffff        /* 4, 0x23 user   4GB code at 0x00000000 */
	.quad 0x00cff2000000ffff        /* 5, 0x2b user   4GB data at 0x00000000 */
	.quad 0x0000000000000000        /* 6, not used */
	.quad 0x0000000000000000        /* 7, not used */
	.fill NR_CPUS * 2, 8, 0

.globl idt_table
idt_table:
	.fill 256, 8, 0

.comm boot_stack, BOOT_STACK_SIZE
