#ifndef XC_SOFT_I2C_H
#define XC_SOFT_I2C_H
#endif


#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 48000000
#endif
#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif


#define SDA_TRIS    TRISCbits.TRISC0
#define SCL_TRIS    TRISCbits.TRISC1
#define SDA_PIN     PORTCbits.RC0
#define SCL_PIN     PORTCbits.RC1

_Bool ACK_EST=0;

void i2c_init(void);            // Inicializa el i2c
void i2c_start(void);           // Condicion de start de la comunicacion
void i2c_stop(void);            // Condicion de stopt de la comunicacion
void i2c_restart(void);         // Condicion de restart
void i2c_write(char date);      // Envia un caracter por i2c
char i2c_read(void);            // Lee un caracter por i2c
void i2c_ack(void);             // Condicion de ack
void i2c_nack(void);            // Condicion de nack 
void i2c_clock(char speed);     // Clock del i2c
void i2c_array(const char *);   // Envia una cadena de caracteres