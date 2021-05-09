/*
 * File:   Main.c
 * Author: Zuliani,Lujan
 *
 * Created on 8 de octubre de 2020, 00:00
 */

#include "Configuracion_Bits.h"
#include "Uart_Hard.h"      // Incluimos libreria Uart por hardware
#include "Pwm_Hard.h"       // Incluimos libreria de PWM por hardware
#include "Ds18b20.h"        // Libreria para el sensor de temperatura
#include "Fonts.h"          // Carga el tipo de fuente de la pantalla
#include "Ili9341.h"        // Carga la libreria del controlador
#include <string.h>
#include <stdio.h>

//Definicion lectura de temperatura:
#define TICKS_TMP   100     // 1 segundo de actulizacion de temperatura
#define TICKS_MPT   250     // 2.5 para mostrar la temperatura en pantalla

//Variable globales:

//Comunicacion UART:
_Bool fin_carro=0;
unsigned char long_cad=0;
char cmd[25];
const char setpwm[]={"setpwm:"},setest[]={"setest:"},setreset[]={"setreset"},gettmp[]={"gettmp"},help[]={"help"};

//Seteos de valores:
unsigned int pwm1=0,rpm=0;

//Lectura de temperatura:
float temp=0;
_Bool vis=0;

//Cambio surgidos:
_Bool giro_ant=0,giro=0,est_m=0,set_ver=0,Most_Temp=0,ver=0,est_c=0,vis_p_temp=0,pwm_c=0;

//Timers por software:
unsigned char Timer_Lect_Temp=TICKS_TMP;
unsigned int  Timer_Most_Pant_Temp=TICKS_MPT;

//Funciones:
void Task_Com(void);    // Tarea en la que trabaja con la comunicacion serial
void Set_Val(void);     // Tarea que visualiza las variables en el lcd
void Set_Pwm(void);     // Tarea que setea el valor del pwm
void Timers(void);      // Realiza una accion luego de terminado un timer
void Send_Temp(void);   // Envia la temperatura
void __interrupt(high_priority) ISRH(void);
void __interrupt(low_priority)  ISRL(void);

//------------------------------------------------------------------------------

//Funcion principal:
void main(void)
{
    char buffer[10];
    
    ADON = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    //Inicializaciones:
    TFT_init();             // Inicializa la pantalla
    Pwm1_init(5000);
    Serial_Hard_init(9600);
    
    //Habilita interupciones
    INTCONbits.GIEH = 1;    // Habilita interrupcion de alta prioridad
    INTCONbits.GIEL = 1;    // Habilita interrupcion de baja prioridad
    RCONbits.IPEN   = 1;    // Con nivel de prioridad
    
    //Configura el timer0
    T0CONbits.TMR0ON = 0;   // Desactiva el tmr0
    T0CONbits.T08BIT = 1;   // 8 bits tmr0
    T0CONbits.T0CS   = 0;   // Interno clock
    T0SE = 0;
    PSA  = 0;
    T0CONbits.T0PS = 0b111; // Preescaler 1:256
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 0;
    INTCON2bits.TMR0IP = 0; // Baja prioridad
    TMR0 = 59;              // 10ms para 20Mhz

    //--------------------------------------------------------------------------
    //Presentacion:
    
    Serial_println("");
    Serial_println("");
    Serial_println("Inicializando controlador...");
    Serial_println("Alumnos: Zuliani,Agustin; Lujan,Lautaro");
    Serial_println("Proyecto: Trabajo Final Desarrollo");
    Serial_interrupt(RECEP_ON,TRANS_OFF);
    
    TFT_fillscreen(BLACK);
    
    int x1=117,y1=155,w=5,h=10;
    unsigned int color;
    for(char i=1;i<100;i++)
    {
        if(i<33)    color=YELLOW;
        if(i>=33)   color=GREEN;
        TFT_rect(x1,y1,w,h,color);
        x1 = x1 - 5;
        y1 = y1 - 5;
        w  = w  + 10;
        h  = h  + 10;
    }
    for(char i=1;i<100;i++)
    {
        if(i<33)    color=GREEN;
        if(i>33 && i<66)    color=PINK;
        if(i>66)    color=MAGENTA;
        TFT_rect(x1,y1,w,h,color);
        x1 = x1 + 5;
        y1 = y1 + 5;
        w  = w  - 10;
        h  = h  - 10;
    }
    x1=117,y1=155,w=5,h=10;
    for(char i=1;i<100;i++)
    {
        if(i<33)    color=ORANGE;
        if(i>33 && i<66)    color=GREEN;
        if(i>66)    color=YELLOW;
        TFT_rect(x1,y1,w,h,color);
        x1 = x1 - 5;
        y1 = y1 - 5;
        w  = w  + 10;
        h  = h  + 10;
    }
    for(char i=1;i<100;i++)
    {
        if(i<33)    color=CYAN;
        if(i>33 && i<66)    color=PURPLE;
        if(i>66)    color=BLUE;
        TFT_rect(x1,y1,w,h,color);
        x1 = x1 + 5;
        y1 = y1 + 5;
        w  = w  - 10;
        h  = h  - 10;
    }
    x1=117,y1=155,w=5,h=10;
    for(char i=1;i<100;i++)
    {
        if(i<33)    color=OLIVE;
        if(i>33 && i<66)    color=WHITE;
        if(i>66)    color=NAVY;
        TFT_rect(x1,y1,w,h,color);
        x1 = x1 - 5;
        y1 = y1 - 5;
        w  = w  + 10;
        h  = h  + 10;
    }
    __delay_ms(3000);
    
    TFT_fillscreen(BLUE);
    TFT_text(70,220,"Sistema de Desarrollo",BLACK,WHITE);
    TFT_text(10,190,"Alumnos: Zuliani,Lujan",BLACK,WHITE);
    TFT_text(10,165,"Controlador: ILI9341 MPLAB XC8",BLACK,WHITE);
    TFT_text(10,140,"Esta programa mide la temperatura",BLACK,WHITE);
    TFT_text(10,115,"y controla un pwm",BLACK,WHITE);
    TFT_text(70,40,"Trabajo Final Desarrollo",GREENYELLOW,BLACK);
    
    __delay_ms(4000);

    TFT_fillscreen(BLACK);
    //--------------------------------------------------------------------------
    
    //Datos:
    TFT_text(57,220,"Visualizacion de Controles",YELLOW,ORANGE);    
    TFT_text(10,157,"Porcentaje PWM:   0",WHITE,BLACK);
    TFT_text(10,84,"Estado del motor:   OFF",WHITE,BLACK);
    TFT_text(10,10,"Temperatura:    0.00     C",WHITE,BLACK);
    TFT_fillrect(86,200,10,10,RED);
    TFT_fillrect(157,200,10,10,GREEN);
    
    //Activa el tmr0
    TMR0ON = 1;
    TMR0IF = 1;
    TMR0IE = 1;
    
    Serial_println("Iniciado");
    Serial_println("");
    Serial_print("cmd < ");
    
    while(1)
    {
        //Si es necesario ingresa a la tarea de comunicacion
        if(fin_carro)   Serial_println(" "),Task_Com();
        //Setea valores en el display:
        Set_Val();
        //Setea el valor del pwm
        Set_Pwm();
        //Envia temperatura
        Send_Temp();
    }
    
    return;
}

void Set_Pwm(void)
{
    if(ver)
    {
        if(est_m)   Pwm1_set(pwm1);
        else        pwm1=0,Pwm1_set(1),Pwm1_stop();
        
        ver = 0;
    }
    
    return;
}

void Set_Val(void)
{
    char buffer[32];
    
    //Visualiza valor de pwm
    if(pwm_c)
    {
        TFT_fill_rect(155,5,30,180,BLACK);
        sprintf(buffer,"Porcentaje PWM:   %3d",pwm1);
        TFT_text(10,157,buffer,WHITE,BLACK);
        
        if(pwm1 < 33)
        {
            TFT_fillrect(157,230,10,10,BLACK);
            TFT_fillrect(157,215,10,10,BLACK);
            TFT_fillrect(157,200,10,10,GREEN);
        }
        if(pwm1 >= 33 && pwm1 < 66)
        {
            TFT_fillrect(157,230,10,10,BLACK);
            TFT_fillrect(157,215,10,10,ORANGE);
        }
        if(pwm1 >= 66)
        {
            TFT_fillrect(157,215,10,10,ORANGE);
            TFT_fillrect(157,230,10,10,RED);
        }
        
        pwm_c = 0;
    }
    if(est_c)
    {
        if(est_m)
        {
            TFT_fill_rect(80,5,30,250,BLACK);
            TFT_text(10,84,"Estado del motor:   ON",WHITE,BLACK);
            TFT_fillrect(86,200,10,10,GREEN);
        }
        else
        {
            TFT_fill_rect(80,5,30,250,BLACK);
            TFT_text(10,84,"Estado del motor:   OFF",WHITE,BLACK);
            TFT_fillrect(86,200,10,10,RED);
            TFT_fill_rect(155,5,30,300,BLACK);
            TFT_text(10,157,"Porcentaje PWM:   0",WHITE,BLACK);
            TFT_fillrect(157,200,10,10,GREEN);
        }
        
        est_c = 0;
    }
    if((Most_Temp && vis)|| vis_p_temp)
    {
        sprintf(buffer,"Temperatura:     %03.02f     C",temp);
        TFT_text(10,10,buffer,WHITE,BLACK);
        
        if(vis_p_temp)  vis_p_temp=0;
    }
    
    return;
}

void Send_Temp(void)
{
    char buffer[10];
    
    if(Most_Temp && vis)   
    {
        sprintf(buffer,"%3.2f C",temp);
        Serial_print("      "),Serial_println(buffer);
        Serial_println("");
        Serial_print("cmd < ");
        __delay_ms(10);
        vis = 0;
        Most_Temp = 0;
    }
    
    return;
}

void Task_Com(void)
{
    _Bool flag=0; //Sea activa cuando se detecta :
    uint8_t comando=0,j=0;
    char buffer_set[15],buffer_val[3];
    
    //--------------------------------------------------------------------------
    //Decodifica los datos enviados y los guarda en dos cadenas auxiliares
    for(char i=0;i<long_cad;i++)
    {
        if(!flag)
        {
            buffer_set[i] = cmd[i];
            if(buffer_set[i] == ':')    flag=1;
        }
        else
        {
            buffer_val[j] = cmd[i];
            j++;
        }
    }
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    //Comando seteado:
    if(buffer_set[0]=='s' && buffer_set[1]=='e' && buffer_set[2]=='t' && buffer_set[3]=='p' && buffer_set[4]=='w' && buffer_set[5]=='m' && buffer_set[6]==':') comando=1;
//    if(strcmp(buffer_set,"setpwm:")==0)       comando=1;
    if(buffer_set[0]=='s' && buffer_set[1]=='e' && buffer_set[2]=='t' && buffer_set[3]=='e' && buffer_set[4]=='s' && buffer_set[5]=='t' && buffer_set[6]==':') comando=2;
//    if(strcmp(buffer_set,"setest:")==0)       comando=2;
    if(buffer_set[0]=='s' && buffer_set[1]=='e' && buffer_set[2]=='t' && buffer_set[3]=='r' && buffer_set[4]=='e' && buffer_set[5]=='s' && buffer_set[6]=='e' && buffer_set[7]=='t') comando=3;
//    if(strcmp(buffer_set,"setreset:")==0)     comando=3;
    if(buffer_set[0]=='g' && buffer_set[1]=='e' && buffer_set[2]=='t' && buffer_set[3]=='t' && buffer_set[4]=='m' && buffer_set[5]=='p') comando=4;
//    if(strcmp(buffer_set,"gettmp:")==0)       comando=4;
    if(buffer_set[0]=='h' && buffer_set[1]=='e' && buffer_set[2]=='l' && buffer_set[3]=='p') comando = 5;
//    if(strcmp(buffer_set,"help:")==0)         comando=5;
    //--------------------------------------------------------------------------
    
    //Comandos:
    switch(comando)
    {
        //No existe:
        case 0:
        {
            Serial_print("cmd > "),Serial_println("El comando no existe");
            Serial_println("");
            Serial_print("cmd < ");
        break;
        }
        //Setea pwm:
        case 1:
        {
            if(est_m)
            {
                pwm_c = 1;
                ver = 1;
                
                pwm1 = buffer_val[0] - 48;                  // 1
                pwm1 = (pwm1 * 10) + (buffer_val[1] - 48);  // 10 + 2
                pwm1 = (pwm1 * 10) + (buffer_val[2] - 48);  // 120 + 3 = 123
                if(pwm1 > 100)   pwm1 = 100;
                
                Serial_print("cmd > "),Serial_println("Seteando valor pwm");
                __delay_ms(2);
                Serial_print("      Set:"),Serial_print(buffer_val),__delay_ms(2),Serial_println("%");
            }
            if(est_m == 0)
            {
                Serial_print("cmd > "),Serial_println("Debe prender el motor");
            }
            Serial_println("");
            Serial_print("cmd < ");
            
        break;
        }
        //Setea el estado del motor
        case 2:
        {
            ver = 1;
            Serial_print("cmd > "),Serial_println("Seteando estado del motor");
            if((buffer_val[0]-48)==0)       est_m=0,est_c=1,Serial_println("      Motor:apagado");
            else if((buffer_val[0]-48)==1)  est_m=1,est_c=1,Serial_println("      Motor:prendido");
            Serial_println("");
            Serial_print("cmd < ");
        break;
        }
        //Resetea el controlador
        case 3:
        {
            Serial_print("cmd > "),Serial_println("Reseteando el controlador");
            Serial_println("");
            Serial_println("cmd < bloqueado"),Serial_println("");
            __delay_ms(3000);
            RESET();
        break;
        }
        //Devuelve el valor de temperatura:
        case 4:
        {
            Serial_print("cmd > "),Serial_println("Obteniendo el valor de temperatura:");
            vis = 1;
        break;
        }
        //Lista los comandos disponibles:
        case 5:
        {
            Serial_print("cmd > "),Serial_println("Listado de comandos disponibles:");
            Serial_print("      "),Serial_print(setest),Serial_println("'1' o '0'");
            Serial_print("      "),Serial_print(setpwm),Serial_println("'000-100'");
            Serial_print("      "),Serial_println(gettmp);
            Serial_print("      "),Serial_println(setreset);
            Serial_println("");
            Serial_print("cmd < ");
        break;
        }
    }
    
    //--------------------------------------------------------------------------
    //Limpia todo el buffer cmd[]
    for(char i=0;i<long_cad+1;i++)
    {
        cmd[i] = 0;
    }
    fin_carro=0,long_cad=0,comando=0;
    
    return;
}

void __interrupt(high_priority) ISRH(void)
{
    if(PIR1bits.RCIF == 1)
    {
        char aux[2];
        
        //aux[0] = Serial_read();
        //cmd[long_cad] = aux[0];
        
        cmd[long_cad] = Serial_read();
        
        if(aux[0] != 13) long_cad++,Serial_write(aux[0]);
        else    fin_carro=1;

        PIR1bits.RCIF = 0;
    }
    
    return;
}

void __interrupt(low_priority)  ISRL(void)
{
    if(INTCONbits.TMR0IF == 1)
    {
        if(Timer_Lect_Temp != 0)        Timer_Lect_Temp--;
        if(Timer_Most_Pant_Temp != 0)   Timer_Most_Pant_Temp--;
        
        if(Timer_Lect_Temp == 0 || Timer_Most_Pant_Temp == 0)    Timers();
        
        TMR0 = 59;
        T0CONbits.TMR0ON  = 1;
        INTCONbits.TMR0IF = 0;
    }
    
    return;
}

void Timers(void)
{
    if(Timer_Lect_Temp == 0)
    {
        Ds18b20_convert();
        temp = Ds18b20_Lect();
        Most_Temp = 1;
        Timer_Lect_Temp = TICKS_TMP;
    }
    if(Timer_Most_Pant_Temp == 0)
    {
        vis_p_temp = 1;
        
        Timer_Most_Pant_Temp = TICKS_MPT;
    }
    
    return;
}