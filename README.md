# ProyectoLed
;=========================================================
; Código en Assembler para PIC18F4550
; LED en RB0 encendido 5 segundos, apagado 2 segundos
; Usa retardos sin interrupciones ni Timer0
; Frecuencia: 8 MHz (Oscilador Interno)
; Ensamblador: MPLAB XC8 3.0
;=========================================================

    #include <xc.inc>   ; Definiciones del ensamblador para PIC18F4550

    ; Configuración de bits de configuración (Fuses)
    CONFIG  FOSC = INTOSCIO_EC   ; Oscilador interno 8 MHz
    CONFIG  WDT = OFF            ; Deshabilitar Watchdog Timer
    CONFIG  LVP = OFF            ; Deshabilitar programación en bajo voltaje
    CONFIG  PBADEN = OFF         ; PORTB como digital

    ;===============================================
    ; Vector de Inicio
    ;===============================================
    PSECT  resetVec, class=CODE, reloc=2
    ORG     0x00
    GOTO    Inicio

    ;===============================================
    ; Código Principal
    ;===============================================
    PSECT  main_code, class=CODE, reloc=2

Inicio:
    CLRF    TRISB       ; Configurar PORTB como salida
    CLRF    LATB        ; Inicializar PORTB en 0 (LED apagado)

Loop:
    BSF     LATB, 0     ; Encender LED en RB0
    CALL    Retardo_5s  ; Esperar 5 segundos

    BCF     LATB, 0     ; Apagar LED en RB0
    CALL    Retardo_2s  ; Esperar 2 segundos

    GOTO    Loop        ; Repetir ciclo

;===============================================
; Subrutina de Retardo de 1 Segundo (base)
;===============================================
Retardo_1s:
    MOVLW   25
    MOVWF   ContadorExterno

LoopExterno:
    MOVLW   250
    MOVWF   ContadorInterno

LoopInterno:
    NOP
    NOP
    NOP
    
    DECFSZ  ContadorInterno, F
    GOTO    LoopInterno

    DECFSZ  ContadorExterno, F
    GOTO    LoopExterno

    RETURN

;===============================================
; Retardo de 5 segundos = 5 * 1s
;===============================================
Retardo_5s:
    MOVLW   5
    MOVWF   R5Segundos

Ret5sLoop:
    CALL    Retardo_1s
    DECFSZ  R5Segundos, F
    GOTO    Ret5sLoop
    RETURN

;===============================================
; Retardo de 2 segundos = 2 * 1s
;===============================================
Retardo_2s:
    MOVLW   2
    MOVWF   R2Segundos

Ret2sLoop:
    CALL    Retardo_1s
    DECFSZ  R2Segundos, F
    GOTO    Ret2sLoop
    RETURN

;===============================================
; Variables en RAM
;===============================================
    PSECT udata
ContadorExterno: DS 1
ContadorInterno: DS 1
R5Segundos:      DS 1
R2Segundos:      DS 1

    END
