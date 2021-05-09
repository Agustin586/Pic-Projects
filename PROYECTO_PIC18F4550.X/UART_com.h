#ifndef XC_UART_COM_H
#define XC_UART_COM_H
#endif


#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 48000000
#endif
#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif


void UART_init(unsigned int BAUD);          // Inicializa las configuraciones
void UART_write(char dato);                 // Escribe un caracter
void UART_array(const char* cadena);        // Envia una cadena de caracteres
char UART_read(void);                       // Lee un caracter
