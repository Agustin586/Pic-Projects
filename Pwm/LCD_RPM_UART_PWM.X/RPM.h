#include <xc.h>
#include <stdint.h>
#include "UART_com.h"
#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 4000000
#endif

/*------------------------------------------------------------------------------
                            FUNCTION PROTOTYPES
 -----------------------------------------------------------------------------*/
void RPM_init(void);                            // Inicializa las configuraciones del rpm
float RPM_read(void);                           // Lee el valor del rpm con interrupcion por rb0

char flanco=1;    //Detecta cuando se produce el primer y segundo pulso por RB0
unsigned int T=0;