#include "Configuracion_Bits.h"
#include "Ili9341.h"

volatile uint16_t LCD_W = LCD_WIDTH;
volatile uint16_t LCD_H = LCD_HEIGHT;

void ili9341_init(void)
{
    //Configuracion de pines
    RST_PIN  = 1;
    RST_TRIS = 0;
    CS_PIN  = 1;
    CS_TRIS = 0;
    RS_PIN  = 1;
    RS_TRIS = 0;
    RD_PIN  = 1;
    RD_TRIS = 0;
    WR_PIN  = 1;
    WR_TRIS = 0;

    //Pone como salid el modo de hardware reset
    RST_PIN  = 1;
    RST_TRIS = 0;
    __delay_ms(200);
    RST_PIN  = 0;
    __delay_ms(200);
    RST_PIN  = 1;
    __delay_ms(200);
    
    //Configuracion inicial
    TFT_cmd(SW_RESET);      // Reset por software
    __delay_ms(200);
    TFT_cmd(DISPLAY_OFF);   // Apaga el display
    //Configura el power control 1
    TFT_cmd(POWER_CTRL_1);
    TFT_print(0x26);        // Setea el vcom con 4.75v
    //Configura el power control 2
    TFT_cmd(POWER_CTRL_2);
    TFT_print(0x11);        // Sete el step up en el mas bajo posible
    //Configura el vcom control 1
    TFT_cmd(VCOM_CTRL_1);
    TFT_print(0x5C);        // VcomH = 4.75v    // Verificar de nuevo esto 
    TFT_print(0x4C);        // VcomL = 0v       //en los datos
    //Configura el vcom control 2
    TFT_cmd(VCOM_CTRL_2);
    TFT_print(0x94);
    //Configura el control de memoria
    TFT_cmd(MEMORY_ACC_CTRL);
    TFT_print(0x48);        // my,mx,mv,ml,bgr,mh,0,0
    //Configura el formato del pixel
    TFT_cmd(PIXEL_FORMT_SET);
    TFT_print(0x55);        // Setea la interface rgb y mcu en 16 bits
    //Configura el control de cuadros por segundos
    TFT_cmd(FRAME_RATE_CTRL_NRM_FULLCOLORS);
    TFT_print(0x00);        // Setea la division del fosc en 0
    TFT_print(0x1B);        // Setea en modo por defecto de 70 cuadros por segundo
    //TFT_print(0x15);        // Setea en modo por defecto de 90 cuadros por segundo
    //Configura la Gamma
    TFT_cmd(0xF2);          // 3Gamma Function Disable
    TFT_print(0x08);        // Estos dos comandos no existen --> 0xF2 y 0x08
    TFT_cmd(GAMMA_SET);     
    TFT_print(0x01);        // gamma set 4 gamma curve 01/02/04/08
    TFT_cmd(POST_GAMMA_CORRECT); 
    TFT_print(0x1F);
    TFT_print(0x1a);
    TFT_print(0x18);
    TFT_print(0x0a);
    TFT_print(0x0f);
    TFT_print(0x06);
    TFT_print(0x45);
    TFT_print(0x87);
    TFT_print(0x32);
    TFT_print(0x0a);
    TFT_print(0x07);
    TFT_print(0x02);
    TFT_print(0x07);
    TFT_print(0x05);
    TFT_print(0x00);
    TFT_cmd(NEGT_GAMMA_CORRECT);
    TFT_print(0x00);
    TFT_print(0x25);
    TFT_print(0x27);
    TFT_print(0x05);
    TFT_print(0x10);
    TFT_print(0x09);
    TFT_print(0x3a);
    TFT_print(0x78);
    TFT_print(0x4d);
    TFT_print(0x05);
    TFT_print(0x18);
    TFT_print(0x0d);
    TFT_print(0x38);
    TFT_print(0x3a);
    TFT_print(0x1f);  
    //Configura la ddram
    TFT_cmd(COLUMN_ACC_SET);    // Direccion de columnas
    TFT_print(0x00);
    TFT_print(0x00);
    TFT_print(0x00);
    TFT_print(0xEF);
    TFT_cmd(PAGE_ACC_SET);      // Direccion de paginas o filas
    TFT_print(0x00);
    TFT_print(0x00);
    TFT_print(0x01);
    TFT_print(0x3F);
    TFT_cmd(D_INVERSION_OFF);       // Inversion de colores dessactivados
    TFT_cmd(ENTRY_MODE_SET);
    TFT_print(0x07);            // DSTB=0;GAS=disable;GON/DTE=normal display;
    //Configura el display
    TFT_cmd(D_FUNCTION_CTRL);
    TFT_print(0x0A);
    TFT_print(0x82);
    TFT_print(0X27);
    TFT_print(0x00);
    TFT_cmd(SLEEP_OUT);
    __delay_ms(125);
    TFT_cmd(DISPLAY_ON);
    __delay_ms(125);
    TFT_cmd(MEMORY_WRITE);
    __delay_ms(200);

    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_write_command(unsigned char cmd)
{
    //Pone el chip select en bajo 
    CS_PIN  = 0;
    CS_TRIS = 0;
    //Pone en alto el pin de lectura
    RD_PIN  = 1;
    RD_TRIS = 0;
    //Pone el reset en alto
    RST_PIN  = 1;
    RST_TRIS = 0;
    //Pone en modo comando
    RS_PIN  = 0;
    RS_TRIS = 0;
    //Pone en bajo el pin de escribir
    WR_PIN  = 0;
    WR_TRIS = 0;
    
    //Carga los datos 
    LCD_D0 = (cmd & 0x01);
    LCD_D1 = ((cmd >> 1) & 0x01);
    LCD_D2 = ((cmd >> 2) & 0x01);
    LCD_D3 = ((cmd >> 3) & 0x01);
    LCD_D4 = ((cmd >> 4) & 0x01);
    LCD_D5 = ((cmd >> 5) & 0x01);
    LCD_D6 = ((cmd >> 6) & 0x01);
    LCD_D7 = ((cmd >> 7) & 0x01);
    
    //__delay_us(10);
    
    //Pone en alto el pin de escritura
    WR_PIN  = 1;
    //Pone en alto el pin de modo de comando o escritura
    RS_PIN  = 1;
    //Pone en alto el pin de chip select
    CS_PIN  = 1;
    
    //__delay_us(10);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_write_date(unsigned char date)
{
    //Pone el chip select en bajo 
    CS_PIN  = 0;
    CS_TRIS = 0;
    //Pone en modo comando
    RS_PIN  = 1;
    RS_TRIS = 0;
    //Pone en alto el pin de lectura
    RD_PIN  = 1;
    RD_TRIS = 0;
    //Pone el reset en alto
    RST_PIN  = 1;
    RST_TRIS = 0;
    //Pone en bajo el pin de escribir
    WR_PIN  = 0;
    WR_TRIS = 0;
    
    //Carga los datos 
    LCD_D0 = (date & 0x01);
    LCD_D1 = ((date >> 1) & 0x01);
    LCD_D2 = ((date >> 2) & 0x01);
    LCD_D3 = ((date >> 3) & 0x01);
    LCD_D4 = ((date >> 4) & 0x01);
    LCD_D5 = ((date >> 5) & 0x01);
    LCD_D6 = ((date >> 6) & 0x01);
    LCD_D7 = ((date >> 7) & 0x01);
    
    //__delay_us(10);
    
    //Pone en alto el pin de escritura
    WR_PIN  = 1;
    //Pone en alto el pin de modo de comando o escritura
    RS_PIN  = 1;
    //Pone en alto el pin de chip select
    CS_PIN  = 1;
    
    //__delay_us(10);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

unsigned char ili9341_read(void)
{
    unsigned char result=0;
    
    //Pone el pin de escritura o comando en alto
    RS_PIN  = 1;
    RS_TRIS = 0;
    //Pone el pin de escritura en alto
    WR_PIN  = 1;
    WR_TRIS = 0;
    //Pone el pin de chip select en bajo
    CS_PIN  = 0;
    CS_TRIS = 0;
    //Pone el pin de lectura en bajo
    RD_PIN  = 0;
    RD_TRIS = 0;
    
    //Carga los datos de lectura
    if(LCD_D7 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D6 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D5 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D4 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D3 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D2 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D1 == 1) result |= 0x01;
    result <<= 1;
    if(LCD_D0 == 1) result |= 0x01;
    
    //Pone el pin de lectura en alto
    RD_PIN  = 1;
    //Pone el pin de chip select en alto
    CS_PIN  = 1;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_setadress(int sc, int ec, int sp, int ep)
{
    TFT_cmd(COLUMN_ACC_SET);
    TFT_print(sc >> 8);         // Envia MSB de start column
    TFT_print(sc & 0x00FF);     // Envia LSB de start column
    TFT_print(ec >> 8);         // ||           end   column
    TFT_print(ec & 0x00FF);     // ||
    
    TFT_cmd(PAGE_ACC_SET);
    TFT_print(sp >> 8);         // Envia MSB de start page
    TFT_print(sp & 0x00FF);     // Envia LSB de start page
    TFT_print(ep >> 8);         // ||           end   page
    TFT_print(ep & 0x00FF);     // ||           
    
    TFT_cmd(MEMORY_WRITE);      // Guarda los datos
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_pushcolor(int color)
{
    TFT_print(color >> 8);
    TFT_print(color & 0x00FF);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_fill(int color)
{
    //Setear el inicio desde el que se va a escribir
    ili9341_setadress(0,LCD_W,0,LCD_H);
    
    for(int i=0;i<LCD_H;i++)
    {
        for(int j=0;j<LCD_W;j++)
        {
            ili9341_pushcolor(color);
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_fillrect(int x, int y, int w, int h, unsigned int color)
{
    if((x >= LCD_W)||(y >= LCD_H))  return;
    
    if((x+w-1) >= LCD_W)    w = LCD_W-x;
    if((y+h-1) >= LCD_H)    h = LCD_H-y;
    
    ili9341_setadress(x,x+w-1,y,y+h-1);
    
    for(y=h;y>0;y--)
    {
        for(x=w;x>0;x--)
        {
            ili9341_pushcolor(color);
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_drawrect(int x, int y, int w, int h,unsigned int color)
{
    ili9341_drawvline(x,y,h,color);
    ili9341_drawvline(x+w,y,h,color);
    ili9341_drawhline(x,y,w,color);
    ili9341_drawhline(x,y+h,w,color);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_drawpixel(int x3, int y3, int color)
{
    if((x3 < 0)||(x3 >= LCD_W)||(y3 < 0)||(y3 >= LCD_H))    return;
    
    ili9341_setadress(x3,x3+1,y3,y3+1);
    ili9341_pushcolor(color);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_drawvline(int x, int y, int h, int color)
{
    if((x >= LCD_W)||(y >= LCD_H))    return;
    
    if((y+h-1) >= LCD_H)    h = LCD_H-y;
    
    ili9341_setadress(x,x+1,y,y+h-1);
    
    for(int i=0;i<h-1;i++)
    {
        for(int j=0;j<2;j++)
        {
            ili9341_pushcolor(color);
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_drawhline(int x, int y, int w, int color)
{
    if((x >= LCD_W)||(y >= LCD_H))  return;
    
    if((x+w-1) >= LCD_W)    w = LCD_W-x;
    
    ili9341_setadress(x,x+w-1,y,y+1);
    
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<w-1;j++)
        {
            ili9341_pushcolor(color);
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////


void ili9341_drawdline(int x, int y, int d, _Bool dir, int color)
{
    if((x >= LCD_W) || (y >= LCD_H))    return;
    
    if(!dir)
    {
        for(int i=0;i<d;i++)
        {
            if(i == 0)  ili9341_drawpixel(x,y,color);
            else        ili9341_drawpixel(x+i,y+i,color);
        }
    }
    else
    {
        for(int i=0;i<d;i++)
        {
            if(i == 0)  ili9341_drawpixel(x,y+d,color);
            else        ili9341_drawpixel(x+i,y+d-i,color);
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_drawchar(int x,int y,int fIndex,int color,int bg_color)
{
    uint8_t j, k, i;
    uint16_t iOffset = fIndex + (FONT_WIDTH)*FONT_HEIGHT;
    
    for(j=0; j < font[fIndex]; j++) // variable character width
    {
        ili9341_setcursor(x + font[fIndex] - j, y); // print from right to left
        
        for(k=0; k<FONT_HEIGHT; k++) // _ bytes per character (height)
        {
            for(i=0; i<8; i++)
            {
                if( font[ iOffset - (j*FONT_HEIGHT) - k ] & (0x80 >> i) )
                {
                    ili9341_pushcolor(color);
                }
                else
                {
                    ili9341_pushcolor(bg_color);
                }
            }
        }
    }
    
    return;
}
////////////////////////////////////////////////////////////////////////////////

void ili9341_txt_colour(int colour)
{
    txt_colour = colour;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_txt_xy(int x, int y)
{
    txt_x = x;
    txt_y = y;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_bg_colour(int colour)
{
    bg_colour = colour;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_setcursor(int x,int y)
{
    TFT_cmd(0x2B);
    TFT_print(x >> 8);         // Envia MSB de start column
    TFT_print(x & 0xFF);     // Envia LSB de start column
    
    TFT_cmd(MEMORY_WRITE);      // Guarda los datos
    
    TFT_cmd(0x2A);
    TFT_print(y >> 8);         // Envia MSB de start page
    TFT_print(y & 0xFF);     // Envia LSB de start page
    
    TFT_cmd(MEMORY_WRITE);      // Guarda los datos
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_draw_string (uint16_t x, uint16_t y, const char *pS, uint16_t fg_color, uint16_t bg_color)
{    
    uint16_t lIndex, k;
    
    while(*pS)
    {
        /* index the width information of character <c> */
        lIndex = 0;
        for(k=0; k<(*pS - ' '); k++)
        {
            lIndex += ((font[lIndex]) << 1) + 1;
        }
        
        /* draw character */
        ili9341_drawchar(x, y, lIndex, fg_color, bg_color);
        
        /* move the cursor forward for the next character */
        x += font[lIndex];
        
        /* next charachter */
        pS++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_setrotation(unsigned char r)
{
    ili9341_write_command(0x36); // MEMORY_ACC_CTRL
    
    switch(r)
    {
        case 0:
        {
            ili9341_write_date(0x40|0x08);
            LCD_W = 240;
            LCD_H = 320;
        break;
        }
        case 1:
        {
            ili9341_write_date(0x20|0x08);
            LCD_W = 320;
            LCD_H = 240;
        break;
        }
        case 2:
        {
            ili9341_write_date(0x80|0x08);
            LCD_W = 240;
            LCD_H = 320;
        break;
        }
        case 3:
        {
            ili9341_write_date(0x40|0x80|0x20|0x08);
            LCD_W = 320;
            LCD_H = 240;
        break;
        }
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_draw_filled_circle (uint16_t xm, uint16_t ym, uint8_t r, uint16_t color)
{
    int16_t f = 1 - r, ddF_x = 1, ddF_y = 0 - (2*r), x = 0, y = r;

    ili9341_drawvline(xm, ym-r, ym+r, color);
    
    while(x < y)
    {
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        ili9341_drawvline(xm+x, ym-y, ym+y, color);
        ili9341_drawvline(xm-x, ym-y, ym+y, color);
        ili9341_drawvline(xm+y, ym-x, ym+x, color);
        ili9341_drawvline(xm-y, ym-x, ym+x, color);
    }
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_draw_circle(int16_t xm, int16_t ym, int16_t r, uint16_t color)
{
    int16_t x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
   
    do 
    {
        ili9341_drawpixel(xm-x, ym+y, color); /*   I. Quadrant */
        ili9341_drawpixel(xm-y, ym-x, color); /*  II. Quadrant */
        ili9341_drawpixel(xm+x, ym-y, color); /* III. Quadrant */
        ili9341_drawpixel(xm+y, ym+x, color); /*  IV. Quadrant */
        
        r = err;
        
        if (r <= y)
        {
            y++;
            err += y*2+1;           /* e_xy+e_y < 0 */
        }
        if (r > x || err > y)
        {
            x++;
            err += x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
        }
    } while (x < 0);
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_setup_scrooll__area(int TFA,int BFA)
{
    ili9341_write_command(VT_SCROLL_DEF); // Definición de desplazamiento vertical
    ili9341_write_date(TFA >> 8);
    ili9341_write_date(TFA & 0x00FF);
    ili9341_write_date((320 -TFA-BFA)>>8);
    ili9341_write_date((320 -TFA-BFA)&0x00FF);
    ili9341_write_date(BFA >> 8);
    ili9341_write_date(BFA & 0x00FF);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void ili9341_scrollAddress(int VSP)
{
    ili9341_write_command(VT_SCROLL_START_ADD);
    ili9341_write_date(VSP >> 8);
    ili9341_write_date(VSP & 0x00FF);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

int  ili9341_scrollLine(void)
{
//    int yTemp = yStart; // Almacenar el antiguo yStart, aquí es donde dibujamos la siguiente línea
//	// llena el espacio restante
//	drawRectFilled ( 0 , yStart, ancho, TEXT_HEIGHT, NEGRO);
//	// Cambiar la parte superior del área de desplazamiento
//	yStart + = TEXT_HEIGHT;
//	// El valor debe ajustarse ya que la memoria de la pantalla es un búfer circular
//	if (yStart> = 320 - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA + (yStart - 320 + BOT_FIXED_AREA);
//	// Ahora podemos desplazarnos por la pantalla
//	scrollAddress (yStart);
//    
//	return   yTemp;
    return 0;
}