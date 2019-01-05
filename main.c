/*****************************************************************************
*
* Copyright (C) 2013 - 2016 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the
*   distribution.
*
* * Neither the name of Texas Instruments Incorporated nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
* MSP432 empty main.c template
*
* for GNU compiler
*
******************************************************************************/

#include "msp_boosterpack_lcd/serial.h"
#include "msp.h"
#include "core_cm4.h"
#include "adc.h"
#include "msp_boosterpack_lcd/lcd.h"
#include "game.h"

#define ENABLE_SLEEPONEXIT (0x00000002)

int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                    /* Stop watchdog timer */
	
    configure_ADC();
    configure_clocks();
    configure_SPI();
    initialize_LCD();
    initialize_buttons();
    __enable_irq();

    SCB->SCR &= ~ENABLE_SLEEPONEXIT;            //wake up on exit from ISR


    while(1){
        while(timer_delay);
        /*draw the start screen and wait for user input*/
        draw_start_screen();
        while(timer_delay);

        select_game();

        run_game(game_array[current_game]);
    }

    return 0;
}
