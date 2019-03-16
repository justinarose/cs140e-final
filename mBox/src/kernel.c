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
unsigned childReady[3];
// unsigned childReady;
unsigned counts[3];
unsigned signals[3];
char* addrs[3];

void kernel_main(void)
{
	init_printf(0, putc);
	while(1) {
		printf("testing\r\n");
	}
	uart_init();
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
	while(!(childReady[0] && childReady[1] && childReady[2])) {
		;
	}
	// delay(300000);

	for(unsigned i=0; i < 40; i++){
		delay(200000);
		mbox_send((i%4), (i%4), 999);
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
	// delay((cpu)*100000);
	for(unsigned i=0; i < 4; i++) {
		mbox_send(0, i, 21*cpu);
	}
	
	childReady[cpu-1] = 1;

	hang();

}