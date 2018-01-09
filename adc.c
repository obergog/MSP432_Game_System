/*
 * adc.c
 *
 *  Created on: Nov 20, 2017
 *      Author: alex
 */

#include "msp.h"
#include "core_cm4.h"
#include "adc.h"

//#define TIMERA

void configure_ADC(void){
    /*enable ports*/

    //Pin 4.4, joystick y
    P4->SEL0 &= ~BIT4;
    P4->SEL1 &= ~BIT4;
    P4->DIR &= ~BIT4;
    P4->REN &= ~BIT4;
    P4->OUT |= BIT4;
    P4->IFG &= ~BIT4;
    P4->IES |= BIT4;
    P4->IE &= ~BIT4;

    //Pin 6.0 joystick x
    P6->SEL0 &= ~BIT0;
    P6->SEL1 &= ~BIT0;
    P6->DIR &= ~BIT0;
    P6->REN &= ~BIT0;
    P6->OUT |= BIT0;
    P6->IFG &= ~BIT0;
    P6->IES |= BIT0;
    P6->IE &= ~BIT0;

    /*configure adc*/
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;                  //disable conversions

    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);        //wait while generator is busy
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;//enable internal 1.2V ref

    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_3 | ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP;
    ADC14->CTL1 |= ADC14_CTL1_CH2MAP | ADC14_CTL1_RES__14BIT;

    //y direction joystick A9
    ADC14->MCTL[1] |= ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_INCH_9 | ADC14_CTL1_CSTARTADD_MASK;
    ADC14->IER0 |= ADC14_IER0_IE1;                               //enable interrupts
    //x direction joystick A15
    ADC14->MCTL[2] |= ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_INCH_15 | ADC14_CTL1_CSTARTADD_MASK;
    ADC14->MCTL[2] |= ADC14_MCTLN_EOS;                           //make this the end of sequence
    ADC14->IER0 |= ADC14_IER0_IE2;                               //enable interrupts

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY));      //wait for generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC;                  //enable conversions
    NVIC_EnableIRQ(ADC14_IRQn);
}


void ADC14_IRQHandler(void){
    __disable_irq();
    if(ADC14->IFGR0 & ADC14_IFGR0_IFG1){
        bit_joy_y = ADC14->MEM[1];

    }
    if(ADC14->IFGR0 & ADC14_IFGR0_IFG2){
        bit_joy_x = ADC14->MEM[2];

    }
    __enable_irq();
}



