/*
 * File:   Main.c
 * Author: AGUSTIN ZULIANI
 *
 * Created on 4 de octubre de 2020, 12:27
 */

#include "Configuracion_Bits.h"
#include "Ili9341.h"

void main(void) {
    
    ADON = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TFT_init();
    
    TFT_fillscreen(BLACK);
    
    TFT_text(170,100,"Agustin Zuliani",YELLOW,BLACK);
    TFT_text(10,20,"Controlador: Ili9341",RED,BLACK);
//        TFT_text(80,40,"Controlador: Ili9341 Mplab XC8",RED,BLACK);
    TFT_fill_rect(150,30,120,20,BLUE);
    //TFT_fill_circ(70,90,5,PINK);
    TFT_rect(50,80,60,180,CYAN);
    
    while(1)
    {
//        __delay_ms(2000);
//        TFT_rotation(0);
//        __delay_ms(1000);
//        TFT_rotation(1);
//        TFT_fillscreen(GREEN);
//        TFT_text(170,100,"Agustin Zuliani",YELLOW,BLACK);
//    TFT_text(10,4,"Controlador: Ili9341 Mplab XC8",RED,BLACK);
//        __delay_ms(1000);
//        TFT_rotation(2);
//        TFT_fillscreen(BLACK);
//        TFT_text(170,100,"Agustin Zuliani",YELLOW,BLACK);
//    TFT_text(10,4,"Controlador: Ili9341 Mplab XC8",RED,BLACK);
//        __delay_ms(1000);
//        TFT_rotation(3);
//        TFT_fillscreen(PURPLE);
//        TFT_text(170,100,"Agustin Zuliani",YELLOW,BLACK);
//    TFT_text(10,4,"Controlador: Ili9341 Mplab XC8",RED,BLACK);
    }
    
    return;
}
