/*
 * spi.h
 *
 *  Created on: Nov 21, 2017
 *      Author: alex
 */
#include "msp.h"

#ifndef SPI_H_
#define SPI_H_


void configure_clocks(void);
void configure_SPI(void);

void Delay(uint32_t count);

uint8_t timer_trigger;
uint8_t timer_delay;
uint8_t timer_count_fast;
uint16_t timer_count;

#endif /* SPI_H_ */
