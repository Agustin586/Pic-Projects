#include <pic18f4550.h>
#include "CONFIG_PIC.h"
#include "RC522.h"
#include "OS_CONFIG.h"
#include "LCD20X4_I2C.h"
#include "Task2.h"


#ifndef	XC_Task1_H
    #include "Task1.h"
#endif

#ifndef XC_SOFT_I2C_H
    #include "SOFT_I2C.h"
#endif

#ifndef XC_SOFT_SPI_H
    #include "SPI_SOFT.h"
#endif


void main(void) {
    /*==========================================================================
                        CONFIGURACIONES INICIALES
    ==========================================================================*/
    unsigned char aux_portd=0,Est_pant=1,Est_proj=1;
    
    //Reinicia el registro
    INTCON = 0;
    
    //Inicializacion de puerto
    TRISA = 0xF0;
    TRISC = 0xFF;
    TRISD = 0xFF;
    TRISE = 0xFF;
    LATC = 0X00;
    LATD = 0x00;
    LATE = 0x00;
    
    
    //Configura Id del dispositivo
    TRISDbits.TRISD0 = 1,TRISDbits.TRISD1 = 1;
    TRISDbits.TRISD2 = 1,TRISDbits.TRISD3 = 1;
    TRISDbits.TRISD4 = 1;
    
    
    //Configuracion de pines como digitales
    ADCON1bits.PCFG = 0b1111;
    CMCON=7; // APAGA EL COMPARADOR.
    
    
    //Inicializaciones de librerias
    OS_init();
    UART_init(9600);
    i2c_init();
    Soft_SPI_Init();
    LCD_init();
    TEC4x4_init();
    MFRC522_Init();  // Inicaliza el controlador de la tarjeta
    
    
    LCD_command(CLEAR);
    LCD_array(1,1," ------------------ ");
    LCD_array(2,1,"|    PIC18F4550    |");
    LCD_array(3,1,"| MPLAB X IDE XC8  |");
    LCD_array(4,1," ------------------ ");
    __delay_ms(1000);
    LCD_command(CLEAR);
    LCD_array(1,1," ------------------ ");
    LCD_array(2,1,"|    MICROCHIP     |");
    LCD_array(3,1," ------------------ ");
    __delay_ms(1000);
    LCD_command(CLEAR);
    
    
    //Inicia interrupcion
    T0CONbits.TMR0ON  = 1;      // Habilita el timer 0
    INTCONbits.TMR0IE = 1;      // Habilita la interrupcion por tmr0
    INTCONbits.TMR0IF = 1;      // Desborda la bandera del tmr0
    
    
    //Configuracion de puertas
    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    TRISDbits.RD6 = 1;
    TRISDbits.RD7 = 1;
    
    ////////////////////////////////////////////////////////////////////////////
    
    /*==========================================================================
                        MULTITAREAS DE BAJA PRIORIDAD
    ==========================================================================*/
    /*
     * ------------------------------------------------
      |                CONTROL DE TAREAS               |
     * ------------------------------------------------
     *|TAREA | PRIORIDAD | TIEMPO (ms) |    FUNCION    |
     *|   0     BAJA        --          Reincia WTD    |
     *|   1     BAJA        50          Escribe Lcd    |
     *|   2     BAJA        50          Lee perifericos|
     *|   3     ALTA        --          Responde Serial|
     * ------------------------------------------------
     * 
     * Cada tarea se divide a su vez en subtareas o estados.
      
      
     * Cuando un temporizador termina cambia la tarea a correr 
     * --> Running = X --> indicando la tarea a realizar
     * --> Luego cuando se ejecuta la tarea pasa a un estado de ejecucion
     * --> Cuando ingresa a la tarea elije el correpondiente a realizar
    
    */
    
    ////////////////////////////////////////////////////////////////////////////
    
    while(1)
    {
        //Actualiza el valor de la llave o id del pic
        aux_portd = PORTD;
        key = aux_portd & 0x1F;
        
        //Elije la tarea a realizar
        switch(Running)
        {
            //Tare que sirve para limpiar el watchdog timer
            case 0:
            {
                //Reinicia el watchdog timer
                CLRWDT();
                
            break;
            }
            //Tareas de bajas prioridad
            case 1:
            {
                if(Task1_Est)
                {
                    Task1_Est = 2;          // Inicia la Tarea 1     
                    Task1(&Est_pant);       // Tarea de lcd
                    Task1_Est = 0;          // Tarea finalizada
                    Timer_Task1 = TICKS_T2; // Reinicia los ticks
                    
                    //Modo sleep
                    if(Task2_Est == 0)  Running=0;
                }
                
            break;
            }
            case 2:
            {
                if(Task2_Est)
                {
                    Task2_Est = 2;         // Inicia la Tarea 2
                    Task2(&Est_proj);      // Tarea de projecto
                    Task2_Est = 0;         // Indica que la tarea 2 finalizo
                    Timer_Task2 = TICKS_T2;// Reinicia los ticks
                    
                    //Modo sleep 
                    if(Task1_Est == 0)  Running=0;
                }
                
            break;
            }
            //Ingresa solo cuando termina de leer la comunicacion
            //Tarea de alta prioridad
            case 3: 
            {
                //Deshabilita las interrupciones para darle alta prioridad a la tarea
                GIEH = 0;       
                GIEL = 0;       
                
//                if(Bluetooth_Com() == 1)  
                Task_Com_USART();             
                if(!Task1_Est && !Task2_Est)    Running=0;
                
                //Vuelve a habilitar las interrupciones
                GIEH = 1;
                GIEL = 1;
                
            break;
            }
        }
    }
    
    return;
}
