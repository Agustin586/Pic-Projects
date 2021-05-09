#include "Configuracion_Bits.h"
#include "Ds18b20.h"

_Bool onewire_init(void)
{
    _Bool pulso=0;
    
    //Pone ne bajo el bus
    ONEWIRE_PIN  = 0;
    ONEWIRE_TRIS = 0;
    
    __delay_us(500);    // Tiempo necesario para resetear todos los dispositivos
    
    //Libera el bus para leer el pulso de reseteo
    ONEWIRE_TRIS = 1;
    __delay_us(80);     // Tiempo necesario para poder leer el pulso
    
    pulso = !ONEWIRE_PIN;    // Lee el estado del bus, si es 1 es correcto
    __delay_us(20);
    
    return pulso;
}

void onewire_write(unsigned char dato)
{
    ONEWIRE_TRIS = 1;   // Libera el bus primero
    
    for(char i=0;i<8;i++)
    {
        ONEWIRE_PIN  = 0;
        ONEWIRE_TRIS = 0;
        
        if((dato & 0x01) == 0x01)
        {
            __delay_us(3);  // Lo mantiene durante 3 micro segundos
            ONEWIRE_TRIS = 1;
            __delay_us(60);
        }
        else
        {
            __delay_us(80);
            ONEWIRE_PIN  = 0;
            ONEWIRE_TRIS = 0;
            __delay_us(7);
        }
        
        valor >>= 1;
    }
    
    ONEWIRE_TRIS = 1;   // Libera el bus para no generar ningun valor erroneo
    
    return;
}

int onewire_read(void)
{
    int result=0;
    
    for(char i=0;i<8;i++)
    {
        ONEWIRE_PIN  = 0;
        ONEWIRE_TRIS = 0;
        __delay_us(5);
        ONEWIRE_TRIS = 1;
        __delay_us(8);
        
        if(ONEWIRE_PIN == 1)    result |= 0x80;
        
        result >>= 1;
    }
    
    ONEWIRE_TRIS = 1;
    
    return result;
}
//
//int owReset(void)
//{
//    char state = 0;
//    OW_TRIS = 0;         //Set as output
//    OW_PORT = 0;        //Drive Low
//    __delay_us(500);
//    OW_TRIS = 1;        //Release, Set back as input
//    __delay_us(80);
//    state = !OW_PORT; //If devices are present, it will keep the pin low
//                                 //! will invert 1=0, 0=1
//    __delay_us(410);
//    return state;           //Returns 1 if devices are present
//}
//
//int owReadBit(void)
//{
//    unsigned int iReadState = 0;
//    OW_TRIS = 0; //Set as output
//    OW_PORT = 0; //Drive low
//    __delay_us(4);
//    OW_TRIS = 1; //Release, set as input
//    __delay_us(8);
//    iReadState = OW_PORT;
//    return iReadState;
//}
//
//int owReadByte(void)
//{
//    int loop, result = 0;
//    for(loop = 0; loop < 8; loop++)
//    {
//        result >>= 1;
//        if(owReadBit())
//            result |= 0x80;
//    }
//    return result;
//}
//
//void owWriteBit(unsigned int b)
//{
//    OW_TRIS = 0; //Set as output
//    OW_PORT = 0; //Drive low
//
//    if(b==1)
//    {
//        __delay_us(3);
//        OW_TRIS = 1; //Release, set as input
//        __delay_us(62);
//    }
//    else
//    {
//        __delay_us(57);
//        OW_TRIS = 1; //Release, set as input
//        __delay_us(7);
//    }
//}
//
//void owWriteByte(unsigned int data)
//{
//    int loop;
//    for(loop = 0; loop < 8; loop++)
//    {
//        owWriteBit(data &0x01);
//        data >>= 1;
//    }
//}