// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include "RPM.h"
#include "LCD_control.h"
#include "UART_com.h"
#include "Libreria pwm.h"

#define _XTAL_FREQ 4000000

#define ON_OFF     PORTBbits.RB1
#define GIRO_P     PORTBbits.RB2
#define PWM_P      PORTBbits.RB3
#define LED1       PORTBbits.RB5
#define LED2       PORTBbits.RB6


void antirrebote(void);
void __interrupt(high_priority) ventana(void);  // Funcion de interrupcion


_Bool giro=0,on=0;
char pwm=0;


/*==============================================================================
  FUNCTION:     main()
  DESCRIPTION:  Devuelve el valor de las rev por minuto(entero sin signo)
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ninguno
==============================================================================*/
void main(void)
{
    //Configuracion de puertos
    TRISD = 0x00;
    PORTD = 0x00;
    TRISB = 255;
    
    //Inicializaciones
    LCD_init();
    RPM_init();
    UART_init();
    PWM_init_1(5000);
    PWM_init_2(5000);
    
//    LCD_command(CLEAR);
//    LCD_array(1,1,"PIC16F877A");
//    LCD_array(2,1,"MPLAB");
//    __delay_ms(500);
//    
//    LCD_command(CLEAR);
//    LCD_array(1,1,"ZULIANI,LUJAN");
//    __delay_ms(500);
    
    LCD_command(CLEAR);
    LCD_array(1,1,"MOTOR:");
    LCD_array(2,1,"RPM:");
    LCD_array(2,10,"PWM:");
    
    //Configuracion de los leds
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    LED1 = 1;
    LED2 = 0;
    
    //Motor apagado
    on = 0;
    giro = 0;
    
    /*-----------------------------------------------------------------------
                            LOOP FOREVER
	-----------------------------------------------------------------------*/
    char buffer[5];
    unsigned int rpm=0;
    
    
    //Bucle infinito del programa principal
    while(1)
    {
        if(ON_OFF == 1)     on=!on,antirrebote();
        if(PWM_P == 1)      antirrebote(),pwm+=25;
        
        //Prende el motor
        if(on == 1)
        {
            LCD_xy(1,9);
            LCD_date(0);
            LCD_array(1,7,"ON");
            
            //Giro para la izquierda
            if(giro == 1)
            {
                PWM_Stop_2();           // Detiene el pwm 2
                PWM_1(pwm);             // Habilita el pwm 1
                rpm = (int)RPM_read();  // Lee los rpm
            }
            else
            {
                PWM_Stop_1();           // Detiene el pwm 1
                PWM_2(pwm);             // Habilita el pwm 2
                rpm = (int)RPM_read();  // Lee los rpm
                if(rpm > 9999)  rpm = 9999;
            }
        }
        //Apaga el motor
        else if(on == 0)
        {
            PWM_Stop_1();
            PWM_Stop_2();
            LCD_array(1,7,"OFF");
            rpm = 0;
            pwm = 0;
            
            //Cambio de giro
            if(GIRO_P == 1)     antirrebote(),giro=!giro;
            
            //Indicadores de tipo de giro
            if(giro == 1)       LED2=1,LED1=0,LCD_array(1,14,"IZQ");
            else if(giro == 0)  LED1=1,LED2=0,LCD_array(1,14,"DER");
        }
        
        //Limites del pwm
        if(pwm > 100)       pwm = 0;
        //Muestra el valor de los parametros
        sprintf(buffer,"%03d",pwm);
        LCD_array(2,14,buffer);
        
        if(rpm < 9999 && rpm != 0)
        {
            //Escritura del rpm
            sprintf(buffer,"%04u",rpm);
            LCD_array(2,5,buffer);
            
            __delay_ms(500);
            
            UART_array("RPM:");
            UART_array(buffer),UART_write(13);
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void antirrebote(void)
{
    __delay_ms(30);
    while(ON_OFF == 1 || GIRO_P == 1 || PWM_P == 1);
    
    return;
}

/*==============================================================================
  FUNCTION:     ventana()
  DESCRIPTION:  Genera una ventana que depende de la frecuencia que ingresa
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ingreso de pulsos por RB0
==============================================================================*/
void __interrupt() ventana(void)
{
    if(INTCONbits.INTF == 1)
    {
        //Detecta el pulso y comienza a contar
        if(flanco == 1)
        {
            if(PIR1bits.TMR1IF == 1)
            {
                TMR1 = 0;                       //Inicia el timer1 en 0
                PIR1bits.TMR1IF = 0;            //Inicia el timer1
            }
        }

        //Cuando se genere el segundo flanco guarda el valor
        if(flanco == 2)
        {
            //Guarda el valor del segundo flanco ascendente
            T1CONbits.TMR1ON = 0;
            T = TMR1;                       //Guarda el valor obtenido
            T1CONbits.TMR1ON = 1;
            INTCONbits.INTE = 0;
        }

        INTCONbits.INTF = 0;
        flanco++;
    }
    
    if(PIR1bits.RCIF == 1)
    { 
        char tecla=0;
        //Lee el valor
        tecla = UART_read();
        UART_write(tecla);
        
        //Prende el motor
        if(tecla == 'A' || tecla == 'a')        if(on == 0) on = ! on;
        //Apaga el motor
        if(tecla == 'D' || tecla == 'd')        if(on == 1) on = ! on;
        //Sube velocidad
        if(tecla == 'W' || tecla == 'w')
        {
            pwm+=25;
            if(pwm > 100)   pwm = 0;
        }
        //Baja velocidad
        if(tecla == 'S' || tecla == 's')
        {
            if(pwm == 0)    pwm = 100;
            else            pwm-=25;  
        }
        if((tecla == 'G' || tecla == 'g') && on == 0)   giro=!giro;
        
        PIR1bits.RCIF = 0;
    }
    
    return;
}







/*******************************************************************************
|  CLASS:         Desarrollo
|  PROGRAM:       LCD_RPM
|  AUTHOR:        Zuliani,Agustin; Lautaro,Lujan
|  DATE:          27 APRIL 2020
|  DESCRIPTION:   Lee las revoluciones por minutos de una determ. frecuencia,
|                 mediante las interrupciones por RB0 con flanco ascendente.
|  REQUIREMENTS:  Ingreso de pulsos por entra RB0
|  ADDITIONAL:    Valor flotante "%(cant enteros).(cant decimales)f","sin (int)" 
|
|  REVISION HISTORY:
|  27 APRIL 2020    V1.0 Creado
|
|  10 MAY 2020      V1.16 Muestra las rev por min de forma flotante  
|  2:50  
|
|  10 MAY 2020      V1.7 Convierte la variable flotante en entera, por lo tanto
|  3:37             devuelve solo valores enteros
|
*******************************************************************************/


//        if(pwm < 100)
//        {
//            LCD_xy(2,16);
//            LCD_date(0);
//        }
//        if(pwm < 10)
//        {
//            LCD_xy(2,15);
//            LCD_date(0);
//        }