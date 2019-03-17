#include "rpi.h"
#include "gpio.h"

#define AUX_ENABLES 0x20215004
#define BAUD_VAL 270

//registers and base addresses from pg 8

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


void set_registers() {
	//disable tx/rx pg 17
	put32(&uart_mmio->IO, 0b0);
	//clear IO registers pg 11
	put32(&uart_mmio->IER, 0b0);
	//clear interrupts pg 12
	put32(&uart_mmio->IIR, 0b110);
	//clear rx/tx FIFO by setting bit 1 and 2 pg 13
	put32(&uart_mmio->LCR, 0b11);
	//write b11 for 8bit pg 14
	put32(&uart_mmio->MCR, 0b0);
	//read register set to 0, pg 15
	put32(&uart_mmio->LSR, 0b0);
	//good practice to clear, pg 15
	put32(&uart_mmio->MSR, 0b0);
	//good practice to clear, pg 16
	put32(&uart_mmio->SCRATCH, 0b0);
	//good practice to clear pg 18
	put32(&uart_mmio->STAT, 0b0);
	//formula on pg 11 for baud value, register pg 19
	put32(&uart_mmio->BAUD, BAUD_VAL);
}

// use this if you need memory barriers.
void dev_barrier(void) {
	dmb();
	dsb();
}



void uart_init(void) {

	dev_barrier();

	//lab 4 had gpio_init() but rpi.h doesn't define, also comment said it did nothing

	// gpio_init();
	// setting up gpio tx/rx pg 102
	gpio_set_function(GPIO_TX, GPIO_FUNC_ALT5);
	gpio_set_function(GPIO_RX, GPIO_FUNC_ALT5);
	// pg7 we need device barrier
	dev_barrier();
	
	// enable aux for mini uart pg 9
	put32((void*) AUX_ENABLES, 0b1);
	
	dev_barrier();
	//disable tx/rx, pg 17
	put32(&uart_mmio->CNTL, 0b0);
	//sets all important registers
	set_registers();
	//enable tx/rx, pg 17
	put32(&uart_mmio->CNTL, 0b11);
	dev_barrier();
}

int uart_getc(void) {
	// first bit of STAT says whether symbols available pg 19
	while((get32(&uart_mmio->STAT) & 0b1) == 0) {;}
	//get last byte of IO reg pg 11
	return get32(&uart_mmio->IO) & 0xFF;
}
void uart_putc(unsigned c) {
	//second bit of STAT says whether space available pg 19
	while((get32(&uart_mmio->STAT) & 0b10) == 0) {;}
	//set last byte of IO reg page 11
	put32(&uart_mmio->IO, c & 0xFF);
}
