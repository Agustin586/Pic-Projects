#include "Task2.h"
#include "RC522.h"
#include "EEPROM_EX.h"
#include "Task_Com_USART.h"

////////////////////////////////////////////////////////////////////////////////

void Task2(unsigned char *Maquina_Estado)
{
    /*
        Tarea subdividida en dos procesos: 1ro --> Lectura de tarjeta Rfid
                                           2do --> Apertura de puertas
    */
    
    if(*Maquina_Estado == 1 && (Timer_Hash == TIMER_HASH))  E_Rfid_Proj();
    else if(*Maquina_Estado == 2)                           E_Ingresos();
    
    *Maquina_Estado = *Maquina_Estado+1;
    if(*Maquina_Estado > 2)    *Maquina_Estado = 1;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

unsigned long  Hash_algoritmo(void)
{
    unsigned long  hash_acum = FNV_BASIS;

    //Convierte el arreglo en un codigo hash
    for (char cont=0;cont<=4;cont++)   // se guardan cinco datos del uid
    {
        //Añiade el valor dentro del Hash
        hash_acum = (hash_acum * FNV_PRIME) ^ UID[cont];
    }

    return (hash_acum);
}

////////////////////////////////////////////////////////////////////////////////

void E_Rfid_Proj(void)
{
    char buffer[20];
    
    if(I_Con_Rfid == 0)  codigo_rfid=0;
        
    //Verifica si se encuentra una tarjeta
    if(MFRC522_isCard(&TagType))
    {
        //Lee la tarjeta un guarda en la posicion de memoria el valor
        if(MFRC522_ReadCardSerial(&UID))
        { 
            //Se convierte a un codigo hash
            codigo_rfid = Hash_algoritmo();
            Tarj_Dect = 1;
        } 
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void E_Ingresos(void)                                                           
{
    //Detecta si se leyo una tarjeta
    if(Tarj_Dect == 1 && Timer_Puerta1!=0)
    {
        //Detecta si esa tarjeta esta guardada en la memoria y la puerta esta
        //cerrada
        if(Read_Hash_ex(codigo_rfid) == 1 && E_Pu1 == 0)
        {
            Pu_1=1;
            Pu_2=1;
            puerta1=1;
            puerta2=0;
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

