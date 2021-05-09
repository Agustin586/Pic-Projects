#include "TECLADO_4X4.h"

void TEC4x4_init(void)
{
    ESC_PIN1 = 1;
    ESC_PIN2 = 1;
    ESC_PIN3 = 1;
    ESC_PIN4 = 1;
    KBI0 = 1, ESC_TRIS1 = 0;
    KBI1 = 1, ESC_TRIS2 = 0;
    KBI2 = 1, ESC_TRIS3 = 0;
    KBI3 = 1, ESC_TRIS4 = 0;
    INTCON2bits.RBPU = 0;           // Habilita las resistencias pull-up
    
    INTCONbits.GIEH  = 0;           // Desactiva las interrupciones de alta prioridad
    INTCONbits.RBIF  = 0;           // Interrupcion por puerto B
    INTCONbits.RBIE  = 1;           // Habilita interrupcion por puerto
    INTCON2bits.RBIP = 1;           // Alta prioridad 
    INTCONbits.PEIE  = 1;           // Perifericos activos de alta prioridad
    INTCONbits.GIEH  = 1;           // Interrupcion Global de alta prioridad
    
    return;
}

//Escaneo de pines
 void escaneo(void)
{
    ESC_PIN1 = 0;
    ESC_PIN2 = 1;
    ESC_PIN3 = 1;
    ESC_PIN4 = 1;
    for(char j=0;j<4;j++)
    {
        for(char i=0;i<=4;i++)
        {
            if(!ESC_PORT1 && i==0)
            {
                ESC_PIN1 = 1;
                ESC_PIN2 = 0;
            }
            if(!ESC_PORT2 && i==1)
            {
                ESC_PIN2 = 1;
                ESC_PIN3 = 0;
            }
            if(!ESC_PORT3 && i==2)
            {
                ESC_PIN3 = 1;
                ESC_PIN4 = 0;
            }
            if(!ESC_PORT4 && i==4)
            {
                ESC_PIN4 = 1;
            }
        }
    }
    
    return;
}

//Espera que el valor sea leido correctamente
void antirrebote(void)
{
    __delay_ms(50);
    while(PORTBbits.RB4 == 0 || PORTBbits.RB5 == 0 || PORTBbits.RB6 == 0 || PORTBbits.RB7 == 0) __delay_ms(10);
    
    return;
}  