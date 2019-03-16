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

int waitingForMaster = 1;
unsigned counts[3];
int signals[3];

char* addrs[3];
// void process(char *array)
// {
// 	while (1){
// 		for (int i = 0; i < 3; i++){
// 			uart_send(array[i]);
// 			while(1){
// 				if(signalStop==0) break;
// 			}
// 			delay(100000);
// 		}
// 	}
// }


void kernel_main(void)
{
	uart_init();

	unsigned c = 0;
	init_printf(0, putc);
	while(c != 's'){
		c = uart_recv();
	}
	printf("Hello from cpu 0!\r\n");
	// mbox_init(0);
	// printf("%u\r\n", get32((unsigned long) 0x40000030));

	char* buf1 = (char*) get_free_page();
	char* buf2 = (char*) get_free_page();
	char* buf3 = (char*) get_free_page();
	memzero(buf1, PAGE_SIZE);
	memzero(buf2, PAGE_SIZE);
	memzero(buf3, PAGE_SIZE);
	if(!buf1 || !buf2 || !buf3) {
		printf("ERROR Malloc!!!\r\n");
		return;
	}

	addrs[0] = buf1;
	addrs[1] = buf2;
	addrs[2] = buf3;

	waitingForMaster = 0;
	unsigned curr = 0;
	char* addr = 0;
	while(1) {
		c = uart_recv();
		unsigned n = c- '0';
		if(c=='3' || c == '1' || c=='2') {
			curr = n;
			printf("Starting with %u\r\n", curr);
			addr = addrs[curr-1];
			break;
		}
	}
	while(1) {
		c = uart_recv();
		unsigned n = c- '0';
		if(c=='3' || c == '1' || c=='2') {
			printf("Switching from %u to %u\r\n", curr, n);
			curr = n;
			addr = addrs[curr-1];
		} else if(c == '0') {
			printf("Turning on %u\r\n", curr);
			signals[curr-1] = 1;
		} else if(c=='9') {
			printf("Stopping %u\r\n", curr);
			signals[curr-1] = 0;
			counts[curr-1] = 0;
			// addr[0] = '\0';
			memzero(addr, PAGE_SIZE);
		} else {
			if(addr!=0) {
				addr[counts[curr-1]] = c;
			}
			counts[curr-1]++;
			uart_send(c);
		}
		
	}
}

void kernel_child(void)
{
	unsigned int cpu_id = getCore();
	while(waitingForMaster) {;}
	delay((cpu_id)*100000);
	printf("CPU %u woke up!\r\n", cpu_id);
	while(1) {
		if(signals[cpu_id-1]==1) {
			printf("%s\r\n", addrs[cpu_id-1]);
		}
		delay(2000000);
	}
	// irq_vector_init();
	// timer_init();
	// enable_interrupt_controller();
	// enable_irq();
	// printf("%s got 1\r\n", msg1);
	// int res = copy_process((unsigned long)&process, msg1);
	// if (res != 0) {
	// 	printf("error while starting process 1");
	// 	return;
	// }
	// printf("%s got 2\r\n", msg1);
	// res = copy_process((unsigned long)&process, msg2);
	// if (res != 0) {
	// 	printf("error while starting process 2");
	// 	return;
	// }
	// printf("%s got 3\r\n", msg1);
	// while (1){
	// 	printf("%s got loop\r\n", msg1);
	// 	schedule();
	// }	

	// while (1) {
	// 	uart_send(uart_recv());
	// }
}