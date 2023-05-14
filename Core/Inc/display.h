/*
 * display.h
 *
 *  Created on: 2023. máj. 13.
 *      Author: plasz
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include <stdint.h>
#include "spi.h"
#include <stdbool.h>
#include "compontents.h"
extern volatile uint8_t asciiDigits[2];

void GetDigits(uint8_t number);
uint8_t convertTo7Segment(uint8_t value);
void DisplayValue(int32_t value);
void EnableDisplays(bool enable);

#endif /* SRC_DISPLAY_H_ */
