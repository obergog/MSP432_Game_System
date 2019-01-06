/*
 * lcd.h
 *
 *  Created on: Nov 28, 2017
 *      Author: alex
 */
#include "msp.h"

#ifndef LCD_H_
#define LCD_H_

//macros for screen orientation
#define LCD_MAX_X           127
#define LCD_MAX_Y           127
#define LCD_TOTAL_PIXEL     16384
#define LCD_CHAR_HEIGHT     10
#define LCD_CHAR_WIDTH      7

/*command macros to be sent to the LCD*/
#define SLPOUT              0x11    //Sleep Out
#define GAMSET              0x26    //Gamma Set
#define FRMCTR1             0xB1    //Frame Rate Control 1
#define PWCTR1              0xC0    //Power Control 1
#define COLMOD              0x3A    //Interface Pixel Format
#define MADCTL              0x36    //Memory Data Access Control
#define NORON               0x13    //Normal Display Mode On
#define CASET               0x2A    //Column Address Set
#define RASET               0x2B    //Row Address Set
#define RAMWR               0x2C    //Memory Write
#define DISPON              0x29    //Display On

/*16-bit color values*/
#define BLACK               0x0000
#define WHITE               0xFFFF
#define RED                 0xF800
#define BLUE                0x001F
#define GREEN               0x07E0
#define YELLOW              (RED + GREEN)
#define CYAN                (BLUE + GREEN)
#define MAGENTA             (RED + BLUE)
#define DARKGREEN           0x03E0
#define MAROON              0x8800
#define NAVY                0x000F
#define OLIVE               (MAROON + DARKGREEN)
#define PURPLE              (MAROON + NAVY)
#define TEAL                (NAVY + DARKGREEN)

extern const uint16_t color_array[14];

/*structure that defines the parameters for a rectangle*/
typedef struct rectangle{
    uint8_t x, y;
    uint8_t height, width;
    uint16_t color;
}RECT;
//structure that defines the parameters for a circle
typedef struct circle{
    uint8_t x, y;
    uint8_t radius;
    uint16_t trace_color, fill_color;
}CIRCLE;
/*function that initializes rectangle based on given parameters*/
void initialize_rectangle(RECT * rect, uint8_t x0, uint8_t y0, uint8_t w0, uint8_t h0, uint16_t color_val);
void initialize_circle(CIRCLE * circ, uint8_t x0, uint8_t y0, uint8_t r0, uint16_t trace_color_val, uint16_t fill_color_val);

/*functions that manipulate the LCD*/
void initialize_LCD(void);
void LCD_send_command(uint8_t command);
void LCD_send_data(uint8_t data);

void LCD_erase_screen(void);

//basic LCD write commands
void LCD_draw_frame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_draw_pixel(int16_t x, int16_t y, uint16_t color);

void LCD_draw_hline(uint8_t y0, uint16_t color);
void LCD_draw_vline(uint8_t y0, uint8_t y1, uint8_t x0, uint16_t color);

//functions that deal with rectangles and the LCD
void LCD_draw_rectangle(RECT rect);
void LCD_erase_rectangle(RECT rect);

//function that draws a circle on the LCD
void LCD_draw_circle(CIRCLE circ);
void LCD_draw_circle_octants(uint8_t x_center, uint8_t y_center, uint8_t x, uint8_t y, uint16_t color);

//functions to write strings to LCD
void LCD_write_character(const uint8_t * array, uint8_t x0, uint8_t y0, uint16_t color);
void LCD_write_string(uint8_t * string, uint8_t x0, uint8_t y0, uint16_t color, uint8_t length);

//functions to convert an integer to string
void reverse(char s[]);
void itoa(int n, char s[]);

/*defines the arrays that are used to write characters to the LCD*/
//numbers
extern const uint8_t char_lib_0[70];
extern const uint8_t char_lib_1[70];
extern const uint8_t char_lib_2[70];
extern const uint8_t char_lib_3[70];
extern const uint8_t char_lib_4[70];
extern const uint8_t char_lib_5[70];
extern const uint8_t char_lib_6[70];
extern const uint8_t char_lib_7[70];
extern const uint8_t char_lib_8[70];
extern const uint8_t char_lib_9[70];
extern const uint8_t * char_lib_num[10];
//capital letters
extern const uint8_t char_lib_A[70];
extern const uint8_t char_lib_B[70];
extern const uint8_t char_lib_C[70];
extern const uint8_t char_lib_D[70];
extern const uint8_t char_lib_E[70];
extern const uint8_t char_lib_F[70];
extern const uint8_t char_lib_G[70];
extern const uint8_t char_lib_H[70];
extern const uint8_t char_lib_I[70];
extern const uint8_t char_lib_J[70];
extern const uint8_t char_lib_K[70];
extern const uint8_t char_lib_L[70];
extern const uint8_t char_lib_M[70];
extern const uint8_t char_lib_N[70];
extern const uint8_t char_lib_O[70];
extern const uint8_t char_lib_P[70];
extern const uint8_t char_lib_Q[70];
extern const uint8_t char_lib_R[70];
extern const uint8_t char_lib_S[70];
extern const uint8_t char_lib_T[70];
extern const uint8_t char_lib_U[70];
extern const uint8_t char_lib_V[70];
extern const uint8_t char_lib_W[70];
extern const uint8_t char_lib_X[70];
extern const uint8_t char_lib_Y[70];
extern const uint8_t char_lib_Z[70];
//punctuation
extern const uint8_t char_lib_exc[70];
extern const uint8_t char_lib_que[70];
extern const uint8_t char_lib_colon[70];

extern const uint8_t * char_lib_ascii[128];






#endif /* LCD_H_ */
