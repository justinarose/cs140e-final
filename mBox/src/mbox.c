#include "utils.h"
#include "mbox.h"
#include "peripherals/mbox.h"
#include "printf.h"
// unsigned IRQCNTLS[] = {CORE0_MBOX_IRQCNTL, CORE1_MBOX_IRQCNTL, CORE2_MBOX_IRQCNTL, CORE3_MBOX_IRQCNTL};

void handle_mbox_irq(unsigned mboxNum) {
	unsigned cpuId = getCore();

	// unsigned irqSourceAddr = CORE0_IRQ_SOURCE + 4*cpuId;
	// unsigned src = get32(irqSourceAddr);
	// unsigned mboxNum = (src >> 4) & (0b1111);

	unsigned msg = mbox_read(mboxNum);

	printf("Core %u got %u\r\n", cpuId, msg);
}

unsigned mbox_init(unsigned mboxNum){

	unsigned cpuId = getCore();

	unsigned irqcntlAddr = CORE0_MBOX_IRQCNTL + 4*cpuId;

	unsigned val = get32(irqcntlAddr);
	// printf("irqcntl %u\r\n", (val&0xF)!=0);


	val |= (1 << mboxNum);

	put32(irqcntlAddr, val);

	// dsb();
	// printf("Initialized mbox %u for core %u\r\n", mboxNum, cpuId);
	return 0;
}

unsigned mbox_send(unsigned cpuId, unsigned mboxNum, unsigned msg){

	// unsigned cpuId = getCore();
	// printf("sending %u to mbox %u of cpu %u\r\n", msg, mboxNum, cpuId);
	unsigned core_x_mbox_x_set_addr = CORE0_MBOX0_SET + 4*4*cpuId + 4*mboxNum;
	// unsigned core_x_mbox_x_rdclr_addr = CORE0_MBOX0_RDCLR + 4*4*cpuId + 4*mboxNum;
	// put32(core_x_mbox_x_rdclr_addr, 0);
	put32(core_x_mbox_x_set_addr, msg);

	return 0;
}

unsigned mbox_read(unsigned mboxNum){

	unsigned cpuId = getCore();
	// printf("cpu %u reading from mbox %u\r\n", cpuId, mboxNum);
	// unsigned core_x_mbox_x_set_addr = CORE0_MBOX0_SET + 4*4*cpuId + 4*mboxNum;
	unsigned core_x_mbox_x_rdclr_addr = CORE0_MBOX0_RDCLR + 4*4*cpuId + 4*mboxNum;

	unsigned toReturn = get32(core_x_mbox_x_rdclr_addr);
	put32(core_x_mbox_x_rdclr_addr, toReturn);
	return toReturn;

}