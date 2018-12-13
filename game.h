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

//macros for dodge game
#define DODGE_BOX_HEIGHT        10
#define DODGE_BOX_WIDTH         10
#define DODGE_PLAYER_START_X    60
#define DODGE_PLAYER_START_Y    60
#define DODGE_OBS_1_START_X     20
#define DODGE_OBS_1_START_Y     20
#define DODGE_OBS_2_START_X     20
#define DODGE_OBS_2_START_Y     90
#define DODGE_OBS_3_START_X     90
#define DODGE_OBS_3_START_Y     20
#define DODGE_OBS_4_START_X     90
#define DODGE_OBS_4_START_Y     90

#define DODGE_OBS_2_DELAY       15
#define DODGE_OBS_3_DELAY       30
#define DODGE_OBS_4_DELAY       45

//enums used menu navigation
typedef enum {
    DEBUG,
    PONG,
    DODGE
}GAME;

typedef enum {
    DEBUG_MODE,
    STANDARD_MODE
}GAME_MODE;

//enums used in dodge game
typedef enum {
    DEAD,
    ALIVE
}DODGE_STATE;

//variable for reading buttons globally
uint8_t button_flag;

//variables for navigating menu
extern const GAME game_array[3];
GAME current_game;
GAME_MODE pong_mode;
RECT start_screen;

//variables used when in debug mode
uint8_t bit_joy_x_string[4];
uint8_t bit_joy_y_string[4];

//variables for pong game
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

//variables for dodge
uint8_t timer_count_string[5];

//function to initialize button interrupts
void initialize_buttons(void);
//function to draw the start screen
void draw_start_screen(void);
//function to run game given
void run_game(GAME game);
//function to enter debug app
void full_debug(void);

/*functions for pong*/
void initialize_pong(void);
void pong_max_score(void);
void pong_paddle_color(void);
void pong_main(void);
void end_pong(void);

/*functions for dodge*/
void run_dodge(void);

#endif /* GAME_H_ */
