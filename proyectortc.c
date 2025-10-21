#include <xc.h>

#pragma config FOSC = INTOSCIO_EC
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config PLLDIV = 1
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config MCLRE = OFF

#define _XTAL_FREQ 8000000

// Variables globales
volatile unsigned char sequence = 0;    // Secuencia actual (0-2)
volatile unsigned char speed = 2;       // Velocidad inicial (0-4)
volatile unsigned char btn_pressed = 0; // Flag de botón presionado
volatile unsigned char last_RB4 = 0;    // Estado anterior de RB4

// Array de velocidades (valores en ms)
const unsigned int speed_levels[] = {1000, 500, 250, 125, 62};

// Función de delay personalizada que acepta variables
void custom_delay(unsigned int ms) {
    for(unsigned int i = 0; i < ms; i++) {
        for(unsigned int j = 0; j < 100; j++) {
            __delay_us(10); // Delay de 10 microsegundos
        }
    }
}

void __interrupt() ISR(void) {
    // Interrupción por INT0 (Cambio de secuencia - RB0)
    if (INTCONbits.INT0IF) {
        sequence = (sequence + 1) % 3; // Cicla entre 0,1,2
        btn_pressed = 1;
        INTCONbits.INT0IF = 0;
    }
    
    // Interrupción por RB4-RB7 (Cambio de velocidad - RB4)
    if (INTCONbits.RBIF) {
        // Verificar si RB4 cambió
        if (PORTBbits.RB4 == 0 && last_RB4 == 1) { // Flanco de bajada
            speed = (speed + 1) % 5;  // Aumenta velocidad
            btn_pressed = 1;
        }
        else if (PORTBbits.RB4 == 1 && last_RB4 == 0) { // Flanco de subida  
            speed = (speed > 0) ? speed - 1 : 4; // Disminuye velocidad
            btn_pressed = 1;
        }
        
        last_RB4 = PORTBbits.RB4; // Guardar estado actual
        INTCONbits.RBIF = 0;      // Limpiar flag
    }
}

void main(void) {
    // Configuración del reloj
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
    
    // Configuración de puertos
    TRISBbits.TRISB0 = 1;    // RB0 como entrada (INT0 - primer botón)
    TRISBbits.TRISB4 = 1;    // RB4 como entrada (segundo botón)
    TRISD = 0x00;            // Puerto D completo como salida (LEDs)
    LATD = 0x00;             // Inicializar LEDs apagados
    
    // Configuración ADC
    ADCON1 = 0x0F;           // Todos los pines digitales
    
    // Configuración de interrupciones para INT0
    INTCONbits.GIE = 1;      // Habilitar interrupciones globales
    INTCONbits.INT0IE = 1;   // Habilitar INT0
    INTCONbits.INT0IF = 0;   // Limpiar flag INT0
    INTCON2bits.INTEDG0 = 0; // INT0 en flanco de bajada
    
    // Configuración de interrupción por cambio en PORTB (para RB4-RB7)
    INTCONbits.RBIE = 1;     // Habilitar interrupción por cambio en RB4-RB7
    INTCONbits.RBIF = 0;     // Limpiar flag
    
    // Leer estado inicial de RB4
    last_RB4 = PORTBbits.RB4;
    
    // Variables locales
    unsigned char led_pattern;
    unsigned char i;
    
    while(1) {
        switch(sequence) {
            case 0: // Secuencia 1: Carrera derecha-izquierda
                led_pattern = 0b0001;
                for(i = 0; i < 4; i++) {
                    LATD = (unsigned char)(led_pattern << i);
                    custom_delay(speed_levels[speed]);
                    if(btn_pressed) break;
                }
                break;
                
            case 1: // Secuencia 2: Carrera izquierda-derecha
                led_pattern = 0b1000;
                for(i = 0; i < 4; i++) {
                    LATD = (unsigned char)(led_pattern >> i);
                    custom_delay(speed_levels[speed]);
                    if(btn_pressed) break;
                }
                break;
                
            case 2: // Secuencia 3: Alternancia
                LATD = 0b0101;
                custom_delay(speed_levels[speed]);
                if(btn_pressed) break;
                LATD = 0b1010;
                custom_delay(speed_levels[speed]);
                break;
        }
        
        btn_pressed = 0; // Resetear flag
    }
}