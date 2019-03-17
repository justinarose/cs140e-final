#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"
#include "utils.h"
#include "mm.h"
#include "mbox.h"
#include "peripherals/irq.h"
#include "peripherals/mbox.h"
unsigned waitMaster = 1;
unsigned waitChild[3] = {1,1,1};


void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);
	// while(1) {
	// 	printf("testing\r\n");
	// }
	
	unsigned c = 0;
	
	while(c != 's'){
		c = uart_recv();
	}
	printf("MASTER!\r\n");
	for(unsigned i = 0; i < 4; i++) {
		mbox_init(i);
	}
	irq_vector_init();
	enable_irq();
	waitMaster = 0;
	while(waitChild[0] || waitChild[1] || waitChild[2]) {
		;
	}
	// delay(300000);
	unsigned msg;
	for(unsigned i=0; i < 40; i++){
		delay(200000);
		msg = i;
		mbox_sendA((i%4), (i%4), &msg);
	}

	while(1) {;}
}

void kernel_child(void)
{
	for(unsigned i = 0; i < 4; i++) {
		mbox_init(i);
	}
	irq_vector_init();
	enable_irq();

	unsigned cpu = getCore();

	while(waitMaster) {;}
	delay((cpu)*100000);
	printf("CPU %u\r\n", cpu);
	unsigned msg;
	for(unsigned i=0; i < 4; i++) {
		delay(200000);
		msg = (10*cpu)+i;
		mbox_sendA(0, i, &msg);
	}
	
	waitChild[cpu-1] = 0;

	hang();

}