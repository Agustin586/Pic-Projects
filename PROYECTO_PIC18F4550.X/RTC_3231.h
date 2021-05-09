#ifndef XC_RTC3231_H
#define XC_RTC3231_H
#endif


#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif


_Bool mod_rtc=0,esc_rtc=0,val_rtc=0;
char hora=0,min=0,seg=0,dia=0,mes=0,year=0,pos_rtc=1;

//Enviar tiempo 
void RTC_send_hora(void);           // Envia el valor de la hora,min,seg

//Enviar fecha
void RTC_send_fecha(void);          // Envia el valor de el dia,mes,añio

//Lectura tiempo
void RTC_read_time(void);           // Lee la hora,...,etc
void RTC_read_date(void);           // Lee el dia,...,etc

