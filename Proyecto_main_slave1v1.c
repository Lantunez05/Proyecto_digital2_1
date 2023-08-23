// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "I2C.h"
//#include "pwm.h"

#define _XTAL_FREQ 8000000
uint8_t z;
int x = 0;
int st_sens = 0;
uint8_t mDC;

// Prototipos
void setup (void);
//-------------Interrupcion---------------------
void __interrupt() isr(void){
    
    if(PIR1bits.SSPIF == 1)
    {
        SSPCONbits.CKP = 0;
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL))
        {
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }
        
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) // Lectura
        {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci?n recepci?n/transmisi?n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci?n se complete
            mDC = SSPBUF;                // Guardar en el PORTD el valor del buffer de recepci?n
            __delay_us(250);
        }
        
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) // Escritura
        {
            z = SSPBUF;
            BF = 0;
            SSPBUF = mDC;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        
        PIR1bits.SSPIF = 0;
    }
}

void main(void) {
    setup();
    while (1) {
        if (RB0 == 1 && x == 0) 
        {
            while (RB0 == 1);
            x = 1;
            if (x == 1)
            {
                
                mDC = 1;
                RE0 = 1;
                __delay_ms(200);
            
            }
            
            
        } 
        if (RB0 == 1 && x == 1) 
        {                       // Cambiado a verificar si RB0 está en 1 cuando x es 1
            while (RB0 == 1);
            x = 0;
            if (x == 0)
            {
                mDC = 0;            
                RE0 = 0;            
            __delay_ms(200);
            }
        }
        
    }
    return;
}


void setup(void)
{
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    PORTB = 0;
    
    TRISE = 0;   // All bits of Port E as outputs
    PORTE = 0;   // Initialize Port E to low
    
    // Oscilador 
    OSCCONbits.IRCF =0b111; 
    OSCCONbits.SCS = 1; 
    
    I2C_Slave_Init(0x50);   
    return;
}

