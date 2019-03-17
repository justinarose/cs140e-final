#include "rpi.h"
#include "uart.h"
#include "gpio.h"
#include "mem-barrier.h"

#define AUX_ENABLES 0x20215004
#define BAUD_VAL 270

struct aux_periph {
	unsigned
		IO,
		IER,
		IIR,
		LCR,
		MCR,
		LSR,
		MSR,
		SCRATCH,
		CNTL,
		STAT,
		BAUD;
} *uart_mmio = (void*) 0x20215040;

void enable_aux() {
	put32((void*) AUX_ENABLES, 0b1);
}

void set_registers() {
	put32(&uart_mmio->IO, 0b0);
	put32(&uart_mmio->IER, 0b0);
	put32(&uart_mmio->IIR, 0b110);
	put32(&uart_mmio->LCR, 0b11);
	put32(&uart_mmio->MCR, 0b0);
	put32(&uart_mmio->LSR, 0b0);
	put32(&uart_mmio->MSR, 0b0);
	put32(&uart_mmio->SCRATCH, 0b0);
	put32(&uart_mmio->CNTL, 0b0);
	put32(&uart_mmio->STAT, 0b0);
	put32(&uart_mmio->BAUD, BAUD_VAL);
}

// use this if you need memory barriers.
void dev_barrier(void) {
	dmb();
	dsb();
}



void uart_init(void) {

	dev_barrier();
	gpio_init();
	gpio_set_function(GPIO_TX, GPIO_FUNC_ALT5);
	gpio_set_function(GPIO_RX, GPIO_FUNC_ALT5);
	dev_barrier();
	
	enable_aux();
	dev_barrier();
	put32(&uart_mmio->CNTL, 0b0);
	set_registers();
	put32(&uart_mmio->CNTL, 0b11);
	dev_barrier();
}

int uart_getc(void) {

	while((get32(&uart_mmio->STAT) & 0b1) == 0) {;}

	return get32(&uart_mmio->IO) & 0xFF;
}
void uart_putc(unsigned c) {

	while((get32(&uart_mmio->STAT) & 0b10) == 0) {;}

	put32(&uart_mmio->IO, c);
}
