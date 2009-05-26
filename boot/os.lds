va = 0x100000;
pa = 0x100000;
OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
SECTIONS
{
	.text va : AT(pa) {
		code = .;
		*(.text)
		. = ALIGN(4096);
	}
	
	.data : AT(pa + (data - code)) {
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	_edata = .;
	
	.bss : AT(pa + (bss - code)) {
		bss = .;
		*(.bss)
		*(COMMON)
		. = ALIGN(4096);
	}
	_end = .;
}
