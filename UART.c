#include "UART.h"

void initUART (uint8_t freq){
    SYNC = 0;
    SPEN = 1;
    TX9 = 0;
    TXEN = 1;
    PIR1bits.TXIF = 0;
    RX9 = 0; 
    CREN = 1;
    PIR1bits.TXIF = 0;
}

void BAUDSelect (uint8_t freq){
    switch(freq) {
        case 4:
            BRGH = 1;
            BRG16 = 0;
            SPBRG = 103;
            SPBRGH = 0;
            break;
        case 8:
            BRGH = 0;
            BRG16 = 0;
            SPBRG = 12;
            SPBRGH = 0;
            break;
        default:
            break;    
    }
}

void txfunc (char data){
    while(TRMT == 0);
    TXREG = data;
}

char rxfunc(){
    return RCREG;
}

void string (char *str){
    while(*str != '\0'){
        txfunc(*str);
        str++;
    }
}