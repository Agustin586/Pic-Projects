/*
 * /////////////////////////////////////////////////////////////////////////////
 * 
 *                          LEER ANTES DE CONTINUAR
 * 
 * /////////////////////////////////////////////////////////////////////////////
 *  
 * File:   Pwm_Soft_Tmr1.h
 * Author: Zuliani,Agustin
 *
 * Created on 5 de noviembre de 2020, 19:08
 *
 * Descripcion: Esta libreria permite controlar todos los pwm que se quieran 
 * pudiendo variar su frecuencia y duty de cada uno por separado sin influir
 * sobre el otro. Se utilizó un timer 1 para controlar cada variable PwmSx la 
 * cual se va comparando con un valor llamado P_W_TSx el cual puede irse modfi-
 * cando segun convenga, llamando a la funcion PwmSx_set(porcentaje duty).
 * Tambien podría ir cambiandose la frecuencia llamando a la funcion de init 
 * correspondiente a cada Pwm.
 * 
 * Importante: 
 * - Para la salida en PIC18F se ultiliza los registros LATx, con la finalidad
 * de poder evitar el ruido causado. Estos registros no existen en PIC16F, por 
 * lo tanto se deberá cambiar ese registro por un PORTx.
 * - La libreria viene configurada solo con 4 Pwm, si se quiere utilizar mas 
 * se debera seguir el mismo criterio que con los demas:
 *      1) Definir parametros como P_W_T_Sx,PER_T_Sx,....,demas.
 *      2) Se deberan crear funciones que actuen sobre estos parametros.
 *      3) Se debera incluir en la interrupcion dicho contador para que sume.
 *      4) Se debera incluir en Pwm_Signal cada parametro que corresponda.
 *      5) Se debera definir su pin, incluyendo tris,pwm_high_pwm_low
 * - La funcion de interrupcion con baja prioridad solo se encuentra en PIC18F o
 * mejores, por lo tanto la interrupcion debera ser definida de la forma:
 *      void __interrupt() ISR(void); 
 * 
 * Inicializacion de la libreria:
 * - Para que el/los pwm funcionen de manera correcta se debera inicializar 
 * anteriormente algunos parametros como se muestra a continuacion:
 *      1) Tmr1_init();
 *      2) PwmSx_init(valor frecuencia); --> Donde x es el numero del pwm
 * 
 * - Se debera incluir en el archivo.c la frecuencia a la que trabaja el 
 * reloj u oscilador del microcontrolador.
 * 
 * Cargar valor de duty:
 * - Para cargar un procentaje del pulso en alto de la onda se debe hacer 
 * lo que se muestra a continuacion:
 *      PwmSx_set(porcentaje);  --> Donde x corresponde al numero del pwm
 */

#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

#ifndef PWM_SOFT_TMR1_H
#define	PWM_SOFT_TMR1_H


//Definiciones de las funciones a utilizar
#define Tmr1_init   Tmr1_configuracion
#define PwmS1_init  Pwm1_init
#define PwmS1_set   Pwm1
#define PwmS1_stop  Pwm1_stop

#define PwmS2_init  Pwm2_init
#define PwmS2_set   Pwm2
#define PwmS2_stop  Pwm2_stop

#define PwmS3_init  Pwm3_init
#define PwmS3_set   Pwm3
#define PwmS3_stop  Pwm3_stop

#define PwmS4_init  Pwm4_init
#define PwmS4_set   Pwm4
#define PwmS4_stop  Pwm4_stop

//Definiciones de salidas

#ifdef _PIC18F4550_H_

#define PWMS1_TRIS  TRISBbits.TRISB0
#define PWMS1_HIGH  LATBbits.LATB0=1
#define PWMS1_LOW   LATBbits.LATB0=0

#define PWMS2_TRIS  TRISBbits.TRISB1
#define PWMS2_HIGH  LATBbits.LATB1=1
#define PWMS2_LOW   LATBbits.LATB1=0

#define PWMS3_TRIS  TRISBbits.TRISB2
#define PWMS3_HIGH  LATBbits.LATB2=1
#define PWMS3_LOW   LATBbits.LATB2=0

#define PWMS4_TRIS  TRISBbits.TRISB3
#define PWMS4_HIGH  LATBbits.LATB3=1
#define PWMS4_LOW   LATBbits.LATB3=0

#else

#define PWMS1_TRIS  TRISBbits.TRISB0
#define PWMS1_HIGH  PORTBbits.RB0=1
#define PWMS1_LOW   PORTBbits.RB0=0

#define PWMS2_TRIS  TRISBbits.TRISB1
#define PWMS2_HIGH  PORTBbits.RB1=1
#define PWMS2_LOW   PORTBbits.RB1=0

#define PWMS3_TRIS  TRISBbits.TRISB2
#define PWMS3_HIGH  PORTBbits.RB2=1
#define PWMS3_LOW   PORTBbits.RB2=0

#define PWMS4_TRIS  TRISBbits.TRISB3
#define PWMS4_HIGH  PORTBbits.RB3=1
#define PWMS4_LOW   PORTBbits.RB3=0

#endif

//Funciones
void Tmr1_configuracion(void);
void Pwm1_init(unsigned int frecuencia);
void Pwm1(float duty);
void Pwm1_stop(void);

void Pwm2_init(unsigned int frecuencia);
void Pwm2(float duty);
void Pwm2_stop(void);

void Pwm3_init(unsigned int frecuencia);
void Pwm3(float duty);
void Pwm3_stop(void);

void Pwm4_init(unsigned int frecuencia);
void Pwm4(float duty);
void Pwm4_stop(void);

void Pwm_Signal(void);

//Variables globales

//Frecuencia globales:
unsigned int freqPwmS1=0,freqPwmS2=0,freqPwmS3=0,freqPwmS4=0;
//Contador de pwm:
unsigned int PwmS1=0,PwmS2=0,PwmS3=0,PwmS4=0;
//Parametros:
float Per_PwmS1=0,Per_PwmS2=0,Pw_PwmS1=0,Pw_PwmS2=0,Per_PwmS3=0,Pw_PwmS3=0,Per_PwmS4=0,Pw_PwmS4=0;
unsigned int P_W_T_S1=0,PER_T_S1=0,P_W_T_S2=0,PER_T_S2=0,P_W_T_S3=0,PER_T_S3=0,P_W_T_S4=0,PER_T_S4=0;
//Activaciones de pwm:
_Bool Act_PwmS1=0,Act_PwmS2=0,Act_PwmS3=0,Act_PwmS4=0;

#endif

