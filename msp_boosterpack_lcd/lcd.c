/*
 * lcd.c
 *
 *  Created on: Nov 28, 2017
 *      Author: alex
 */

#include <msp_boosterpack_lcd/serial.h>
#include "msp.h"
#include "lcd.h"
#include <string.h>

//send signals to LCD to initialize it
void initialize_LCD(void){
    //toggle LCD reset pin
    P5->OUT &= ~BIT7;
    Delay(50*48);
    P5->OUT |= BIT7;
    Delay(120*48);

    //Exit Sleep
    LCD_send_command(SLPOUT);
    Delay(200*48);
    //Set Gamma
    LCD_send_command(GAMSET);
    LCD_send_data(0x04);
    //Set Frame Rate
    LCD_send_command(FRMCTR1);
    LCD_send_data(0x0A);
    LCD_send_data(0x14);
    //Set Power to Appropriate Voltage
    LCD_send_command(PWCTR1);
    LCD_send_data(0x0A);
    LCD_send_data(0x00);
    //Set LCD to 16-bit/pixel format
    LCD_send_command(COLMOD);
    LCD_send_data(0x05);
    Delay(10*48);
    //Configure scanning direction of frame memory
    LCD_send_command(MADCTL);
    LCD_send_data(0x08);
    //Normal Display Mode on
    LCD_send_command(NORON);
    //Draw frame for entire LCD and make it all white
    LCD_draw_frame(0, 0, 127, 127);
    LCD_send_command(RAMWR);
    uint32_t i;
    for (i = 0; i < 16384; i++){
        LCD_send_data(0xFF);
        LCD_send_data(0xFF);
    }
    Delay(10*48);
    //Turn Display on
    LCD_send_command(DISPON);
}

//sends the given command to the LCD
void LCD_send_command(uint8_t command){
    //set DC low to enter command mode
    P3->OUT &= ~BIT7;

    //wait while busy
    while (UCB0STATW_SPI & UCBUSY);
    //transmit command
    UCB0TXBUF = command;

    //set DC high to re-enter data mode
    P3->OUT |= BIT7;
}

//sends raw data to the LCD
void LCD_send_data(uint8_t data){
    //wait while busy
    while (UCB0STATW_SPI & UCBUSY);
    //transmit data
    UCB0TXBUF = data;
}

//sends information to the LCD to initialize the size of the frame
void LCD_draw_frame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    //change the values to correspond with an upright orientation
    x0 += 2;
    y0 += 3;
    x1 += 2;
    y1 += 3;

    //initialize column information
    LCD_send_command(CASET);
    LCD_send_data((uint8_t)(x0 >> 8));
    LCD_send_data((uint8_t)(x0));
    LCD_send_data((uint8_t)(x1 >> 8));
    LCD_send_data((uint8_t)(x1));
    //initialize row information
    LCD_send_command(RASET);
    LCD_send_data((uint8_t)(y0 >> 8));
    LCD_send_data((uint8_t)(y0));
    LCD_send_data((uint8_t)(y1 >> 8));
    LCD_send_data((uint8_t)(y1));
}

void LCD_erase_screen(void){
    LCD_draw_frame(0, 0, 127, 127);
    LCD_send_command(RAMWR);
    uint32_t i;
    for (i = 0; i < 16384; i++){
        LCD_send_data(0xFF);
        LCD_send_data(0xFF);
    }
}

//draws a pixel on the screen at coordinates x,y with given color
void LCD_draw_pixel(int16_t x, int16_t y, uint16_t color){
    //initialize the frame using given data
    LCD_draw_frame(x,y,x,y);

    /*write the pixel to the screen
      since color is 16 bits, send it with two iterations*/
    LCD_send_command(RAMWR);
    LCD_send_data(color >> 8);
    LCD_send_data(color);
}

/*draws a horizontal line across the screen at given y value with given color*/
void LCD_draw_hline(uint8_t y0, uint16_t color){
    for (uint8_t i = 0; i < 128; i++){
        LCD_draw_pixel(i,y0,color);
    }
}
/*initialize the given rectangle with given values*/
void initialize_rectangle(RECT * rect, uint8_t x0, uint8_t y0, uint8_t w0, uint8_t h0, uint16_t color_val){
    rect->x = x0;
    rect->y = y0;
    rect->width = w0;
    rect->height = h0;
    rect->color = color_val;
}

/*draws the given rectangle on the LCD*/
void LCD_draw_rectangle(RECT rect){
    uint8_t x0 = rect.x;
    uint8_t x1 = rect.x + rect.width;
    uint8_t y0 = rect.y;
    uint8_t y1 = rect.y + rect.height;
    uint16_t color_val = rect.color;

    //draw the frame that represents the rectangle
    LCD_draw_frame(x0,y0,x1,y1);

    /*find the total number of pixels in the rectangle and fill them all
     * with the given color*/
    uint16_t i;
    uint16_t pixels = (rect.width + 1) * (rect.height + 1);
    LCD_send_command(RAMWR);
    for(i = 0; i <= pixels; i++){
        LCD_send_data(color_val >> 8);
        LCD_send_data(color_val);
    }
}

/*erases the given rectangle from the LCD*/
void LCD_erase_rectangle(RECT rect){
    uint8_t x0 = rect.x;
    uint8_t x1 = rect.x + rect.width;
    uint8_t y0 = rect.y;
    uint8_t y1 = rect.y + rect.height;

    //draw the frame that represents the rectangle
    LCD_draw_frame(x0,y0,x1,y1);

    /*find the total number of pixels in the rectangle and fill them all
     * with white*/
    uint16_t i;
    uint16_t pixels = (rect.width + 1) * (rect.height + 1);
    LCD_send_command(RAMWR);
    for(i = 0; i <= pixels; i++){
        LCD_send_data(0xFF);
        LCD_send_data(0xFF);
    }
}

//Takes a 70 entry array of ones and zeros and fills a corresponding 7x10 frame
void LCD_write_character(const uint8_t * array, uint8_t x0, uint8_t y0, uint16_t color){
    uint8_t x1 = x0 + 6;
    uint8_t y1 = y0 + 9;

    //draw frame
    LCD_draw_frame(x0,y0,x1,y1);

    //fill representative array
    uint16_t i;
    LCD_send_command(RAMWR);
    for(i = 0; i < 70; i++){
        if(array[i]){
            LCD_send_data(color >> 8);
            LCD_send_data(color);
        }
        else{
            LCD_send_data(0xFF);
            LCD_send_data(0xFF);
        }
    }
}
/*write the given string to the LCD with given length at (x0,y0) with given color*/
void LCD_write_string(uint8_t * string, uint8_t x0, uint8_t y0, uint16_t color, uint8_t length){
    int8_t i;
    for(i = (length - 1); i >= 0; i--){
        LCD_write_character(char_lib_ascii[string[i]],x0+(LCD_CHAR_WIDTH*(length - 2)) - (LCD_CHAR_WIDTH*i),y0,color);
    }
}

/*the following reverse and itoa functions were found at:
  https://en.wikibooks.org/wiki/C_Programming/stdlib.h/itoa */
/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert integer n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
      n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

const uint16_t color_array[14] = {BLACK,
                                  WHITE,
                                  RED,
                                  BLUE,
                                  GREEN,
                                  YELLOW,
                                  CYAN,
                                  MAGENTA,
                                  DARKGREEN,
                                  MAROON,
                                  NAVY,
                                  OLIVE,
                                  PURPLE,
                                  TEAL};
