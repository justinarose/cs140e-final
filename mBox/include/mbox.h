#ifndef _MBOX_H
#define _MBOX_H

unsigned mbox_init(unsigned mboxNum);

unsigned mbox_send(unsigned cpuId, unsigned mboxNum, unsigned msg);

unsigned mbox_read(unsigned mboxNum);

void handle_mbox_irq(unsigned mboxNum);

#endif /* _MBOX_H */