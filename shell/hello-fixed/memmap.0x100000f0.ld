LINK_ADDR = 0x100000f0;
SECTIONS
{
	/* our simple a.out header.  will add as needed. */
        .crt0_header LINK_ADDR : {
                /* tell bootloader which version. */
                LONG(2);
                /* where we are linked at */
                LONG(LINK_ADDR);
        }
	.text :  { 
		KEEP(*(.text.boot))  
		*(.text*) 
	}
    	.data : { 
		. = ALIGN(4);
		*(.data*) 
		. = ALIGN(4);
  	} 
    	.rodata : { 
		. = ALIGN(4);
		*(.rodata*)
		. = ALIGN(4);
	}

	.bss : {
        	__bss_start__ = .;
        	*(.bss*)
        	. = ALIGN(4);
		*(COMMON)
        	. = ALIGN(8);
        	__bss_end__ = .;
		. = ALIGN(8);
        	__heap_start__ = .;
    	}
}
