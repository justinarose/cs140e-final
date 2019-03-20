SECTIONS
{
	.text.boot 0x0: { *(.text.boot) }
	.text :  { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) }
	bss_end = .;
}
