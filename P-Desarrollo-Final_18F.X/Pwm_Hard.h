#ifndef _PWM_HARD_H
#define _PWM_HARD_H

#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

//Definiciones de funciones
#define Pwm1_init   PWM_init_1
#define Pwm2_init   PWM_init_2
#define Pwm1_set    PWM_1
#define Pwm2_set    PWM_2
#define Pwm1_stop   PWM_Stop_1
#define Pwm2_stop   PWM_Stop_2

unsigned int freq=0,pr2=0;
/*------------------------------------------------------------------------------
                            FUNCTION PROTOTYPES
 -----------------------------------------------------------------------------*/
void PWM_init_1(unsigned int frecuencia);
void PWM_1(unsigned char duty);
void PWM_Stop_1(void);
void PWM_init_2(unsigned int frecuencia);
void PWM_2(char duty);
void PWM_Stop_2(void);

#endif
