#include "Configuracion_Bits.h"
#include "MEF.h"
#include <stdio.h>

#define INICIO      PORTBbits.RB0
#define ENTER       PORTBbits.RB1
#define DETENER     PORTBbits.RB2
#define MOVER       PORTBbits.RB3
#define BUZZER      PORTAbits.RA5
#define SALIDA_PIN  PORTBbits.RB4
#define SALIDA_TRIS TRISBbits.TRISB4
#define SALIDA_HIGH PORTBbits.RB4=1
#define SALIDA_LOW  PORTBbits.RB4=0

#define TICKS_T1                200   // Cada 20ms cambia de estado
#define TICKS_DELAY100ms        1000  // Cada 100ms escribe en la pantalla
#define TICKS_DELAY200ms        2000  // Cada 200ms limpia el watch dog y actualizar variables en lcd
#define TICKS_DELAY1s           10000 // Cada 1s resta al temporizador

#define Pin_Init    Pines_Init

void Pines_Init(void);
void Antirrebote(void);
void Task_Ready(void);
void __interrupt () ISR (void);

unsigned char Modo=0,Pwm=0,Min=0,Seg=0,Temp=0; 
_Bool Mostrar=0,Act_Variables=0,mod_tiempo=0,Output=0,Temporizador=0,Reset=0,Pwm_Seteado=0;
volatile unsigned int Delay100ms=TICKS_DELAY100ms,Delay200ms=TICKS_DELAY200ms,Delay1s=TICKS_DELAY1s;
unsigned int Rpm=0;

////////////////////////////////////////////////////////////////////////////////
void main(void)
{   
    //Inicializaciones
    Pin_Init();
    Mef_Init();
    Lcd_Init();
    Timer1_Init();
    Adc_Init();
    PwmS1_stop();
    
    TMR1IE=1,TMR1IF=1;      // Activa la interrupcion de tmr1
    
    WDTCONbits.SWDTEN = 1;          //Habilita el watch dog
    WDTCONbits.WDTPS  = 0b1010;     // Preescaler de wdt 1:32768 --> timer = 32768 / 32Khz ~= 1 segundo
    
    while(1)
    {
        Mef_Updated(); 
    }

    return;
}
////////////////////////////////////////////////////////////////////////////////
void Pines_Init(void)
{
    //Configuraciones de Entradas y Salidas
    TRISAbits.TRISA5 = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    TRISB = 0xFF;                       // Port B how to input
    TRISC = 0x00;                       // Port C how to output
    ANSELHbits.ANS12 = 0;               // Setting RB0 how to a digital input
    ANSELHbits.ANS11 = 0;               // Setea como digital
    ANSELHbits.ANS10 = 0;               // Setting RB1 how to a digital input
    ANSELHbits.ANS8 = 0;                // Setting RB2 how to a diigtla input
    ANSELHbits.ANS9 = 0;                // Setting RB3 how to a diigtla input
    ANSELbits.ANS4  = 0;                // Setting how to digital input
    
    SALIDA_TRIS = 0;
    
    return;
}
////////////////////////////////////////////////////////////////////////////////
void Antirrebote(void)
{
    BUZZER = 1;
    __delay_ms(50);
    BUZZER = 0;
    while(INICIO || ENTER || DETENER || MOVER)  __delay_ms(10);
    
    return;
}
////////////////////////////////////////////////////////////////////////////////
void __interrupt () ISR (void)
{
    _Bool f_pwmS1=0;
    
    //Interrupcion por timer 0 
    if(TMR1IF == 1)
    {
        if(Delay100ms!=0 && !Mostrar)       Delay100ms--;    // Temporizador de muestra del display 
        if(Delay200ms!=0)                   Delay200ms--;    // Limpia el wdt y actualiza variables del lcd
        if(Delay1s!=0 && Output)            Delay1s--;       // Temporizador de contador de segundos
        if(Act_PwmS1 && PwmS1!=PER_T_S1)            PwmS1++;
        if(PwmS1 == P_W_T_S1 || PwmS1 == PER_T_S1)  f_pwmS1=1;
        
        TMR1 = 65285;     // 100us
        TMR1ON = 1;
        TMR1IF = 0;       // Limpia la bandera de desborde
    }
    
    if(!Delay100ms || !Delay200ms || !Delay1s)          Task_Ready();
    if(f_pwmS1 && Act_PwmS1)                            Pwm_Signal();
    
    return;
}
////////////////////////////////////////////////////////////////////////////////
void Task_Ready(void)
{
    if(!Delay100ms)
    {
        Mostrar = 1;
        Delay100ms = TICKS_DELAY100ms;
    }
    if(!Delay200ms)
    {
        CLRWDT();
        Act_Variables = 1;
        Delay200ms = TICKS_DELAY200ms;
    }
    if(!Delay1s)
    {
        if(Seg == 0)
        {
            if(Min != 0)    Min--,Seg=59;
        }
        else Seg--;
        Temporizador = 1;
        Delay1s = TICKS_DELAY1s;
    }
    
    return;
}
////////////////////////////////////////////////////////////////////////////////
