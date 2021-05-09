#ifndef XC_TECLADO_4X4_H
#define XC_TECLADO_4X4_H
#endif


#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif


#define KBI0            TRISBbits.TRISB4
#define KBI1            TRISBbits.TRISB5
#define KBI2            TRISBbits.TRISB6
#define KBI3            TRISBbits.TRISB7

#define ESC_TRIS1       TRISAbits.TRISA0
#define ESC_TRIS2       TRISAbits.TRISA1
#define ESC_TRIS3       TRISAbits.TRISA2
#define ESC_TRIS4       TRISAbits.TRISA3
#define ESC_PORT1       PORTAbits.RA0
#define ESC_PORT2       PORTAbits.RA1
#define ESC_PORT3       PORTAbits.RA2
#define ESC_PORT4       PORTAbits.RA3
#define ESC_PIN1        LATAbits.LA0        
#define ESC_PIN2        LATAbits.LA1
#define ESC_PIN3        LATAbits.LA2
#define ESC_PIN4        LATAbits.LA3


unsigned char numero=0;
unsigned int valor=0;
_Bool tec_p=0;


void TEC4x4_init(void);     // Configuraciones iniciales para el teclado
void escaneo(void);         // Realiza el escaneo cada cierto tiempo
void antirrebote(void);     // Genera el tiempo necesario para el antirrebote
