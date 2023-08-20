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
#include "pwm.h"
#include "LCD.h"

#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#define date 0x04
#define month 0x05
#define year 0x06
#define hour 0x02
#define min  0x01
#define sec  0x00

char s1_pot [10];
char dia_str [3];
char mes_str [3];
char ye_str [3];
char hora_str [3];
char min_str [3];
char sec_str [3];

uint8_t dia = 0x01, mes = 0x01, ye = 0x20, hora = 0x01, seg= 0x00, minutos=0x00;
uint8_t h,m,s,d,me,an;

void setup(void);
uint8_t mdc;
uint8_t mdc2;
uint8_t cont;
uint8_t ban;

uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void main(void)
{
    setup();
    PWM_Init();
    Lcd_Init();
    while(1)
    {
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        mdc = I2C_Master_Read(0);
        I2C_Master_Stop();
        I2C_Master_Start();
        I2C_Master_Write(0x53);
        mdc2 = I2C_Master_Read(0);
        I2C_Master_Stop();
        if(mdc2 == 1){
        if(mdc == 1)
        {
            CCPR1L = 125;
            CCPR2L = 0;
        
        // Minutos
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(min);      // Seleccionar la direccion para los años
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        m = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        // Segundos
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(sec);      // Seleccionar los segundos
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        s = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        }
        else 
        {
            //if(mdc2 == 1){
                CCPR1L = 0;
                CCPR2L = cont;
                if(cont == 125)
                    ban = 0;
                if(cont == 0)
                    ban = 1;

                if(ban == 1)
                    cont++;
                if(ban == 0)
                    cont--;
            //}
        }
        }
        else {
            CCPR1L = 0;
            CCPR2L = 0;
        }
        uint8_t min_decimal = bcd_to_decimal(m);
       uint8_t seg_decimal = bcd_to_decimal(s);
       
        Lcd_Clear();                         // Iniciar la LCD
        Lcd_Set_Cursor(1,1);                 // Posicion del cursor
        Lcd_Write_String("I/O:");            // Caracter a mostrar
        Lcd_Set_Cursor(1,6);                 // Posicionar el cursor 
        if(mdc2 == 1)
            Lcd_Write_String("I");            // Escribir el valor del potenciometro
        else
            Lcd_Write_String("O");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("IR:");          // Caracter a mostrar de fecha
        Lcd_Set_Cursor(2,5);
        if(mdc == 1)
            Lcd_Write_String("!");            // Escribir el valor del potenciometro
        else
            Lcd_Write_String("...");
        Lcd_Set_Cursor(1,12); 
        Lcd_Write_String("TDD:"); 
        Lcd_Set_Cursor(2,12);
        sprintf(min_str, "%d:",min_decimal);
        Lcd_Write_String(min_str); 
        Lcd_Set_Cursor(2,14);
        sprintf(sec_str, "%d",seg_decimal);
        Lcd_Write_String(sec_str); 
         __delay_ms(200);
    }

    return;
}
void setup(void)
{
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    TRISE = 0;
    
    
    OSCCONbits.IRCF = 0b111; //8 MHz
    OSCCONbits.SCS = 1;
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}