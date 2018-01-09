/*
 * game.h
 *
 *  Created on: Dec 7, 2017
 *      Author: alex
 */
#include "msp.h"
#include <stdint.h>
#include "lcd.h"
#include "adc.h"
#include "serial.h"

#ifndef GAME_H_
#define GAME_H_

uint8_t button_flag;
void initialize_buttons(void);

typedef enum {
    DEBUG,
    PONG,
    DODGE
}GAME;

typedef enum {
    DEBUG_MODE,
    STANDARD_MODE
}GAME_MODE;

extern const GAME game_array[3];
GAME current_game;

GAME_MODE pong_mode;

RECT start_screen;
void draw_start_screen(void);

void run_game(GAME game);

/*function to enter debug app*/
void full_debug(void);

uint8_t bit_joy_x_string[4];
uint8_t bit_joy_y_string[4];

/*functions and variables for pong*/
void initialize_pong(void);
void pong_max_score(void);
void pong_paddle_color(void);
void pong_main(void);
void end_pong(void);

RECT ball;
RECT right;  //right paddle
RECT left;  //left paddle

int8_t yvelocity;
int8_t xvelocity;

uint8_t score_1;
uint8_t score_2;

uint8_t max_score;

uint8_t at_left;
uint8_t at_right;

uint16_t ball_color;
uint16_t left_paddle_color;
uint16_t right_paddle_color;

uint8_t field_bottom;

/*functions and vairables for dodge*/
void run_dodge(void);

typedef enum {
    DEAD,
    ALIVE
}DODGE_STATE;

uint8_t timer_count_string[5];
#endif /* GAME_H_ */
