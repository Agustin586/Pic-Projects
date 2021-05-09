#include "Configuracion_Bits.h"
#include "Pwm_Soft_Tmr1.h"     //Inlcuye la configuracion de bits y la frecuencia del oscilador

void Tmr1_configuracion(void)
{
    //Deshabilita interrupciones
    INTCONbits.GIE = 0;
    
    //Configura el registro del timer 1
    T1CONbits.RD16    = 1;      //Configura para cargar un solo valor de 16 bits
    T1CONbits.T1RUN   = 0;      //Reloj principal no deriva del oscilador tmr1
    T1CONbits.T1CKPS  = 0b01;   //Preescaler con 1:2 --> 39Hz <= freq <= 2.5MHz --> 400ns
    T1CONbits.T1OSCEN = 1;
    T1CONbits.TMR1CS  = 0;      //Oscilador desde el pic
    T1CONbits.TMR1ON  = 0;      //Deshabilita el timer1
    //Para 20Mhz
//    TMR1 = 65510;               //Configura a 10us para el desborde
//    TMR1 = 65410;     //50us
//    TMR1 = 65285;     //100us
//    TMR1 = 63035;     //1ms
    //Para 48Mhz
    TMR1 = 65232;   //50us

    //Configura las interrupciones del timer 1
#ifdef _PIC18F4550_H_
    INTCONbits.GIE  = 1;
    INTCONbits.GIEL = 1;    //Interrupcion de baja prioridad
    INTCONbits.PEIE = 1;    //Habilita interrupcion por periferico
    IPR1bits.TMR1IP = 0;    //Prioridad de interrupcion baja prioridad
    PIE1bits.TMR1IE = 1;    //Habilita interrupcion por timer1
    PIR1bits.TMR1IF = 0;    //Deshalitida la bandera de interrupcion
#else
    INTCONbits.GIE = 1;     //Interrupcion de baja prioridad
    INTCONbits.PEIE = 1;    //Habilita interrupcion por periferico
    PIE1bits.TMR1IE = 1;    //Habilita interrupcion por timer1
    PIR1bits.TMR1IF = 0;
#endif

    return;
}

////////////////////////////////////////////////////////////////////////////////

void Pwm1_init(unsigned int frecuencia)
{
    //Pone como salida
    PWMS1_TRIS = 0;
    PWMS1_LOW;

    //Obtiene la frecuencia y el periodo de la onda
    freqPwmS1 = frecuencia;
    Per_PwmS1 = 1.0/frecuencia;
//    PER_T_S1  = Per_PwmS1 / 0.00001;  //10us
    PER_T_S1  = Per_PwmS1 / 0.00005;    //50us
//    PER_T_S1  = Per_PwmS1 / 0.0001;   //100us
//    PER_T_S1  = Per_PwmS1 / 0.001;    //1ms
    
    //Carga el valor de parametros
    Pw_PwmS1 = 0;
    PwmS1 = 0;

    return;
}

void Pwm1(float duty)
{
    //Activa el Pwm
    Act_PwmS1 = 1;

    Pw_PwmS1 = (duty * Per_PwmS1) / 100.0;
//    P_W_T_S1 = Pw_PwmS1 / 0.00001;    //10us
    P_W_T_S1 = Pw_PwmS1 / 0.00005;      //50us
//    P_W_T_S1 = Pw_PwmS1 / 0.0001;     //100us
//    P_W_T_S1 = Pw_PwmS1 / 0.001;      //1ms
        
    //Habilita el timer1
    T1CONbits.TMR1ON  = 1;
    
    return;
}

void Pwm1_stop(void)
{
    //Desactiva el pwm
    Act_PwmS1 = 0;
    
    Pw_PwmS1 = 0;
    PwmS1 = 0;
    
    PWMS1_LOW;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Pwm2_init(unsigned int frecuencia)
{
    //Pone como salida
    PWMS2_TRIS = 0;
    PWMS2_LOW;

    //Obtiene la frecuencia y el periodo de la onda
    freqPwmS2 = frecuencia;
    Per_PwmS2 = 1.0/frecuencia;
//    PER_T_S2  = Per_Pwm2 / 0.00001;  //10us
    PER_T_S2  = Per_PwmS2 / 0.00005;    //50us
//    PER_T_S2  = Per_PwmS2 / 0.0001;   //100us
//    PER_T_S2  = Per_PwmS2 / 0.001;    //1ms
    
    //Carga el valor de parametros
    Pw_PwmS2 = 0;
    PwmS2 = 0;

    return;
}

void Pwm2(float duty)
{
    //Activa el Pwm
    Act_PwmS2 = 1;

    Pw_PwmS2 = (duty * Per_PwmS2) / 100.0;
//    P_W_T_S2 = Pw_PwmS2 / 0.00001;    //10us
    P_W_T_S2 = Pw_PwmS2 / 0.00005;      //50us
//    P_W_T_S2 = Pw_PwmS2 / 0.0001;     //100us
//    P_W_T_S2 = Pw_PwmS2 / 0.001;      //1ms
        
    //Habilita el timer1
    T1CONbits.TMR1ON  = 1;
    
    return;
}

void Pwm2_stop(void)
{
    //Desactiva el pwm
    Act_PwmS2 = 0;
    
    Pw_PwmS2 = 0;
    PwmS2 = 0;
    
    PWMS2_LOW;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Pwm3_init(unsigned int frecuencia)
{
    //Pone como salida
    PWMS3_TRIS = 0;
    PWMS3_LOW;

    //Obtiene la frecuencia y el periodo de la onda
    freqPwmS3 = frecuencia;
    Per_PwmS3 = 1.0/frecuencia;
    PER_T_S3  = Per_PwmS3 / 0.00005;    //50us
    
    //Carga el valor de parametros
    Pw_PwmS3 = 0;
    PwmS3 = 0;

    return;
}

void Pwm3(float duty)
{
    //Activa el Pwm
    Act_PwmS3 = 1;

    Pw_PwmS3 = (duty * Per_PwmS3) / 100.0;
    P_W_T_S3 = Pw_PwmS3 / 0.00005;      //50us
        
    //Habilita el timer1
    T1CONbits.TMR1ON  = 1;
    
    return;
}

void Pwm3_stop(void)
{
    //Desactiva el pwm
    Act_PwmS3 = 0;
    
    Pw_PwmS3 = 0;
    PwmS3 = 0;
    
    PWMS3_LOW;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Pwm4_init(unsigned int frecuencia)
{
    //Pone como salida
    PWMS4_TRIS = 0;
    PWMS4_LOW;

    //Obtiene la frecuencia y el periodo de la onda
    freqPwmS4 = frecuencia;
    Per_PwmS4 = 1.0/frecuencia;
    PER_T_S4  = Per_PwmS4 / 0.00005;    //50us
    
    //Carga el valor de parametros
    Pw_PwmS4 = 0;
    PwmS4 = 0;

    return;
}

void Pwm4(float duty)
{
    //Activa el Pwm
    Act_PwmS4 = 1;

    Pw_PwmS4 = (duty * Per_PwmS4) / 100.0;
    P_W_T_S4 = Pw_PwmS4 / 0.00005;      //50us
        
    //Habilita el timer1
    T1CONbits.TMR1ON  = 1;
    
    return;
}

void Pwm4_stop(void)
{
    //Desactiva el pwm
    Act_PwmS4 = 0;
    
    Pw_PwmS4 = 0;
    PwmS4 = 0;
    
    PWMS4_LOW;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Pwm_Signal(void)
{
    if(PwmS1==P_W_T_S1)
    {
        PWMS1_LOW;                              //Baja el pin
    }
    if(PwmS1==PER_T_S1)
    {
        PwmS1 = 0;
        PWMS1_HIGH;                             //Sube el pin
    }
    if(PwmS2==P_W_T_S2)
    {
        PWMS2_LOW;                              //Baja el pin
    }
    if(PwmS2==PER_T_S2)
    {
        PwmS2 = 0;
        PWMS2_HIGH;                             //Sube el pin
    }
    if(PwmS3==P_W_T_S3)
    {
        PWMS3_LOW;                              //Baja el pin
    }
    if(PwmS3==PER_T_S3)
    {
        PwmS3 = 0;
        PWMS3_HIGH;                             //Sube el pin
    }
    if(PwmS4==P_W_T_S4)
    {
        PWMS4_LOW;                              //Baja el pin
    }
    if(PwmS4==PER_T_S4)
    {
        PwmS4 = 0;
        PWMS4_HIGH;                             //Sube el pin
    }
    
    return;
}

//Se tiene que cargar estas lineas en el main.c
/*
void __interrupt(low_priority) ISRL(void)
{
    _Bool f_pwmS1=0,f_pwmS2=0,f_pwmS3=0,f_pwmS4=0;
    
    if(PIR1bits.TMR1IF)
    {
        if(Act_PwmS1 && PwmS1!=PER_T_S1)   PwmS1++;
        if(Act_PwmS2 && PwmS2!=PER_T_S2)   PwmS2++;
        if(Act_PwmS3 && PwmS3!=PER_T_S3)   PwmS3++;
        if(Act_PwmS4 && PwmS4!=PER_T_S4)   PwmS4++;
        
        if(PwmS1 == P_W_T_S1 || PwmS1 == PER_T_S1)       f_pwmS1=1;
        if(PwmS2 == P_W_T_S2 || PwmS2 == PER_T_S2)       f_pwmS2=1;
        if(PwmS3 == P_W_T_S3 || PwmS3 == PER_T_S3)       f_pwmS3=1;
        if(PwmS4 == P_W_T_S4 || PwmS4 == PER_T_S4)       f_pwmS4=1;
        
//        TMR1 = 65510;           //10us para 20MHz
//        TMR1 = 65410;           //50us para 20MHz
//        TMR1 = 65285;           //100us
//        TMR1 = 63035;           //1ms
        //Para 48Mhz
        TMR1 = 65232;   //50us
        
        TMR1ON = 1;
        PIR1bits.TMR1IF = 0;
    }
    
    if((f_pwmS1 && Act_PwmS1) || (f_pwmS2 && Act_PwmS2) || (f_pwmS3 && Act_PwmS3) || (f_pwmS4 && Act_PwmS4))    Pwm_Signal();
    
    return;
}
}*/