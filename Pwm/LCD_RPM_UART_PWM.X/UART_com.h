#include <xc.h>
#define _XTAL_FREQ 4000000

/*------------------------------------------------------------------------------
                            FUNCTION PROTOTYPES
 -----------------------------------------------------------------------------*/
void UART_init(void);
void UART_write(char dato);
void UART_array(const char* cadena);
char UART_read(void);
