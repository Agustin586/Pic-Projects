//Pagina desde donde se saco la libreria: https://www.firtec.com.ar/cms/10-notas-tecnicas/14-xc8-ds18b20
#ifndef     DS18B20_H
 #define    DS18B20_H

#include <stdio.h>
#include <stdlib.h>

#define DQ_LOW		PORTBbits.RB7=0; TRISB = 0b00000000; 	
#define DQ_RELEASE	TRISB = 0b10000000;			
#define DQ_READ		PORTBbits.RB7;

#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

#define Ds18b20_Lect    Read_Temperature_DS18B20
#define Ds18b20_convert Convert_Temperature

/**
 * Genera un reseteo en el sensor --> envia un pulso en bajo durante un determinado tiempo
 */
int OWReset(void);                 
/**
 * Envia un bit al sensor
 * @parametro x: bit
 */
void OWWriteBit(int x);
/**
 * Lee un bit desde la memoria scrachpat del sensor
 * @return 
 */
int OWReadBit(void);
/**
 * Escribe un byte entero en la memoria del sensor
 * @param data
 */
void OWWriteByte(int data);
/**
 * Lee un byte desde la memoria del sensor
 * @return 
 */
int OWReadByte(void);
/**
 * Escribe un dato y devuelve un resultado
 * @param data
 * @return 
 */
int OWTouchByte(int data);
/**
 * Escribe una cantidad de bytes determinadas
 * @param data
 * @param data_len
 */
void OWBlock(unsigned char *data, int data_len);
/**
 * Inicia la conversion de temperatura
 */
void Convert_Temperature(void);
/**
 * Se encargar de leer la temperatura pero desde el sensor ds18s20
 * @return 
 */
float Read_Temperature_DS18S20(void);
/**
 * Se encarga de leer la temperatura desde el sensor ds18b20
 * @return 
 */
float Read_Temperature_DS18B20(void);

#endif