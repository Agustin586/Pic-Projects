#include "UART_com.h" 

/*==============================================================================
  FUNCTION:     UART_init()
  DESCRIPTION:  Inicializa las configuraciones para la comunicacion
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ninguno
==============================================================================*/
void UART_init(void)
{
    TRISCbits.TRISC7 = 1;   //Pone el Rx como entrada para leer
    TRISCbits.TRISC6 = 0;   //Configura como salida el Tx
    TXSTAbits.TX9 = 0;      //Configura solo palabra de 8 bits
    TXSTAbits.TXEN = 1;     //Habilita la transmicion
    TXSTAbits.SYNC = 0;     //Modo asincronico
    TXSTAbits.BRGH = 1;     //Alta velocidad
    SPBRG = 25;             //Configura para 9600 baudios con 4Mhz
    RCSTAbits.SPEN = 1;     //Habilita el puerto serial
    RCSTAbits.CREN = 1;     //Habilita recepcion
    RCSTAbits.RX9 = 0;      //Habilita a 8 bits
    
    //Interrupcion por recepcion
    INTCONbits.GIE = 1;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    
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
        UART_write(*cadena);
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
