#include <xc.h>

#pragma CONFIG FOSC = INTOSCIO_EC

#pragma CONFIG WDT = OFF

#pragma CONFIG LVP = OFF

#pragma CONFIG PBADEN = OFF

#pragma CONFIG PLLDIV = 1

#pragma CONFIG CPUDIV = OSC1_PLL2

#pragma CONFIG USBDIV = 2

#pragma CONFIG FCMEN = OFF

#pragma CONFIG IESO = OFF

#pragma MCLRE = OFF

#define _XTAL_FREQ 8000000

void __interrupt() ISR(void) {

if (INTCONbits.INT0IF) {

LATDbits.LATD0 ^= 1;

INTCONbits.INT0IF = 0;

}

}
void main(void) {

OSCCONbits.IRCF = 0b111; // Set internal clock to 8 MHz

OSCCONbits.SCS = 0b10; // Select internal oscillator as system clock

TRISBbits.TRISB0 = 1; // RB0 as input (INT0)

TRISDbits.TRISD0 = 0; // RD0 as output

LATDbits.LATD0 = 1; // Initial LED state: ON

ADCON1 = 0x0F; // All analog inputs as digital

INTCONbits.GIE = 1; // Global interrupt enable

INTCONbits.INT0IE = 1; // Enable INT0

INTCONbits.INT0IF = 0; // Clear interrupt flag

INTCON2bits.INTEDG0 = 0; // Interrupt on falling edge of RB0

while (1) ;

}
