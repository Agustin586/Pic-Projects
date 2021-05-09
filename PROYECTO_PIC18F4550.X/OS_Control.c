#include "OS_CONFIG.h"

////////////////////////////////////////////////////////////////////////////////

void OS_init(void)
{
    //Configuraciones del timer0 para la multitarea
    T0CONbits.TMR0ON = 1;               // Detiene el timer 0
    T0CONbits.T08BIT = 0;               // Modo tiemr 0 en 16 bits
    T0CONbits.T0CS   = 0;               // Modo temporizador
    T0CONbits.T0SE   = 0;               // Transicion de bajo a alto
    T0CONbits.PSA    = 0;               // Prescaler asignado
    T0CONbits.T0PS   = 0b111;           // Prescaler de 1:256
    TMR0 = 65066;                       // 10ms para 48MHz
//    TMR0 = 65339;
    
    //Habilita interrupciones por timer0
    RCONbits.IPEN      = 1;             // Con nivel de prioridad 
    INTCONbits.T0IE    = 0;             // Timer0 Interruption Enable
    INTCONbits.T0IF    = 1;             // Timer0 Interruption Flag
    INTCON2bits.TMR0IP = 0;             // Timer0 interrupt como baja prioridad
    INTCONbits.GIEL    = 1;             // Interrupcion global de baja prioridad
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void __interrupt(high_priority) ISR_HighP(void)
{
    //Interrupcion por recepcion
    if(PIR1bits.RCIF == 1)
    {
        word[long_cad] = UART_read();               // Guarda el caracter leido
        //UART_write(word[long_cad]);                 // Envia el mismo caracter leido
//        if(word[long_cad] != 13 && word[long_cad] != '.')    long_cad++;     // Suma la cantidad maxima de la cadena
//        else                        Fin_Cad = 1;    // Finaliza la cadena con un final de carro

//        if(word[long_cad] == 13)    Fin_Cad=1;
//        else    long_cad++;
//        long_cad = 0;
//        word[0] = 0;
        
        while(word[long_cad] != 13)
        {
            word[long_cad] = UART_read();
            long_cad++;
        }
        
        Fin_Cad = 1;
        
        //Habilita la tarea de comunicacion y la realiza como primera prioridad
        if(Fin_Cad == 1)    Running=3;              // Tarea de alta prioridad 
        
        Timer_RX = TICKS_RX;
        
        PIR1bits.RCIF = 0;
    }
    
    //Interrupcion por Puerto B 
    if(INTCONbits.RBIF == 1)
    {
        if(PORTBbits.RB4 == 0 || PORTBbits.RB5 == 0 || PORTBbits.RB6 == 0 || PORTBbits.RB7 == 0)
        {
            numero = 0;
            if((PORTAbits.RA3 == 0) && (PORTBbits.RB7 == 0))       numero = 1;
            if((PORTAbits.RA2 == 0) && (PORTBbits.RB7 == 0))       numero = 2;
            if((PORTAbits.RA1 == 0) && (PORTBbits.RB7 == 0))       numero = 3;
            if((PORTAbits.RA3 == 0) && (PORTBbits.RB6 == 0))       numero = 4;
            if((PORTAbits.RA2 == 0) && (PORTBbits.RB6 == 0))       numero = 5;
            if((PORTAbits.RA1 == 0) && (PORTBbits.RB6 == 0))       numero = 6;
            if((PORTAbits.RA3 == 0) && (PORTBbits.RB5 == 0))       numero = 7;
            if((PORTAbits.RA2 == 0) && (PORTBbits.RB5 == 0))       numero = 8;
            if((PORTAbits.RA1 == 0) && (PORTBbits.RB5 == 0))       numero = 9;
            if((PORTAbits.RA2 == 0) && (PORTBbits.RB4 == 0))       numero = 0;
            if((PORTAbits.RA0 == 0) && (PORTBbits.RB7 == 0))       numero = 10;
            if((PORTAbits.RA1 == 0) && (PORTBbits.RB4 == 0))       numero = 11;

            //Espera el tiempo necesario para guardar el valor correcto
            antirrebote();

            //Actualiza el valor 
            if(numero != 10 && numero != 11 && mod_rtc == 0)
            {
                if(tec_p)   valor=(valor*10)+numero;
                else        valor=numero,tec_p=1,mostrar=0,aceptar=0;
            }
            
            if(numero != 10 && numero != 11 && tec_p == 0 && mod_rtc == 1 && valor<=59)
            {
                if(val_rtc) valor=(valor*10)+numero;
                else        valor=numero,val_rtc=1,aceptar=0;
            }
            
            //Detecta la tecla aceptar
            if(numero == 10)   aceptar=1;
        }
        
        //Limpia la badera de interrupcion
        INTCONbits.RBIF = 0;
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void __interrupt(low_priority)  ISR_LowP(void)
{
    //Interrupcion por desborde de tmr0
    if(INTCONbits.TMR0IF == 1)
    {
        //Tareas de procesamiento continuo 
        if(Timer_Task1 != 0 && !Fin_Cad)    Timer_Task1--;                  // Tarea de Lcd
        if(Timer_Task2 != 0 && !Fin_Cad)    Timer_Task2--;                  // Tarea Projecto
        
        //Tareas de perifericos
        if(Timer_Tec != 0 && !Fin_Cad)                      Timer_Tec--;    // Tarea de escaneo de teclado
        if(Timer_Rtc != 0 && mod_rtc == 0 && !Fin_Cad)      Timer_Rtc--;
        
        //Tareas de delays
        if(Timer_DelayLcd != 0)     Timer_DelayLcd--;           // Delays bloqueantes
        if(Timer_DelayMuestra != 0) Timer_DelayMuestra--;       // Delay que permite actualizar los valores
        
        //Temporizador de puertas
        if(Timer_Puerta1 != 0 && puerta1)       Timer_Puerta1--;// Delay para cerrar la puerta
        if(Timer_Puerta2 != 0 && puerta2)       Timer_Puerta2--;// ||
        
        //Temporizador HASH
        if(Tarj_Dect == 1 && Timer_Hash != 0)   Timer_Hash --;  // Delay para borrar el hash leido
        
        //Limpia los archivos basura del rx si no recive nada
        if(Timer_RX != 0 && !Fin_Cad)       Timer_RX--;   
        
        TMR0 = 65066;           // 10ms a 48MHz  
//        TMR0 = 65339;
        
        T0CONbits.TMR0ON  = 1;  // Habilita el tmr0
        INTCONbits.TMR0IF = 0;  // Limpia la bandera de desborde
    }
    
    //Reinicia los temporizadores
    if(Timer_Task1==0 || Timer_Task2==0 || Timer_Tec==0 || Timer_Rtc==0 || Timer_Puerta1==0 || Timer_Puerta2==0 || Timer_Hash==0 || Timer_RX == 0) TaskDetected();
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void TaskDetected(void)
{
    if(Timer_Task1 == 0)
    {
        Task1_Est = 1;      // Tarea en espera
        Running = 1;        // Corre la tarea 1
    }
    if(Timer_Task2 == 0)
    {
        Task2_Est = 1;      // Tarea en espera
        Running = 2;        // Corre la tarea 2
    }
    if(Timer_Tec == 0)
    {
        escaneo();              // Realiza el escaneo del teclado
        Timer_Tec = TICKS_TEC;  // Reinicia la ticks
    }
    if(Timer_Rtc == 0 && mod_rtc == 0 && !Read_Eeprom)
    {
        RTC_read_time();        // Lee hora
        RTC_read_date();        // Lee fecha
        Timer_Rtc = TICKS_RTC;  // Reincia los ticks
    }
    if(Timer_Puerta2 == 0)
    {
        Pu_2 = 0;                   // Cierra puerta 2
        puerta2 = 0;                
        Timer_Puerta2 = TIMER_P2;   // Reinicia los ticks
    }
    if(Timer_Puerta1 == 0)
    {
        Pu_1 = 0;                   // Cierra puerta 1 y 2
        Pu_2 = 0;                   // --
        puerta1 = 0;
        Timer_Puerta1 = TIMER_P1;   // Reinicia los ticks
    }
    if(Timer_Hash == 0)
    {
        Timer_Hash = TIMER_HASH;    // Reinicia los ticks
        Tarj_Dect = 0;              // Reinicia las tarjetas detectadas
    }
    if(Timer_RX == 0)
    {
        Timer_RX = TICKS_RX;
        for(char i=0;i<20;i++)   word[i] = 0;               
        Fin_Cad = 0,long_cad = 0;
    }
    
    return;
}

//////////////////////////////////////////////////////////////////////////////// 
//long_cad = 0;
//        word[long_cad-1] = 0;
//        word[long_cad] = 0;
//        
//        while(word[long_cad-1] != 13 && word[long_cad-1] != '.')
//        {
//            if(PIR1bits.RCIF == 1)
//            {
//                word[long_cad] = UART_read();
//                long_cad++;
//            }
//        }
//        
//        Fin_Cad = 1;