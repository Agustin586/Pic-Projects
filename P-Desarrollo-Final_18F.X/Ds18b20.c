#include "Configuracion_Bits.h"
#include "Ds18b20.h"

///////////////////////////////////////////////////////////////////////
//                                                                   //
// NOTA: No olvidar la resistencia de 4,7K entre el pin RA4 y Vdd.   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

/*
//-----------------------------------------------------------------------------
// Genera un pulso de calibración de 50uSec. (Ajustar con Osciloscopio)
void OW_Calibrate(int timebase)
{
	switch (timebase)
	{
	case 5:											
		while (1)
		{
			DQ_LOW          // DQ pasa a nivel bajo
                        __delay_us(6);  // Espera 6uSec
			DQ_RELEASE;     // DQ pasa a nivel alto
			__delay_us(6);  // Espera 6uSec
             }
		
	case 50:										
		while (1)
		{
			DQ_LOW		// DQ pasa a nivel bajo
			__delay_us(50); // Espera 50uSec.
                        DQ_RELEASE; 	// DQ pasa a nivel alto
			__delay_us(50); // Espera 50uSec.
		}
	}
	return;
}
*/

int OWReset(void)
{
	int result=1;
	DQ_LOW
        __delay_us(500); // Espera 500uS DQ es bajo al menos por 480 uSec
	DQ_RELEASE; 	// DQ pasa a uno lógico por la resistencia de 4.7K
	__delay_us(60); // Espera 60uSec. Entre 15 y 60uSec es correcto
	result = (int)DQ_READ
	__delay_us(240); // Espera 240uS para completar el pulso de presencia
     return (result);	 // Si es 0 encontró un dispositivo!!!
}

void OWWriteBit(int x)
{
	if (x)
	{
		// Escribe '1' bit
		DQ_LOW          // DQ low para generar la ventana de tiempo
		__delay_us(6);  // Espera 6uS
		DQ_RELEASE	// Libera el pin que pasa a nivel alto
		__delay_us(60); // Espera 60usec para que temine
	}
	else
	{
		// Escribe '0' bit
		DQ_LOW              // DQ low para generar la ventana de tiempo
		__delay_us(60);     // Espera 60usec para que temine
		DQ_RELEASE          // Libera el pin que pasa a nivel alto
	}
__delay_us(10);
}

int OWReadBit(void)
{
	int result=0;
	DQ_LOW								
	__delay_us(6);					
	DQ_RELEASE				
	__delay_us(6);
	result = (int)DQ_READ		 				
	__delay_us(60);
 	return (result);
}

void OWWriteByte(int data)
{
 int loop;
	for (loop = 0; loop < 8; loop++)
	{
	    OWWriteBit(data & 0x01);
		data >>= 1;
	}
}

int OWReadByte(void)
{
  int loop, result=0;
	for (loop = 0; loop < 8; loop++)
	{
	   result >>= 1;
	    if (OWReadBit())
		result |= 0x80;
	}
	return result;
}

int OWTouchByte(int data)
{
	int loop, result=0;
	
	for (loop = 0; loop < 8; loop++)
	{
		result >>= 1;
		if (data & 0x01)
		{
			if (OWReadBit())
				result |= 0x80;
		}
		else
			OWWriteBit(0);
		data >>= 1;
	}
	return result;
}

void OWBlock(unsigned char *data, int data_len)
{
	int loop;
	
	for (loop = 0; loop < data_len; loop++)
	{
		data[loop] = OWTouchByte(data[loop]);
	}
}

void Convert_Temperature(void)
{
	OWReset();			// resets bus
	OWWriteByte(0xCC);		// skip ROM
	OWWriteByte(0x44);		// convertir temperatura
	OWReset();			// resets bus
}

float Read_Temperature_DS18S20(void)
{

	// DS18S20 temperatura formato :
	// LS Byte:	b7		b6		b5		b4		b3		b2		b1		b0
	//        	2^6		2^5		2^4		2^3		2^2		2^1		2^0		2^-1
	//
	// MS Byte:	b15		b14		b13		b12		b11		b10		b9		b8
	//        	S		S		S		S		S		S		S		S

	unsigned char get[10];
	int k, temp;
	float ftemp;

	OWWriteByte(0xCC);		// skip ROM
	OWWriteByte(0xBE);		// leer scratch pad

	for (k=0; k<9; k++)		// leer 9 bytes desde scratchpad
		get[k]=OWReadByte();

	temp = get[0];			// csrgsr lod 8 bits en LSB

	if (get[1] > 0x80)		// colocar signo si es negativo
		temp = temp * -1;

	ftemp = temp;
    return (ftemp/2);
}

float Read_Temperature_DS18B20(void)
{
	// DS18B20 / DS1822 temperatura formato :
	// LS Byte:	b7		b6		b5		b4		b3		b2		b1		b0
	//        	2^3		2^2		2^1		2^0		2^-1	2^-2	2^-3	2^-4
	//
	// MS Byte:	b15		b14		b13		b12		b11		b10		b9		b8
	//        	S		S		S		S		S		2^6		2^5		2^4

	unsigned char get[10];
	int k;
	long temp;
	float ftemp;

	OWWriteByte(0xCC);			// skip ROM
	OWWriteByte(0xBE);			// leer scratch pad

	for (k=0; k<9; k++)		      // lee 9 bytes desde scratchpad
		get[k]=OWReadByte();

	temp = get[1] & 0x7;	// cargar 3 bits de MSB con una mascara
	temp = temp << 8;	
	temp = (temp | get[0]);	 // cargar los 8 bits restantes en LSB

	if (get[1] > 0x80)	// procesar el signo
		temp = temp * -1;

	ftemp = temp;
    return (ftemp/16);
}