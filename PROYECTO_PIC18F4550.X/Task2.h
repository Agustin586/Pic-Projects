#ifndef XC_Task2_H
#define	XC_Task2_H
#endif


#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif

#ifndef XC_RTC3231_H
    #include "RTC_3231.h"
#endif


#define FNV_PRIME   16777619
#define FNV_BASIS   2166136261


unsigned int TagType;
long long codigo_rfid=0;
char UID[10];               // Almacena el buffer del codigo de rfid


void Task2(unsigned char *Maquina_Estado);                                      // Elije el estado que debe realizar
void E_Rfid_Proj(void);                                                         // Lee la tarjeta rfid
unsigned long  Hash_algoritmo(void);                                            // Convierte los datos de la tarjeta en un codigo hash de 10 diigtos
void E_Ingresos(void);                                                          // Permite la apertura de las puertas bajo ciertas condiciones