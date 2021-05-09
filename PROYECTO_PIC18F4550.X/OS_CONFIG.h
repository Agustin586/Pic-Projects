#ifndef XC_OS_CONFIG_H
#define	XC_OS_CONFIG_H
#endif

#include "CONFIG_PIC.h"

#ifndef XC_UART_COM_H
    #include "UART_com.h"
#endif

#ifndef XC_TECLADO_4X4_H
    #include "TECLADO_4X4.h"
#endif

#ifndef XC_T_USART_H
    #include "Task_Com_USART.h"
#endif

#ifndef XC_RTC3231_H
    #include "RTC_3231.h"
#endif

#ifndef XC_Task1_H
    #include "Task1.h"
#endif

#ifndef XC_SOFT_SPI_H
    #include "SPI_SOFT.h"
#endif


#define TICKS_T1        2       // 20ms  
#define TICKS_T2        7       // 70ms
#define TICKS_TEC       7       // Cada 70ms escanea el teclado
#define TICKS_RTC       100     // Cada 1 segundo lee el rtc
#define DELAY_LCD       300     // Cada 3 segundos cambia de pantalla
#define DELAY_MUESTRA   50      // Cada medio segundo actualizan los valores
#define TIMER_P1        700     // Durante 7 segundos queda activada la puerta 1
#define TIMER_P2        700     // Durante un tiempo queda actvada la puerta 2
#define TIMER_HASH      700     // Durante 7 segundo se queda grabado el hash
#define TICKS_RX        20      // 200ms para limpiar los datos del uart si no se recive nada

unsigned char Timer_Task1=TICKS_T1,Timer_Task2=TICKS_T2,Timer_Tec=TICKS_TEC,Blocked=0;
unsigned int  Timer_Rtc=TICKS_RTC,Timer_DelayLcd=DELAY_LCD,Timer_DelayMuestra=DELAY_MUESTRA,Timer_Puerta1=TIMER_P1,Timer_Puerta2=TIMER_P2,Timer_Hash=TIMER_HASH,Timer_RX=TICKS_RX;
//Tarea en 0: Tarea supendida//Tarea en 1: Tarea lista//Tarea en 2: Tarea corriendo//Tarea en 3: Tarea detenida
unsigned char key=0,Task1_Est=0,Task2_Est=0,Running=0;
_Bool puerta1=0,puerta2=0,Read_Eeprom=0,rx_recivido=0;


void OS_init(void);                                                             // Incializa la configuracion para la multitarea
void __interrupt(high_priority) ISR_HighP(void);                                // Interrupcion de alta prioridad
void __interrupt(low_priority)  ISR_LowP(void);                                 // Interrupcion de baja prioridad
void TaskDetected(void);                                                        // Reinicia los temporizadores y lleva a cabo algo