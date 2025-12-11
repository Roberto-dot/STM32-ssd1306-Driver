#ifndef CONF_H
#define CONF_H

#include <stm32f446xx.h>

extern void confRCC(void);
extern void confGPIO(void);
extern void confI2C(void);
extern void setConf(void);
extern void confADC(void);
extern uint16_t convADC(void);


#endif