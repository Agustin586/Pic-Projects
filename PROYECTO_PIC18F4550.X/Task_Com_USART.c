#include "Task_Com_USART.h"
#include "Task2.h"


unsigned int direccion=0;                                                       //Para establecer la direccion de memoria

////////////////////////////////////////////////////////////////////////////////

_Bool Bluetooth_Com(void)
{
    _Bool bluetooth=0,act_var=0,switch_est=0;
    const char H_P1_txt[]={"hp1."},H_P2_txt[]={"hp2."},actualizar_txt[]={"act."};
    
    if(strcmp(word,"s1_on.") == 0)     Pu_1 = 1,switch_est=1;
    else if(strcmp(word,"s1_off.") == 0)Pu_1 = 0,switch_est=1;
    if(strcmp(word,"s2_on.") == 0)     Pu_2 = 1,switch_est=1;
    else if(strcmp(word,"s2_off.") == 0)Pu_2 = 0,switch_est=1;
    
    if(strcmp(H_P1_txt,word) == 0)  Pu_1=1,puerta1=1;
    if(strcmp(H_P2_txt,word) == 0)  Pu_2=1,puerta2=1;
    else if(strcmp(actualizar_txt,word) == 0)
    {
        if(E_Pu1 == 1)  UART_array("1: Abierta");
        else            UART_array("1: Cerrada");
        if(E_Pu2 == 1)  UART_array("2: Abierta");
        else            UART_array("2: Cerrada");
        act_var = 1;
    }
    else if(strcmp(H_P1_txt,word) == 1 && strcmp(H_P2_txt,word) == 1 && act_var == 0 && switch_est == 0)   bluetooth=1;
    
    //Borra los datos enviados
    if(bluetooth == 0)
    {
        for(char i=0;i<=long_cad;i++)   word[i] = 0;               
        Fin_Cad = 0,long_cad = 0;
    }
    
    return bluetooth;
}

////////////////////////////////////////////////////////////////////////////////

void Task_Com_USART(void)
{   
    char i=0,aux2=key;
    
    //Conversion de decimal a binario
    while(i < 5)
    {
        llave[4-i] = (int)(aux2 % 2);
        aux2 = (int)aux2/2;
        if(llave[4-i] == 0)       llave[4-i] = '0';
        else if(llave[4-i] == 1)  llave[4-i] = '1';
        i++;
    }
    i = 0;
    
    Read_Com();             // Lee el mensaje e interpreta la accion a realizar
    
    //Realiza la accion solo si coinciden con la id del pic
    if(address == key)
    {
        if(CR == 1)         ACN_CR_Com();
        else if(PA == 1)    ACN_PA_Com();
        else if(CO == 1)    ACN_CO_Com();
    }
    else    UART_array("\n"),UART_write(13);
    
    address = 0;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Read_Com(void)
{
    if(Fin_Cad == 1)
    {
        if(( word[0] == '[' ) && ( word[long_cad-1] == ']' ))
        {
            //Detecta los datos enviados
            for(char i=1;i<=(long_cad-2);i++)
            {
                //Detecta el tipo de accion
                if(i >= 1 && i <= 2)
                {
                    if(word[1] == 'C' && word[2] == 'O')        CO = 1;
                    else if(word[1] == 'C' && word[2] == 'R')   CR = 1;
                    else if(word[1] == 'P' && word[2] == 'A')   PA = 1;
                }
                //Detecta el pedido 
                else if(i == 4)
                {
                    if(word[4] == '1')      N = 1;
                    else if(word[4] == '2') N = 2;
                    else if(word[4] == '3') N = 3;
                    else if(word[4] == '4') N = 4;
                }
                //Detecta la direccion
                else if(i >= 6 && i <= 10)
                {
                    if(word[i] == '1')      address |= 1;
                    else if(word[i] == '0') address |= 0;
                    if(i != 10)             address <<= 1;
                }
                //Detecta el valor de XX e YY
                else if(i >= 12 && i <= 16)
                {
                    if(i == 12)         XX = 10 * (word[i] - 48);
                    else if(i == 13)    XX = XX + word[i] - 48;
                    if(i == 15)         YY = 10 * (word[i] - 48);
                    else if(i == 16)    YY = YY + word[i] - 48;
                }
            }
        }
        //Borra los datos enviados
        for(char i=0;i<20;i++)   word[i] = 0;               
        Fin_Cad = 0,long_cad = 0;
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ACN_CR_Com(void)
{
    const char texto1[] = "Disp_LZ";
    const char texto2[] = "Disp que controla una puerta,mediante un lector Rfid";
                                               
    char buffer[10];
    
    //Envia consulta de referencia 1
    if(N == 1)
    {
        const char cant = strlen(texto1);                                       //Devuelve la longitud de la cadena;
        
        UART_array("\n[RCR,1,"),UART_array(llave);
        UART_array(","),        sprintf(buffer,"%d",cant),UART_array(buffer);
        UART_array(","),        UART_array(texto1);
        UART_array("]\n"),      UART_write(13);
    }
    //Envia consulta de referencia 2
    if(N == 2)
    {
        const char cant = strlen(texto2);                                       //Devuelve la longitud de la cadena;
        
        UART_array("\n[RCR,2,"),UART_array(llave);
        UART_array(","),        sprintf(buffer,"%d",cant),UART_array(buffer);
        UART_array(","),        UART_array(texto2);
        UART_array("]\n"),      UART_write(13);
    }
    CR=0,CO=0,PA=0;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ACN_PA_Com(void)
{
    char buffer[10];
    
    //Numero de pedido de accion
    switch(N)
    {
        //Pide o bloquea el acceso de la puerta
        case 1:                                                       
        {
            if(XX == 01)    Ha_Pu=1;
            else            Ha_Pu=0;
            
            if(Ha_Pu == 1)
            {
                if((YY == 01) && (E_Pu1 == 0))  Pu_1=1,puerta1=1,puerta2=0;     // Abre la primera puerta 1
                else                            Pu_1=0;                         // Cierra la puerta 1
                if((YY == 02) && (E_Pu2 == 0))  Pu_2=1,puerta2=1,puerta1=0;     // Abre la segunda puerta 2
                else                            Pu_2=0;                         // Cierra la puerta 2
            }
            if(Ha_Pu == 0)  Pu_1=0,Pu_2=0;
            
            Ha_Pu = 0;
            
        break;
        }
        
        //Pide ingresar la hora y los minutos
        case 2:
        {
            LCD_command(CLEAR);
            
            LCD_array(2,1,"     Modificando    ");
            LCD_array(3,1,"     valores ...    ");
            __delay_ms(1000);
            LCD_command(CLEAR);
            
            hora = XX, min = YY, seg = 1;
            RTC_send_hora();
            LCD_array(2,1,"     Terminado     ");
            __delay_ms(1000);
            
            LCD_command(CLEAR);
            
        break;
        }
        
        //Pide ingresar la fecha y el mes
        case 3:
        {
            LCD_command(CLEAR);
            

            LCD_array(2,1,"     Modificando    ");
            LCD_array(3,1,"     valores ...    ");
            __delay_ms(1000);
            LCD_command(CLEAR);

            dia = XX, mes = YY, year = 20;
            RTC_send_fecha();
            LCD_array(2,1,"     Terminado     ");
            __delay_ms(1000);
            
            LCD_command(CLEAR);
            
        break;
        }
        
        //Pide el borrado de la memoria
        case 4:
        {
            Erase_Eeprom();
            
        break;
        }
        
    }
    PA=0,CR=0,CO=0;
    
    //Respuesta al pedido de accion
    UART_array("\n[RPA,"),      sprintf(buffer,"%d",N);
    UART_array(buffer),         UART_array(",");
    UART_array(llave),          sprintf(buffer,"%02d",XX);
    UART_array(","),            UART_array(buffer);
    sprintf(buffer,"%02d",YY),  UART_array(",");
    UART_array(buffer),         UART_array("]\n");
    UART_write(13);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ACN_CO_Com(void)
{
    char buffer[10];
    
    //Consulta de tipo:
    switch(N)
    {
        //Pregunta que puerta esta abierta o cerrada
        case 1:
        {
            if(XX == 01)    if(E_Pu1 == 1)     xx=01, yy=01;                    //Puerta 1 esta abierta
            if(XX == 02)    if(E_Pu2 == 1)     xx=02, yy=01;                    //Puerta 2 esta abierta
            if(XX == 01)    if(E_Pu1 == 0)     xx=01, yy=00;                    //Puerta 1 esta cerrada
            if(XX == 02)    if(E_Pu2 == 0)     xx=02, yy=00;                    //Puerta 2 esta cerrada
            
        break;
        }
        
        //Pregunta cualquier usuario
        case 2:                                                                 //Mensaje: yy numero de usuario  xx si existe o no
        {
            if(User_Scan(YY))       yy = YY, xx = 1;                            //Pone un 1 si en usuario existe
            else                    yy = YY, xx = 0;                            //Pone un 0 si no existe
            
        break;
        }
        
        //Pregunta la cantidad de usuarios
        case 3:                                                                 //Mensaje: xx cant_user
        {
            xx = cant_user;
            
        break;
        }
        
        //Pregunta la hora y minutos
        case 4:
        {
            xx=hora;
            yy=min;
            
        break;
        }
    }
    
    CO=0,PA=0,CR=0;
    
    //Respuesto a consulta
    UART_array("\n[RCO,"),      sprintf(buffer,"%d",N);
    UART_array(buffer),         UART_array(",");
    UART_array(llave),          sprintf(buffer,"%02d",xx);
    UART_array(","),            UART_array(buffer);
    sprintf(buffer,"%02d",yy),  UART_array(",");
    UART_array(buffer),         UART_array("]\n");
    UART_write(13);
    
    xx=00,yy=00;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////