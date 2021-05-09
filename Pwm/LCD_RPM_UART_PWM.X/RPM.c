#include "RPM.h"


/*==============================================================================
  FUNCTION:     RPM_init()
  DESCRIPTION:  Inicializa los registros y configuraciones necesarios
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ingreso de pulso por RB0
==============================================================================*/
void RPM_init(void)
{
    //Configuracion de Puertos
    TRISBbits.TRISB0 = 1;
    
    //Configuracion de timer 1 como contador de oscilador interno
    T1CONbits.T1CKPS1 = 1;              //Preescaler en 1:8
    T1CONbits.T1CKPS0 = 1;              //Preescaler en 1:8
    T1CONbits.TMR1CS = 0;               //Reloj interno/4
    T1CONbits.TMR1ON = 1;               //Hablita timer1
    
    //Configuracion de interrupcion por RB0
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 0;                //Habilita interrupcion por RB0
    INTEDG = 1;                         //Por flanco ascendente
    INTF = 0;                           //Bandera en 0
    
    return;
}


/*==============================================================================
  FUNCTION:     RPM_read()
  DESCRIPTION:  Genera el valor de frecuencia y rpm 
  PARAMETERS:   Ninguno
  RETURNS:      Flotante --> variable rpm
  REQUIREMENTS: Ninguno
==============================================================================*/
float RPM_read(void)
{
    /*-----------------------------------------------------------------------
                            VARIABLE DECLARATIONS
    -----------------------------------------------------------------------*/
    float freq=0,rpm=0;

    flanco = 1;
    TMR1IF = 1;
    INTCONbits.INTE = 1;
    while(flanco <= 2);
    
    //Calcula la frecuencia y la transforma en rpm
    freq = 1 / (T * 0.000008);      //Calcula la frecuencia de los pulsos
    rpm = freq * 60.0;              //Calcula los rpm
    
    //Devuelve el valor de rpm
    return rpm;
}



