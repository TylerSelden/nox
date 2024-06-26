#ifndef DRIVERS_PIC_H
#define DRIVERS_PIC_H


#include <stdint.h>

#define PIC1      0x20
#define PIC2      0xa0
#define PIC1_CMD  PIC1
#define PIC1_DATA PIC1 + 1
#define PIC2_CMD  PIC2
#define PIC2_DATA PIC2 + 1


void pic_end_int(uint8_t irq);


#define ICW1_ICW4       0x01
#define ICW1_SINGLE     0x02
#define ICW1_INTERVAL4  0x04
#define ICW1_LEVEL      0x08
#define ICW1_INIT       0x10
 
#define ICW4_8086       0x01
#define ICW4_AUTO       0x02
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM       0x10

void pic_disable();



void pic_init();



#endif
