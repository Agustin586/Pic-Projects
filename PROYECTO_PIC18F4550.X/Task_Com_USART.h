#ifndef XC_T_USART_H
#define	XC_T_USART_H
#endif

#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif

#ifndef XC_OS_CONFIG_H
#include "OS_CONFIG.h"
#endif

#ifndef XC_EEPROM_EX_H
#include "EEPROM_EX.h"
#endif

#ifndef XC_RTC3231_H
#include "RTC_3231.h"
#endif

#ifndef XC_Task1_H
    #include "Task1.h"
#endif

#ifndef STRING_H
    #include <string.h>
#endif

#include <stdio.h>

//Variables de comunicacion
char word[20],llave[4];
unsigned int xx=0,yy=0;
unsigned char address=0,long_cad=0,N=0,XX=0,YY=0;
_Bool CO=0,CR=0,PA=0,Fin_Cad=0;


#define Pu_1    LATEbits.LATE1
#define Pu_2    LATEbits.LATE0
#define E_Pu1   PORTDbits.RD6
#define E_Pu2   PORTDbits.RD7

_Bool Tarj_Dect=0,Ha_Pu=0;


_Bool Bluetooth_Com(void);           // Analiza el mensaje para determinar si es bluetooth
void Task_Com_USART(void);          // Analiza el mensaje y da una respuesta
void Read_Com(void);                // Lee el mensaje caracter por caracter
void ACN_CR_Com(void);              // Respuesta de consulta de referencia
void ACN_PA_Com(void);              // Respuesta de pedido de accion
void ACN_CO_Com(void);              // Respuesta de consulta



