#ifndef __UART_H
#define	__UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

void initUART (uint8_t freq);
void BAUDSelect (uint8_t freq);
void txfunc (char data);
char rxfunc();
void string (char *str);
#endif
