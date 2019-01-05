/*
 * spi.c
 *
 *  Created on: Nov 21, 2017
 *      Author: alex
 */

#include <msp_boosterpack_lcd/serial.h>
#include "msp.h"

void configure_clocks(void){
    //set the core voltage higher to support higher clock frequency
    PCM->CTL0 |= PCM_CTL0_AMR__AM_LDO_VCORE1;
    FLCTL->BANK0_RDCTL |= FLCTL_BANK0_RDCTL_WAIT2;
    FLCTL->BANK1_RDCTL |= FLCTL_BANK1_RDCTL_WAIT2;

    //set the SMLK clock to 48MHz
    CS->KEY = 0x695A;
    CS->CTL0 = 0;
    CS->CTL0 |= CS_CTL0_DCORSEL_5;   //set frequency range to 32 to 64 MHz
    CS->CTL1 |= CS_CTL1_DIVS_0;      //clock divider for SMCLK is 1 (48MHz)
    CS->CTL1 |= CS_CTL1_DIVHS__2;      //clock divider for HSMCLK is 4 (12MHz)
    CS->KEY = 0;

    //Configure Timer 0
    TIMER_A0->R = 0;         // Reset count
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_ID_3 | TIMER_A_CTL_IE;
    TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_7;       //divide the clock again
    TIMER_A0->CCR[0] = 15000;    // Value to count to (50 times a second)
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;   // TACCR0 interrupt enabled

    /* Enable Interrupts in the NVIC */
    NVIC_EnableIRQ(TA0_0_IRQn);

    timer_trigger = 0;
    timer_delay = 0;
    timer_count_fast = 0;
    timer_count = 0;
}
/*configure EUSCI_B0 and other GPIO pins to work with SPI*/
void configure_SPI(void){
    //configure the 1.5 pin to be CLK and 1.6 to be MOSI (primary  module function)
    P1->SEL0 |= BIT5 | BIT6;
    P1->SEL1 &= ~BIT5 & ~BIT6;

    //configure 5.0(CS) 5.7(LCD RESET) and 3.7(DC) 2.5(backlight) as output pins
    P5->SEL0 &= ~BIT0 & ~BIT7;
    P5->SEL1 &= ~BIT0 & ~BIT7;
    P5->DIR |= BIT0 | BIT7;
    P5->OUT &= ~BIT0 & ~BIT7;

    //3.6 for new PCB
    P3->SEL0 &= ~BIT7;
    P3->SEL1 &= ~BIT7;
    P3->DIR |= BIT7;
    P3->OUT &= ~BIT7;

    P2->SEL0 &= ~BIT5;
    P2->SEL1 &= ~BIT5;
    P2->DIR |= BIT5;
    P2->OUT |= BIT5;

    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_SWRST;         //enable software reset

    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_MODE_0;        //3 wire SPI mode
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;   //use SMCLK
    EUSCI_B0_SPI->CTLW0 &= ~EUSCI_B_CTLW0_SEVENBIT;     //8 bit characters
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_MSB;           //MSB first
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_CKPH;          //clock phase select
    EUSCI_B0_SPI->CTLW0 &= ~EUSCI_B_CTLW0_CKPL;         //clock polarity low
    EUSCI_B0_SPI->CTLW0 |= EUSCI_B_CTLW0_MST;           //enable as master
    EUSCI_B0_SPI->BRW = 3;                              //divide clock by 3 to get 16MHz

    EUSCI_B0_SPI->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;        //disable software reset

    //set CS low and DC high
    P5->OUT &= ~BIT0;
    P3->OUT |= BIT7;
}

//delays for the given number of clock cycles
void Delay(uint32_t count)
{
    uint32_t i = 0;
    for (i = 0; i < count; i++);
}

void TA0_0_IRQHandler(void){
    if(TIMER_A0->CCTL[0] & BIT0){
        timer_trigger = 1;
        if(timer_delay > 0){
            timer_delay--;
        }
        if(timer_count_fast >= 5){
            timer_count++;
            timer_count_fast = 0;
        }
        else {
            timer_count_fast++;
        }
        TIMER_A0->CCTL[0] &= ~BIT0;
        TIMER_A0->R = 0;
    }
}
