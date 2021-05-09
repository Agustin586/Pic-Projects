#ifndef XC_Task1_H
#define	XC_Task1_H
#endif


#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif

#ifndef XC_TECLADO_4X4_H
    #include "TECLADO_4X4.h"
#endif

#ifndef XC_OS_CONFIG_H
    #include "OS_CONFIG.h"
#endif

#ifndef XC_LCD_I2C_H
    #include "LCD20X4_I2C.h"
#endif

#ifndef XC_EEPROM_EX_H
#include "EEPROM_EX.h"
#endif


_Bool mostrar=0,aceptar=0,I_Con_Rfid=0;
unsigned char opc_rfid=0;
char M_variable=0;


void Task1(unsigned char *Pantalla);        // Selecciona la maquina de estado
void E_pant1_Lcd(void);                     // Pantalla 1 
void E_pant2_Lcd(void);                     // Pantalla 2: Son las pantallas principales
void E_Teclado_Lcd(void);                   // Pantalla de funciones especiales
void E_Rtc_Lcd(void);                       // Configuracion del rtc
void Rfid_Lcd(void);                        // Configuraciones del rfid
void Erase_Eeprom(void);                    // Limpia toda la memoria eeprom
void Easter_Egg(void);                      // Funcion easter egg