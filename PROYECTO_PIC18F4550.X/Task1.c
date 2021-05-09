#include "Task1.h"
#ifndef XC_Task2_H
    #include "Task2.h"
#endif

#include "RC522.h"

////////////////////////////////////////////////////////////////////////////////

void Task1(unsigned char *Pantalla)
{
    //Cambia de pantalla cada 1 segundo
    if(Timer_DelayLcd == 0 && tec_p == 0 && mod_rtc == 0)
    {
        if(*Pantalla == 1)                          *Pantalla = 2;
        else if(*Pantalla == 2)                     *Pantalla = 1;
        else if(*Pantalla == 3 || *Pantalla == 4)   *Pantalla = 1;
        
        Timer_DelayLcd = DELAY_LCD; // Re-inicializa el temporizador
        mostrar = 0;                // Muestrar las palabras de otra pantalla
    }
    else if(tec_p == 1 && mod_rtc == 0)             *Pantalla = 3;
    else if(mod_rtc == 1 && tec_p == 0)             *Pantalla = 4;
    
    //Detecta si tiene que limpiar la pantalla
    if(!mostrar)            LCD_command(CLEAR);
    
    //Pantallas del lcd
    if(*Pantalla == 1)        E_pant1_Lcd();
    else if(*Pantalla == 2)   E_pant2_Lcd();
    else if(*Pantalla == 3)   E_Teclado_Lcd();
    else if(*Pantalla == 4)   E_Rtc_Lcd();
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void E_pant1_Lcd(void)
{
    char llave[5],i=0,buffer[20],aux2=key;
    
    //Muestra en pantalla palabras
    if(mostrar == 0)
    {
        LCD_array(1,1,"ID-PIC:");                       // Identificacion del pic en la red
        LCD_array(3,1,"Hora:"),LCD_array(4,1,"Fecha:"); // Tiempo
        mostrar = 1;                                    // Deja de mostrar estas 
        M_variable = 1;                                 // Permite mostrar solo una vez algo
    }

    //Convierte el valor del id del pic en un valor binario en ascii
    while(i<5)
    {
        llave[4-i] = (int)(aux2 % 2);
        aux2 = (int)aux2/2;
        if(llave[4-i] == 0)       llave[4-i] = '0';
        else if(llave[4-i] == 1)  llave[4-i] = '1';
        i++;
    }
        
    if(!Timer_DelayMuestra)
    {
        if(mostrar)
        {
            LCD_array(1,8,llave);           // Identificacion del pic

            sprintf(buffer,"%02d",hora);
            LCD_array(3,7,buffer),LCD_array(3,9,":");   // Muestra la hora

            sprintf(buffer,"%02d",min);
            LCD_array(3,10,buffer),LCD_array(3,12,":"); // Muestra los minutos

            sprintf(buffer,"%02d",seg);
            LCD_array(3,13,buffer);                     // Muestra los segundos

            sprintf(buffer,"%02d",dia);
            LCD_array(4,7,buffer),LCD_array(4,9,"/");   // Muestra el dia

            sprintf(buffer,"%02d",mes);
            LCD_array(4,10,buffer),LCD_array(4,12,"/"); // Muestra el mes

            sprintf(buffer,"%02d",year);
            LCD_array(4,13,buffer);                     // Muestra el añio

        }
        
        Timer_DelayMuestra = DELAY_MUESTRA; // Actualiza los valores cada medio segundo
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void E_pant2_Lcd(void)
{
    char buffer[5], buffer2[10];
    
    if(!Timer_DelayMuestra)
    {
        if(mostrar == 0)
        {
            //Muestra de palabra
            LCD_array(1,1,"CARD:");              // UID de la tarjeta    
            LCD_array(2,1,"USUARIO:");           // Numero de usuario
            LCD_array(4,1,"Cant.Usuarios:");     // Cantidad total de usuarios   
            mostrar = 1;                         // Dejar de mostrar palabras
        }

        if(codigo_rfid != 0)
        {
            //Imprime el codigo hash de la tarjeta o llavero
            sprintf(buffer2,"%010lu",codigo_rfid);
            LCD_array(1,6,buffer2);

            //Imprime el usuario correspondiente al hash
            if(Num_User(codigo_rfid) != 0)
            {
                sprintf(buffer,"%02d",Num_User(codigo_rfid));
                LCD_array(2,9,buffer);
            }
            else    LCD_array(2,9,"--");

            //Imprime la cantidad de usuarios
            sprintf(buffer,"%02d",Read_User(0));
            LCD_array(4,15,buffer);
        }
        
        if(codigo_rfid == 0)
        {
            LCD_array(1,6,"No detec.  ");
            
            //Imprime que nadie pone la tarjeta
            LCD_array(2,9,"--");
            
            //Imprime la cantidad de usuarios
            sprintf(buffer,"%02d",Read_User(0));
            LCD_array(4,15,buffer);
        }
        
        Timer_DelayMuestra = DELAY_MUESTRA;
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void E_Teclado_Lcd(void)
{
    char buffer[10];
    
    //Se ejecuta cada medio segundo
    if(!Timer_DelayMuestra)
    {
        //Muestra palabras
        if(mostrar == 0)
        {
            LCD_array(1,1,"SETEAR CONFIGURACION");
            LCD_array(4,1,"Luego Presione 'A'");
            LCD_array(2,1,"Opcion:");
            mostrar = 1;
        }

        //Muestra variables
        if(valor > 9999)    valor=0;
        sprintf(buffer,"%04d",valor),LCD_array(2,8,buffer);

        //Detecta si sale de la funcion
        if(valor == 3231 && aceptar == 1)       tec_p=0,valor=0,mod_rtc=1,mostrar=0,aceptar=0;
        else if(valor == 522 && aceptar == 1)   tec_p=0,valor=0,mostrar=0,aceptar=0,opc_rfid=0,I_Con_Rfid=1,Rfid_Lcd(),tec_p=0;
        else if(valor == 256 && aceptar == 1)   tec_p=0,valor=0,aceptar=0,Erase_Eeprom(),tec_p=0;
        else if(valor == 777 && aceptar == 1)   Easter_Egg();
        else if(valor != 3231 && valor != 522 && aceptar == 1)  tec_p=0,valor=0,mostrar=0,aceptar=0;
    
        Timer_DelayMuestra = DELAY_MUESTRA;
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void E_Rtc_Lcd(void)
{
    char buffer[10];
    
    //Ejecuta cada medio segundo
    if(!Timer_DelayMuestra)
    {
        //Se escribe una sola vez por cada vez que ingrese a la funcion
        if(mostrar == 0)
        {
            LCD_array(1,1,"CONFIGURACION RTC");
            LCD_array(2,1,"HORA:"),LCD_array(2,10,"DIA:");
            LCD_array(3,1,"MIN:") ,LCD_array(3,10,"MES:");
            LCD_array(4,1,"SEG:") ,LCD_array(4,10,"YEAR:");
            mostrar = 1;
        }
        
        //Ingrese a la configuracion de la hora,dia,mes,etc.
        switch(pos_rtc)
        {
            case 1: 
            {
                if(valor > 24) hora=0,valor=0;  // Genera el limite 
                hora = valor;                   // Toma el valor
                sprintf(buffer,"%02d",hora),LCD_array(2,6,buffer);
                if(aceptar)    pos_rtc++,valor=0,aceptar=0;
                break;
            }
            case 2:
            {   
                if(valor > 59) min=0,valor=0;   
                min = valor;
                sprintf(buffer,"%02d",min),LCD_array(3,5,buffer);
                if(aceptar)    pos_rtc++,valor=0,aceptar=0;
                break;
            }
            case 3:
            {
                if(valor > 59) seg=0,valor=0;
                seg = valor;
                sprintf(buffer,"%02d",seg),LCD_array(4,5,buffer);
                if(aceptar)    pos_rtc++,valor=0,aceptar=0;
                break;
            }
            case 4:
            {
                if(valor > 31) dia=0,valor=0;
                dia = valor;
                sprintf(buffer,"%02d",dia),LCD_array(2,14,buffer);
                if(aceptar)    pos_rtc++,valor=0,aceptar=0;
                break;
            }
            case 5:
            {
                if(valor > 12) mes=0,valor=0;   
                mes = valor;                    
                sprintf(buffer,"%02d",mes),LCD_array(3,14,buffer);
                if(aceptar)    pos_rtc++,valor=0,aceptar=0;
                break;
            }
            case 6:
            {
                if(valor > 99)  year=0,valor=0;
                year = valor;
                sprintf(buffer,"%02d",year),LCD_array(4,15,buffer);
                if(aceptar)    pos_rtc=1,valor=0,mod_rtc=0,esc_rtc=1,aceptar=0;
                break;
            }
        }
        
        //Primero guarda todo los valores en el rtc y luego reinicia escritura_rtc
        if(esc_rtc == 1)    RTC_send_hora(),RTC_send_fecha(),esc_rtc=0;
        Timer_DelayMuestra = DELAY_MUESTRA;
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Rfid_Lcd(void)
{
    char buffer[20];
    unsigned char num_usuario=0,Fin_Config=0;

    LCD_command(CLEAR);
    
    while(1)
    {
        //Menu con opciones para el modulo de tarjeta
        if(!opc_rfid)
        {
            LCD_array(1,1,"  CONFIG. TARJETA   ");
            LCD_array(2,1,"1-AGREGAR");
            LCD_array(4,1,"OPCION:");
        }
        
        switch(opc_rfid)
        {
            case 0:
            {
                if(valor >= 2)   valor=0;
                opc_rfid = valor;
                sprintf(buffer,"%d",opc_rfid);
                LCD_array(4,8,buffer);
            
                break;
            }
            case 1:
            {
                char paso=0;
                
                //Menu de opcion 1:
                LCD_array(1,1,"ACERQUE LA TARJETA");
                LCD_array(2,1,"Luego presione 'A'");
                LCD_array(4,1,"RFID:");

                //Reinicia variables
                codigo_rfid = 0;
                valor = 0;
                
                while(1)
                {
                    //Lee la tarjeta
                    if(codigo_rfid == 0)
                    {
                        E_Rfid_Proj();
                        sprintf(buffer,"%010lu",codigo_rfid);
                        LCD_array(4,6,buffer);                  // Imprime el codigo hash
                    }
                    
                    //Detecta cuando continuar 
                    if(aceptar == 1)    LCD_command(CLEAR),paso+=1,valor=0,aceptar=0;    

                    //Digita el numero de usuario:
                    while(paso == 1)
                    {
                        //Siguiente paso
                        LCD_array(1,1,"NUMERO USUARIO");
                        LCD_array(4,1,"NUM:");
                        
                        if(valor > 99)  valor=0;
                        num_usuario = valor;
                        sprintf(buffer,"%02d",num_usuario);
                        LCD_array(4,5,buffer);

                        if(aceptar == 1)  paso+=1,aceptar=0;  
                    }
                        
                    //Escribe el hash y el numero de usuario en la memoria:
                    while(paso == 2)
                    {
                        //Primero debe detectar si el hash y el usuario ya existen
                        //Detecta si el usuario no existe
                        if((User_Scan(num_usuario)==0) && (Read_Hash_ex(codigo_rfid) == 0))   
                        {
                            unsigned int ubicacion=0;

                            ubicacion = New_User_Pos();         // Ubica el nuevo usuario
                            Write_User(ubicacion,num_usuario);  // Escribe el numero de usuario
                            __delay_ms(20);                     // Espera para guardar los datos escritos
                            M_Write((ubicacion+3),codigo_rfid); // Escribe el hash
                            __delay_ms(20);                     // Espera para guardar los datos escritos
                            Sum_Cant_Usuarios();                // Suma 1 a la cantidad de usuarios
                            __delay_ms(20);                     // Espera para guardar los datos escritos
                            
                            valor=0,Fin_Config=1;
                            
                            char buffer[20];
                            
                            //Verificacion mediante serial
                            
                            sprintf(buffer,"%d",ubicacion);
                            UART_array("UBICACION:"),UART_array(buffer),UART_array("\n"),UART_write(13);
                            __delay_ms(5);
                            
                            sprintf(buffer,"%d",Read_User(0));
                            UART_array("CANTIDAD USUARIOS:"),UART_array(buffer),UART_array("\n"),UART_write(13);
                            
                            sprintf(buffer,"%d",Read_User(ubicacion));
                            UART_array("NUMERO DE USUARIO:"),UART_array(buffer),UART_array("\n"),UART_write(13);
                            __delay_ms(5);
                            
                            sprintf(buffer,"%lu",M_Read((ubicacion+3),10));
                            UART_array("RFID:"),UART_array(buffer),UART_array("\n"),UART_write(13);
                            
                            LCD_command(CLEAR),LCD_array(2,1,"  USUARIO AGREGADO  ");
                            __delay_ms(1000);
                            LCD_command(CLEAR);
                            
                            paso+=1;
                            
                            break;
                        }
                        else
                        {
                            LCD_command(CLEAR),LCD_array(2,1,"  USUARIO EXISTENTE "),__delay_ms(1000),valor=0,mostrar=0;
                            Fin_Config=1;
                            paso +=1;
                            
                            break;
                        }
                    }
                    if(paso == 3)   break;
                }
                break;
            }
        }
        if(Fin_Config == 1) break;
    }
    
    I_Con_Rfid = 0;

    return;
}

////////////////////////////////////////////////////////////////////////////////

void Erase_Eeprom(void)
{
    //Presentacion
    LCD_command(CLEAR);
    LCD_array(1,1,"Restableciendo");
    LCD_array(2,1,"a ajustes de fabrica");
    __delay_ms(1000);
    
    //Comienza el borrado
    EEPROM_init();
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Easter_Egg(void)
{
    LCD_command(CLEAR);
    LCD_array(1,1,"*-*-*-*-*-*-*-*-*-*-");
    LCD_array(2,1," EASTER EGG FUNTION ");
    LCD_array(3,1,":) :) :) :) :) :) :)");
    LCD_array(4,1,"????????????????????");
    __delay_ms(2000);
    
    LCD_command(CLEAR);
    LCD_array(2,1,"        3...        ");
    __delay_ms(1000);
    LCD_command(CLEAR);
    LCD_array(2,1,"        2...        ");
    __delay_ms(1000);
    LCD_command(CLEAR);
    LCD_array(2,1,"        1...        ");
    __delay_ms(1000);
    LCD_command(CLEAR);
    LCD_array(2,1,"  AUTODESTRUCCION   ");
    __delay_ms(1000);

    RESET();
    
    return;
}

////////////////////////////////////////////////////////////////////////////////