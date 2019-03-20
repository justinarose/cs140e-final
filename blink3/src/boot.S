#include "arm/mmu.h"
#include "arm/sysregs.h"
#include "mm.h"
#include "peripherals/base.h"

.section ".text.boot"

.globl _start
_start:
	mov x20, x30
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero
	bl kernel_main
	mov x30, x20
	ret
