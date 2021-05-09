#include "Configuracion_Bits.h"
#include "Pwm_Hard.h"

////////////////////////////////////////////////////////////////////////////////

void PWM_init_1(unsigned int frecuencia)     //Frecuencia minima 246 HZ - Frecuencia maxima 62500 HZ todo esto con 4 MHz
{
    float per=0;
    
    //Transforma a periodo
    per = 1.0 / frecuencia;
    freq = frecuencia;
    
    //Configura Modo pwm
    CCP1CONbits.CCP1M = 0b1100;
    
    //Carga el valor inicial en el pwm
    PR2 = (int) (per / (4 * (1.0 / _XTAL_FREQ) * 16)) - 1;
    pr2 = PR2;
    CCPR1L = 0;                         // Limpia el registro ccpr1l     LBS o LSB--> bit menos significativo
    CCP1CONbits.DC1B1 = 0;              // Bit1 = 0
    CCP1CONbits.DC1B0 = 0;              // Bit0 = 0
    
    //Configuracion timer2
    T2CONbits.TOUTPS = 0b0000;          // Postescaler 1:1
    T2CONbits.T2CKPS = 0b11;            // Preescaler 1:16
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;               // Habilita Timer2

    //Configuracion salida
    TRISCbits.TRISC2 = 0;               // Declara salida pwm
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void PWM_1(unsigned char duty) 
{
    float per=0,pulso_alto=0,aux=0;
    unsigned int pwm=0;
    
    TMR2ON = 1;
    
    per = 1.0 / freq;                       // Obtiene el periodo de la frecuencia elegida
    if(duty == 100) duty = 95;
    pulso_alto = (duty * per) / 100.0;      // per_total___100%
                                            // pulso_alto__duty
                                        
    //Carga el valor necesario en CCPR1H    
    aux = ((pulso_alto) / ((1.0/_XTAL_FREQ) * 16)); // Con 50 us --> aux = 12.5
    pwm = (int)aux;
    
    CCP1CONbits.DC1B = 0b00;                // Limpia los lsb 
    //CCP1CON |= ((pwm & 0x03) << 4);       // Carga el valor a los lbs del pwm (1)
    CCP1CONbits.DC1B = (pwm & 0x03);
    
    CCPR1L &= 0x00;                         // Limpiar registro ccp1l 
    CCPR1L |= (pwm & 0x3FC) >> 2;           // Carga el valor en ccp1l (2)
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void PWM_Stop_1(void)     //Funcion de apagado del PWM
{
    Pwm1_set(1);
    T2CONbits.TMR2ON = 0;               // Detiene la cuenta del tmr2
    CCPR1L = 0;                         // Limpia el registro ccpr1l
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;              // Limpia el lbs del pwm 1

    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que inicializa el PWM 2 
void PWM_init_2(unsigned int frecuencia)     //Configura el PWM
{
    float per=0;
    
    //Transforma a periodo
    per = 1.0 / frecuencia;
    
    //Valor del PR2
    PR2 = (per / (4 * 1.0 / _XTAL_FREQ * 16)) - 1;                               
    CCPR2L = 0;
    CCP2CONbits.DC2B1 = 0;
    CCP2CONbits.DC2B0 = 0;
    
    //Configuracion salida
    TRISCbits.TRISC1 = 0;               //Declara salida pwm

    //Configuracion PWM
    CCP2M3 = 1;                         //Modo PWM
    CCP2M2 = 1;                         // -
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que reliza otro pwm y se le envia el porcentaje del duty requerido
void PWM_2(char duty)
{
    float per=0,pulso_alto=0,aux=0;
    unsigned int pwm=0;
    
    //Calcula el preiodo de la frecuencia o señal
    per = 1.0 / freq; 
    //Hace que el duty no pase del periodo maximo real
    if(duty == 100) duty = 95;
    //Calcula el porcentaje del priodo del pulso en alto
    pulso_alto = (duty * per) / 100.0;      // per_total___100%
                                            // pulso_alto__duty
                                        
    //Carga el valor necesario en CCPR1H    
    aux = ((pulso_alto) / ((1.0/_XTAL_FREQ) * 16));
    pwm = (int)aux;
    CCP2CON &= 0x0F;
    CCP2CON |= ((pwm & 0x03) << 4);
    CCPR2L &= 0x00;
    CCPR2L |= (pwm & 0x3FC) >> 2;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que detiene el Pwm 2
void PWM_Stop_2(void)     //Funcion de apagado del PWM
{
    TMR2ON = 0;  
    CCPR2L = 0;
    CCP2CONbits.DC2B1 = 0;
    CCP2CONbits.DC2B0 = 0;
    
    return;
}

