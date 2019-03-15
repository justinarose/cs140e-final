#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"
#include "utils.h"
int waitingForMaster = 1;
int signalStop = 0;
void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			while(1){
				if(signalStop==0) break;
			}
			delay(100000);
		}
	}
}

void kernel_main(void)
{
	uart_init();
	irq_vector_init();
	timer_init();
	enable_interrupt_controller();

	unsigned c = 0;
	init_printf(0, putc);
	while(c != 's'){
		c = uart_recv();
	}
	printf("Hello from cpu 0!\r\n");

	waitingForMaster = 0;


	// unsigned c = 0;
	while(1) {
		c = uart_recv();
		if(c == 'z') signalStop = 1;
		if(c == 'c') signalStop = 0;
		uart_send(c);
	}



}

void kernel_child(void)
{
	enable_irq();
	irq_vector_init();
	timer_init();
	enable_interrupt_controller();

	init_printf(0, putc);
	unsigned int cpu_id = getCore();
	while(waitingForMaster) {;}
	delay(cpu_id*100000);
	printf("CPU %u reporting!\r\n", cpu_id);

	unsigned long msg1, msg2;

	if(cpu_id == 1) {
		msg1 = (unsigned long)"000";
		msg2 = (unsigned long)"111";
	} else if(cpu_id == 2) {
		msg1 = (unsigned long)"222";
		msg2 = (unsigned long)"333";
	} else if (cpu_id == 3) {
		msg1 = (unsigned long)"444";
		msg2 = (unsigned long)"555";
	} else {
		msg1 = (unsigned long)"PROBLEM";
		msg2 = (unsigned long)"UHOH";
	}

	int res = copy_process((unsigned long)&process, msg1);
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}
	res = copy_process((unsigned long)&process, msg2);
	if (res != 0) {
		printf("error while starting process 2");
		return;
	}
	while (1){
		schedule();
	}	
	// while(1) {;}
	// while (1) {
	// 	uart_send(uart_recv());
	// }
}