#include "utils.h"
#include "mbox.h"
#include "peripherals/mbox.h"

unsigned IRQCNTLS[] = {CORE0_MBOX_IRQCNTL, CORE1_MBOX_IRQCNTL, CORE2_MBOX_IRQCNTL, CORE3_MBOX_IRQCNTL};

// unsigned CORE_X_MBOX_X_SET = {}

void handle_mbox_irq() {
	unsigned cpuId = getCore();

	unsigned irqSourceAddr = CORE0_IRQ_SOURCE + 4*cpuId;
	
	
}

unsigned mbox_init(unsigned mboxNum){

	unsigned cpuId = getCore();

	unsigned irqcntlAddr = IRQCNTLS[cpuId];

	unsigned val = get32(irqcntlAddr);

	val |= (1 << mboxNum);

	put32(irqcntlAddr, val);

	return 0;
}

unsigned mbox_send(unsigned cpuId, unsigned mboxNum, unsigned msg, unsigned long nbytes){

	// unsigned cpuId = getCore();

	unsigned core_x_mbox_x_set_addr = CORE0_MBOX0_SET + 4*4*cpuId + 4*mboxNum;
	unsigned core_x_mbox_x_rdclr_addr = CORE0_MBOX0_RDCLR + 4*4*cpuId + 4*mboxNum;
	put32(core_x_mbox_x_rdclr_addr, 0);
	put32(core_x_mbox_x_set_addr, msg);

	return 0;
}

unsigned mbox_read(unsigned mboxNum, unsigned* buf, unsigned long nbytes){


	// unsigned core_x_mbox_x_set_addr = CORE0_MBOX0_SET + 4*4*cpuId + 4*mboxNum;
	unsigned core_x_mbox_x_rdclr_addr = CORE0_MBOX0_RDCLR + 4*4*cpuId + 4*mboxNum;

	*buf = get32(core_x_mbox_x_rdclr_addr);

	return get32(irqSourceAddr);
}