#include "Configuracion_Bits.h"
#include "Ds18b20.h"
#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Librerias XC8\Include\Lcd.h"


void main(void) {
    ADON = 0;       // Setea todo como digital
    
    Lcd_init();
    
    Lcd_cadena(1,1,"Ds18b20 Lib");
    Lcd_cadena(2,1,"Temp:");
    
    while(1)
    {
        
    }
    return;
}
