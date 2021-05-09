#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del cristal
#endif

#define ONEWIRE_PIN     PORTBbits.RB0
#define ONEWIRE_TRIS    TRISBbits.TRISB0

#define TEMP_CONV   0x44

#define Onewire_reset   onewire_init
#define Onewire_esc     
#define Onewire_lec

_Bool onewire_init(void);
void onewire_write(unsigned char dato);
int onewire_read(void);