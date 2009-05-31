/* REF: MLXOS :-) */

LOAD_OFFSET = 0xC0000000;

OUTPUT_FORMAT("elf32-i386")

ENTRY(_pa_start)
SECTIONS
{
	. = (LOAD_OFFSET + 0x100000);
	_pa_start = _va_start - LOAD_OFFSET;

	_text = .;
	.text : AT(ADDR(.text) - LOAD_OFFSET) {
		multiboot.o(.text)
		*(.text)
	} = 0x9090
	_etext = .;

	.rodata : AT(ADDR(.rodata) - LOAD_OFFSET) {
		*(.rodata)
		*(.rodata.*)
	}

	. = ALIGN(16);
	.data : AT(ADDR(.data) - LOAD_OFFSET) {
		*(.data)
		CONSTRUCTORS
	}
	_edata = .;

	. = ALIGN(4096);
	.data.page_aligned : AT(ADDR(.data.page_aligned) - LOAD_OFFSET) {
		 *(.data.idt) 
	}

	. = ALIGN(16);
	__bss_start = .;
	.bss : AT(ADDR(.bss) - LOAD_OFFSET) {
		*(.bss)
	}
	_end = .;

	/* Sections to be discarded */
	/DISCARD/ : {
		*(.text.exit)
		*(.data.exit)
		*(.exitcall.exit)
	}

	/* Stabs debugging sections.  */
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
}
