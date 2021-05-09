#ifndef TFT_H
#define TFT_H

#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

//Informacion
/*
 RS = "0" modo de comando; RS = "1" modo de caracter o escritura
 RST = es un reset por hardware 
 CS = selecciona el dispositivo para enviarle algo
 RD = lectura de señal 
 WR = escritura de señal
 D[7:0] = puerto paralelo por el cual se escribe la informacion 
 */

//#include <inttypes.h>
#include <stdbool.h>
#include "Fonts.h"

//Definciones de la dimension de la pantalla
#define LCD_WIDTH   240
#define LCD_HEIGHT  320

//Definiciones de pines a utilizar
#define RST_PIN         PORTBbits.RB1
#define RST_TRIS        TRISBbits.TRISB1
#define CS_PIN          PORTBbits.RB2
#define CS_TRIS         TRISBbits.TRISB2
#define RS_PIN          PORTBbits.RB3
#define RS_TRIS         TRISBbits.TRISB3
#define WR_PIN          PORTBbits.RB4
#define WR_TRIS         TRISBbits.TRISB4
#define RD_PIN          PORTBbits.RB5
#define RD_TRIS         TRISBbits.TRISB5

#define LCD_D0          PORTDbits.RD0
#define LCD_D1          PORTDbits.RD1
#define LCD_D2          PORTDbits.RD2
#define LCD_D3          PORTDbits.RD3
#define LCD_D4          PORTDbits.RD4
#define LCD_D5          PORTDbits.RD5
#define LCD_D6          PORTDbits.RD6
#define LCD_D7          PORTDbits.RD7

//Comandos para trabajar con el controlador ILI9341
#define SW_RESET                            0x01
#define R_D_INF                             0x04
#define R_D_STA                             0x09
#define R_D_POM                             0x0A
#define R_D_MADCTL                          0x0B
#define R_D_PIXEL_FORMT                     0x0C
#define R_D_IMAGE_FORMT                     0x0D
#define R_D_SIGNALM                         0x0E
#define R_D_SELFDG_RESL                     0x0F
#define SLEEP_IN                            0x10
#define SLEEP_OUT                           0x11
#define PART_MODE_ON                        0x12
#define NORM_D_MODE_ON                      0x13
#define D_INVERSION_OFF                     0x20
#define D_INVERSION_ON                      0x21
#define GAMMA_SET                           0x26
#define DISPLAY_OFF                         0x28
#define DISPLAY_ON                          0x29
#define COLUMN_ACC_SET                      0x2A
#define PAGE_ACC_SET                        0x2B
#define MEMORY_WRITE                        0x2C
#define COLOR_SET                           0x2D
#define MEMORY_READ                         0x2E
#define PARTIAL_AREA                        0x30
#define VT_SCROLL_DEF                       0x33
#define TE_LINE_OFF                         0x34
#define TE_LINE_ON                          0x35
#define MEMORY_ACC_CTRL                     0x36
#define VT_SCROLL_START_ADD                 0x37
#define IDLE_MODE_OFF                       0x38
#define IDLE_MODE_ON                        0x39
#define PIXEL_FORMT_SET                     0x3A
#define WRITE_MEMORY_CONT                   0x3C
#define READ_MEMORY_CONT                    0x3E
#define SET_TEAR_SCANL                      0x44
#define GET_SCANLINE                        0x45
#define WRITE_D_BRIGHTNESS                  0x51
#define REAd_D_BRIGHTNESS                   0x52
#define WRITE_CTRL_D                        0x53
#define READ_CTRL_D                         0x54
#define WRITE_CONT_ADAPT_BRIGHTNESS_CTRL    0x55
#define READ_CONT_ADAPT_BRIGHTNESS_CTRL     0x56
#define WRITE_CABC_MIN_BRIGHTNESS           0x5E
#define READ_CABC_MIN_BRIGHTNESS            0x5F
#define READ_ID1                            0xDA
#define READ_ID2                            0xDB
#define READ_ID3                            0xDC
#define RGB_SIGNAL_CTRL                     0xB0
#define FRAME_RATE_CTRL_NRM_FULLCOLORS      0xB1
#define FRAME_RATE_CTRL_IDMO_8COLORS        0xB2
#define FRAME_RATE_CTRL_PARTM_FULLCOLORS    0xB3
#define D_INVERSION_CTRL                    0xB4
#define BLANK_PORCH_CTRL                    0xB5
#define D_FUNCTION_CTRL                     0xB6
#define ENTRY_MODE_SET                      0xB7
#define BACKLIGHT_CTRL_1                    0xB8
#define BACKLIGHT_CTRL_2                    0xB9
#define BACKLIGHT_CTRL_3                    0xBA
#define BACKLIGHT_CTRL_4                    0xBB
#define BACKLIGHT_CTRL_5                    0xBC
#define BACKLIGHT_CTRL_7                    0xBE
#define BACKLIGHT_CTRL_8                    0xBF
#define POWER_CTRL_1                        0xC0
#define POWER_CTRL_2                        0xC1
#define VCOM_CTRL_1                         0xC5
#define VCOM_CTRL_2                         0xC7
#define NV_MEMORY_WRITE                     0xD0
#define NV_MEMORY_PROT_KEY                  0xD1
#define NV_MEMORY_STAT_READ                 0xD2
#define READ_ID4                            0xD4
#define POST_GAMMA_CORRECT                  0xE0
#define NEGT_GAMMA_CORRECT                  0xE1
#define DIGITAL_GAMMA_CTRL_1                0xE2
#define DIGITAL_GAMMA_CTRL_2                0xE3
#define INTERFACE_CTRL                      0xF6

//Definiciones de los valores para los colores
#define BLACK       0x0000      
#define NAVY        0x000F      
#define DARKGREEN   0x03E0      
#define DARKCYAN    0x03EF      
#define MAROON      0x7800      
#define PURPLE      0x780F      
#define OLIVE       0x7BE0      
#define LIGHTGREY   0xC618      
#define DARKGREY    0x7BEF      
#define BLUE        0x001F      
#define GREEN       0x07E0      
#define CYAN        0x07FF      
#define RED         0xF800     
#define MAGENTA     0xF81F      
#define YELLOW      0xFFE0      
#define WHITE       0xFFFF      
#define ORANGE      0xFD20      
#define GREENYELLOW 0xAFE5     
#define PINK        0xF81F

//Definiciones de funciones
#define TFT_init        ili9341_init
#define TFT_cmd         ili9341_write_command
#define TFT_read        ili9341_read
#define TFT_print       ili9341_write_date
#define TFT_text        ili9341_draw_string
#define TFT_fillscreen  ili9341_fill
#define TFT_xy          ili9341_setadress
#define TFT_fillrect    ili9341_fillrect
#define TFT_linev       ili9341_drawvline
#define TFT_lineh       ili9341_drawhline
#define TFT_lined       ili9341_drawdline
#define TFT_text_colour ili9341_txt_colour
#define TFT_bg_colour   ili9341_bg_colour
#define TFT_text_xy     ili9341_txt_xy
#define TFT_rotation    ili9341_setrotation
#define TFT_fill_circ   ili9341_draw_filled_circle  // Esta funcion no anda correctamente
#define TFT_circ        ili9341_draw_circle
#define TFT_rect        ili9341_drawrect
#define TFT_fill_rect   ili9341_fillrect
#define TFT_setupScroll_Area    ili9341_setup_scrooll__area

//Variables globales:
int txt_colour=0,bg_colour=0,txt_x=0,txt_y=0;

//Funciones

/**
 * Esta funcion permite inicializar el controlador
 */
void ili9341_init(void);
/**
 * Esta funcion permite escribir un comando con el bus de 8 bits
 * @param cmd
 */
void ili9341_write_command(unsigned char cmd);
/**
 * Esta funcion permite escribir un parametro
 * @param date
 */
void ili9341_write_date(unsigned char date);
/**
 * Esta funcion permite leer algun dato desde el controlador
 * @return 
 */
unsigned char ili9341_read(void);
/**
 * Esta funcion setea el controlador en una posicion
 * @param sc: columna de inicio
 * @param ec: columna de fin
 * @param sp: fila de inicio
 * @param ep: fila de fin
 */
void ili9341_setadress(int sc,int ec,int sp,int ep);
/**
 * Esta funcion permite setear un color en un pixel
 * @param colour
 */
void ili9341_pushcolor(int color);
/**
 * Esta funcion permite llenar la pantalla de un color
 * @param color
 */
void ili9341_fill(int color);
/**
 * Esta funcion permite llenar un rectangulo de un color
 * @param x: posicion x
 * @param y: posicion y
 * @param w: ancho
 * @param h: alto
 * @param colour
 */
void ili9341_fillrect(int x,int y,int w,int h,unsigned int color);
/**
 * Esta funcion permite realizar un rectangulo de un color
 * @param x
 * @param y
 * @param w
 * @param h
 * @param color
 * @param bg_color 
 */
void ili9341_drawrect(int x,int y,int w,int h,unsigned int color);
/**
 * Esta funcion permite dibujar el color de un pixel
 * @param x3
 * @param y3
 * @param color
 */
void ili9341_drawpixel(int x3,int y3,int color);
/**
 * Esta funcion permite dibujar una linea vertical 
 * @param x
 * @param y
 * @param h
 * @param color
 */
void ili9341_drawvline(int x,int y,int h,int color);
/**
 * Esta funcion permite dibujar una linea horizontal
 * @param x
 * @param y
 * @param w
 * @param color
 */
void ili9341_drawhline(int x,int y,int w,int color);
/**
 * Esta funcion permite dibujar una linea diagonal
 * @param x
 * @param y
 * @param d
 * @param dir: en '1' orientacion arriba; en '0' orientacion abajo
 * @param color
 */
void ili9341_drawdline(int x,int y,int d,_Bool dir,int color);
/**
 * Esta funcion permite dibujar un caracter con un color de fondo
 * @param x
 * @param y
 * @param findex
 * @param colour
 * @param bg_colour
 */
void ili9341_drawchar(int x,int y,int fIndex,int colour,int bg_colour);
//void ili9341_drawchar(int x,int y,char c,int color,int bg_color);
/**
 * Esta funcion permite setear el color del texto antes de escribirlo
 * @param colour
 */
void ili9341_txt_colour(int colour);
/**
 * Esta funcion permite ubicar el texto antes de escribirlo
 * @param x
 * @param y
 */
void ili9341_txt_xy(int x,int y);
/**
 * Esta funcion permite establecer el color de fondo del texto
 * @param colour
 */
void ili9341_bg_colour(int colour);
/**
 * Setea el cursor para ubicar la palabra o caracter
 * @param x
 * @param y
 */
void ili9341_setcursor(int x,int y);
/**
 * Esta funcion se encarga de escribir una cadena de texto en la pantalla
 * @param x
 * @param y
 * @param pS
 * @param fg_color
 * @param bg_color
 */
void ili9341_draw_string (uint16_t x, uint16_t y, const char *pS, uint16_t fg_color, uint16_t bg_color);
/**
 * Esta funcion permite rotar la pantalla
 * @param r: '0': 0°; '1': 90°; '2': 180°; '3': 270°
 */
void ili9341_setrotation(unsigned char r);
/**
 * Esta funcion permite dibujar un circulo relleno de un color
 * @param xm
 * @param ym
 * @param r
 * @param color
 */
void ili9341_draw_filled_circle (uint16_t xm, uint16_t ym, uint8_t r, uint16_t color);
/**
 * Esta funcion permite dibujar un circulo solo con borde
 * @param xm
 * @param ym
 * @param r
 * @param color
 */
void ili9341_draw_circle(int16_t xm, int16_t ym, int16_t r, uint16_t color);
/**
 * Esta funcion configura el area para el desplazamiento
 * @param TFA
 * @param BFA
 */
void ili9341_setup_scrooll__area(int TFA,int BFA);
/**
 * Esta funcion la direccion de inicio de desplazamiento
 * @param VSP
 */
void ili9341_scrollAddress(int VSP);
/**
 * Esta funcion desplaza una linea de texto
 * @return 
 */
int  ili9341_scrollLine(void);

#endif

