/* 
 * File:   Main.c
 * Author: Agustin Zuliani
 *
 * Created on 5 de noviembre de 2020, 19:06
 */

#include "Configuracion_Bits.h"
#include "Pwm_Soft_Tmr1.h"
#include <stdio.h>
#include <stdlib.h>

void __interrupt(low_priority) ISRL(void);

_Bool aux=0;

void main(void)
{
    unsigned int freq=200;
    float pwm=6.5;
    
    ADON = 0;

    TRISBbits.RB3=0;
        
    Tmr1_init();
    
    PwmS1_init(50);
//    PwmS1_set(50);
    
    PwmS2_init(50);
//    PwmS2_set(25);
//    
    PwmS3_init(50);
//    PwmS3_set(75);
//    
    PwmS4_init(50);
//    PwmS4_set(10);
    
//    __delay_ms(1000);
    
    while(1)
    {
//        PwmS1_init(freq+150);
        PwmS1_set(pwm);
        PwmS2_set(pwm);
        PwmS3_set(pwm);
        PwmS4_set(pwm);
//        PwmS2_init(freq+250);
//        PwmS2_set(pwm);
//        PwmS3_init(freq+1500);
//        PwmS3_set(pwm);
//        PwmS4_init(freq+2000);
//        PwmS4_set(pwm);
        __delay_ms(3000);
        PwmS1_stop();
        PwmS2_stop();
        PwmS3_stop();
        PwmS4_stop();
        __delay_ms(1000);
        
        pwm+=3.5;
//        freq+=100;
        if(pwm>10) pwm=6.5;
//        if(freq>5000)   freq=50;
        //PWMS1_HIGH;
    }

    return;
}

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
