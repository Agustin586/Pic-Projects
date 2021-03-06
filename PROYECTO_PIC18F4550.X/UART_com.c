#include "UART_com.h"

/*==============================================================================
  FUNCTION:     UART_init()
  DESCRIPTION:  Inicializa las configuraciones para la comunicacion
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ninguno
==============================================================================*/
void UART_init(unsigned int BAUD)
{
    //Configuracion de puertos
    TRISCbits.TRISC7 = 1;   //Pone el Rx como entrada para leer
    TRISCbits.TRISC6 = 0;   //Configura como salida el Tx
    
    
    //Configuracion de baudios
    TXSTAbits.BRGH = 0;     // Baja velocidad
    SPBRG = (unsigned char) (((_XTAL_FREQ / BAUD) / 64) - 1);            
    
    //Modo de comunicacion
    TXSTAbits.SYNC  = 0;     // Modo asincronico
    
    //Habilita los pines seriales Tx y Rx
    RCSTAbits.SPEN = 1;
    
    
    //Configuracion de registro de transmision
    TXSTAbits.TX9   = 0;     // Configura solo palabra de 8 bits
    TXSTAbits.TXEN  = 1;     // Habilita la transmicion
    
    //Configuracion de registro de recepcion
    RCSTAbits.CREN = 1;     // Habilita recepcion
    RCSTAbits.RC9  = 0;     // Recepción 8 bits
    
    
    //Interrupcion por recepcion
    INTCONbits.GIE  = 1;    // Habilita interrupcion global
    PIE1bits.RCIE   = 1;    // Habilita la interrupcion por recepcion
    IPR1bits.RCIP   = 1;    // Pone prioridad alta en la interrupcion
    INTCONbits.PEIE = 1;    // Habilita la interrupcion por perifericos

    return;
}

/*==============================================================================
  FUNCTION:     UART_write()
  DESCRIPTION:  Envia la letra o digito
  PARAMETERS:   caracter
  RETURNS:      Nada
  REQUIREMENTS: Ninguno
==============================================================================*/
void UART_write(char dato)
{
    TXREG = dato;               //Enviar el dato
    while(TXSTAbits.TRMT == 0); //Lee si todavia no se termino de enviar todo el dato
    return;
}

/*==============================================================================
  FUNCTION:     UART_array()
  DESCRIPTION:  Envia una cadena de texto
  PARAMETERS:   Cadena de texto
  RETURNS:      Nada
  REQUIREMENTS: Ninguno
==============================================================================*/
void UART_array(const char* cadena)
{
    while(*cadena)
    {
        TXREG = *cadena;               //Enviar el dato
        while(TXSTAbits.TRMT == 0); //Lee si todavia no se termino de enviar todo el dato
        cadena++;
    }
    return;
}

/*==============================================================================
  FUNCTION:     UART_read()
  DESCRIPTION:  Lee un caracter desde otro dispositivo esclavo
  PARAMETERS:   Ninguno
  RETURNS:      Caracter
  REQUIREMENTS: Ninguno
==============================================================================*/
char UART_read(void)
{
    if(PIR1bits.RCIF == 1)  return RCREG;   //Si hay algo para leer lo devuelve 
    else return 0;
}