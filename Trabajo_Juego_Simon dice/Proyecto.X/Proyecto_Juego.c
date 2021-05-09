#include <xc.h>
// PIC16F628A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include<stdlib.h>
#include<time.h>
#define _XTAL_FREQ 4000000
#define led1 RB1
#define led2 RB0
#define led3 RB2
#define led4 RB3
#define buzzer RB5
#define P1 RA3
#define P2 RA2
#define P3 RA1
#define P4 RA0

void secuencia(int [],int tiempo,char contador);
void antirrebote(void);
char verificacion(int [],char cantidad);
void reseteo(void);
void inicio(int tiempo);
void Delay_ms(int tiempo_de_delay);
void valor_aleatorio(char contador,int valor);
void Delay_us(char contador);

int sec[40];
int t=0;
int valor_aleat=1;
int valores;
int ult_valor=0;
char cont=0;

void main(void) {
    TRISA=0b11111111;
    TRISB=0b11010000;
    CMCON=0b00000111;
    
    while(1){
        int nivel=0;        
        
        valor_aleat=1;
        
        while(P1==0 && P2==0 && P3==0 && P4==0){            
            led1=1,led2=1,led3=1,led4=1;            
            __delay_ms(200);
            led1=0,led2=0,led3=0,led4=0;
            __delay_ms(200);
        }
        if(P1==1)   nivel=1;
        if(P2==1)   nivel=2;
        if(P3==1)   nivel=3;
        if(P4==1)   nivel=4;
        
        antirrebote();
        
        switch(nivel){
            case 1: t=200;
                break;
            case 2: t=170;
                break;
            case 3: t=140;
                break;
            case 4: t=110;
                break;
            default:
                break;
        }
        inicio(t);
        secuencia(sec,t,cont);
    }
    return;
}

void inicio(int tiempo){
    
    char i=1,error=0;
    int tiempo_delay=tiempo;
    
    cont=0;
    
    while(error==0 && i<=40){   
        
        cont++;
        
        valor_aleatorio(cont,valores);
        
        srand(valor_aleat);
        
        ult_valor=rand() % (4-1+1) + 1;
        
        sec[i]=ult_valor;
        
        antirrebote();
        
        secuencia(sec,tiempo_delay,cont);
        
        error=verificacion(sec,cont);
                
        if(i==40){
            while(P1==0){
                led1=1,led2=1,led3=1,led4=1;
                __delay_ms(200);
                led1=0,led2=0,led3=0,led4=0;
                __delay_ms(200);
            }
        }
        
        reseteo();
        
        i++;
    
    }
 
}

void reseteo(void){
        
    led1=1,led2=1,led3=1,led4=1,buzzer=1;
        
    __delay_ms(1000);
        
    led1=0,led2=0,led3=0,led4=0,buzzer=0;
        
    __delay_ms(500);
}

void secuencia(int arreglo[],int tiempo,char contador){
    for(int j=0;j<=contador;j++){
        
        if(arreglo[j]==1){
            led1=1,buzzer=1;
            Delay_ms(tiempo);
            led1=0,buzzer=0;
        }
        
        if(arreglo[j]==2){
            led2=1,buzzer=1;
            Delay_ms(tiempo);
            led2=0,buzzer=0;
        }
        
        if(arreglo[j]==3){
            led3=1,buzzer=1;
            Delay_ms(tiempo);
            led3=0,buzzer=0;
        }
        
        if(arreglo[j]==4){
            led4=1,buzzer=1;
            Delay_ms(tiempo);
            led4=0,buzzer=0;
        }
    
        Delay_ms(tiempo);
    }
}

char verificacion(int arreglo[],char cantidad){
    char aux=0;
    char x=1;
    
    while(x<=cantidad && aux==0){
              
        if(sec[x]==1){
            
            __delay_ms(1);
            
            while(P1==0 && P2==0 && P3==0 && P4==0){                
            }
            
            if(P1==1){
                antirrebote();
                led1=1,buzzer=1;
                __delay_ms(120);
                led1=0,buzzer=0;
                aux=0;
            }
            
            else if(P2==1 || P3==1 || P4==1)    aux=1;
        
            antirrebote();
            
        }
        
        if(sec[x]==2){
            
            __delay_ms(1);
            
            while(P1==0 && P2==0 && P3==0 && P4==0){
            }
            
            if(P2==1){
                antirrebote();
                led2=1,buzzer=1;
                __delay_ms(120);
                led2=0,buzzer=0;
                aux=0;
            }
        
            else if(P1==1 || P3==1 || P4==1)    aux=1;
        
            antirrebote();
        
        }
        
        if(sec[x]==3){
            
            __delay_ms(1);
            
            while(P1==0 && P2==0 && P3==0 && P4==0){
            }
            
            if(P3==1){
                antirrebote();
                led3=1,buzzer=1;
                __delay_ms(120);
                led3=0,buzzer=0;
                aux=0;
            }
            
            else if(P1==1 || P2==1 || P4==1)    aux=1;
        
            antirrebote();
        
        }
        
        if(sec[x]==4){

            __delay_ms(1);
            
            while(P1==0 && P2==0 && P3==0 && P4==0){
            }
            
            if(P4==1){
                antirrebote();
                led4=1,buzzer=1;
                __delay_ms(120);
                led4=0,buzzer=0;
                aux=0;
            }
            
            else if(P1==1 || P3==1 || P2==1)    aux=1;
        
            antirrebote();
        
        }
        
        x++;
    }
    return aux;
}

void antirrebote(void){
    __delay_ms(30);
    valores++;
    while(P1==1 || P2==1 || P3==1 || P4==1){
        __delay_ms(30);
        valores++;
    }
}

void Delay_ms(int tiempo_de_delay){
    char i=1;
    while(i<=tiempo_de_delay){
        __delay_ms(1);
        i++;
    }
}

void valor_aleatorio(char contador,int valor){
    int c=1;
    while(c<contador+valores){
        c=c+valor_aleat;
        c++;
        valor_aleat++;
        if(valor_aleat==5){
            valor_aleat=1;
        }
    }
}