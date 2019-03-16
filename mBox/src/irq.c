#include "utils.h"
#include "printf.h"
#include "timer.h"
#include "entry.h"
#include "peripherals/irq.h"
#include "peripherals/mbox.h"
#include "mbox.h"
const char *entry_error_messages[] = {
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",		
	"FIQ_INVALID_EL1t",		
	"ERROR_INVALID_EL1T",		

	"SYNC_INVALID_EL1h",		
	"IRQ_INVALID_EL1h",		
	"FIQ_INVALID_EL1h",		
	"ERROR_INVALID_EL1h",		

	"SYNC_INVALID_EL0_64",		
	"IRQ_INVALID_EL0_64",		
	"FIQ_INVALID_EL0_64",		
	"ERROR_INVALID_EL0_64",	

	"SYNC_INVALID_EL0_32",		
	"IRQ_INVALID_EL0_32",		
	"FIQ_INVALID_EL0_32",		
	"ERROR_INVALID_EL0_32"	
};

void enable_interrupt_controller()
{
	// unsigned cpuId = getCore();
	// printf("Enabling interrupts for %u\r\n", cpuId);
	put32(ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1);
	// put32(ENABLE_BASIC_IRQS, ARM_MBOX_IRQ);
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
	printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr, address);
}

void handle_irq(void)
{
	// unsigned int irq = get32(IRQ_BASIC_PENDING);
	unsigned cpuId = getCore();
	// printf("cpu %u handler\r\n", cpuId);
	printf("%u\r\n", cpuId);
	unsigned irqSourceAddr = CORE0_IRQ_SOURCE + 4*cpuId;
	unsigned src = get32(irqSourceAddr);
	unsigned val = (src >> 4) & (0b1111);
	unsigned mboxNum = 0;
	while(!(val & 1 << mboxNum)) {
		mboxNum++;
	}
	// printf("SRC: %u\r\n", src);
	// switch (irq) {
	// 	case (ARM_MBOX_IRQ):
	// 		handle_mbox_irq();
	// 		break;
	// 	default:
	// 		printf("Unknown pending irq: %x\r\n", irq);
	// }
	// printf("IRQ is %x\r\n", irq);
	// if(irq & ARM_MBOX_IRQ) {
	// if(val) {
	// 	// printf("received mbox interrupt on cpu %u mbox %u\r\n", cpuId, mboxNum);
	// 	handle_mbox_irq(mboxNum);
	// } 
	// unsigned irq = get32(IRQ_PENDING_1);
	// switch (irq) {
	// 	case (SYSTEM_TIMER_IRQ_1):
	// 		handle_timer_irq();
	// 		break;
	// 	default:
	// 		printf("Unknown pending irq: %x\r\n", irq);
	// }

}
