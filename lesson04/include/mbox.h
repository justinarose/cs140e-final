#ifndef _MBOX_H
#define _MBOX_H

unsigned mbox_init(unsigned mboxNum);

unsigned mbox_send(unsigned cpuId, unsigned mboxNum, unsigned msg, unsigned long nbytes);

unsigned mbox_read(unsigned mboxNum, unsigned* buf, unsigned long nbytes);


#endif /* _MBOX_H */