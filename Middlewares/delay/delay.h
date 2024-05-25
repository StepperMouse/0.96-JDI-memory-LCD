#ifndef __DELAY_H   
#define __DELAY_H

#include <stdio.h>

void DelayInit(void);

void Delay_us(unsigned long nms);

void Delay_ms(unsigned int nms);

void Delay_s(unsigned int ns);

#endif
