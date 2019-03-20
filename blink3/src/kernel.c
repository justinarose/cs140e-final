#include <stddef.h>
#include <stdint.h>

#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"
#include "sys.h"
#include "user.h"
#include "mm.h"


#define GPFSEL1 (VA_START + 0x3F200004)
#define GPSET0  (VA_START + 0x3F20001C)
#define GPCLR0  (VA_START + 0x3F200028)
void kernel_main()
{
	init_printf(0, putc);
	printf("Hello world from the program!\r\n");

	unsigned int ra;
	ra=get32(GPFSEL1);
	ra&=~(7<<18);
	ra|=1<<18;
	put32(GPFSEL1,ra);

	for(int i =0; i< 10; i++)
	{
			put32(GPSET0,1<<16);
			for(ra=0;ra<0x100000;ra++) burn();
			put32(GPCLR0,1<<16);
			for(ra=0;ra<0x100000;ra++) burn();
	}

}
