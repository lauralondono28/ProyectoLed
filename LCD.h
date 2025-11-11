#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xc.h>
#include <pic18f4550.h>
#include "Config.h"
    
/*********************definicion de puertos********************************/
#define RS LATDbits.LATD2
#define EN LATDbits.LATD3
#define ldata LATD
#define LCD_Port TRISD
   
#define CMD_CLEAR_LCD 0x01
    
// Prototipos de funciones
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char x);
void LCD_String(const char *msg);
void LCD_Clear(void);
void LCD_SetCursor(unsigned char row, unsigned char column);

#ifdef __cplusplus
}
#endif

#endif /* LCD_H */
