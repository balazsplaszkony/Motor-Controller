/*
 * display.c
 *
 *  Created on: 2023. máj. 13.
 *      Author: plasz
 */


#include "display.h"

volatile uint8_t Digits[2];

void GetDigits(uint8_t number)
{
	enterCriticalSection();
	Digits[0] = convertTo7Segment((number / 10));
    Digits[1] = convertTo7Segment((number % 10));
    exitCriticalSection();
}

uint8_t convertTo7Segment(uint8_t value) {
    switch (value) {
        case 0:
            value = ~0x3F;  // Inverted value
            break;
        case 1:
            value = ~0x06;  // Inverted value
            break;
        case 2:
            value = ~0x5B;  // Inverted value
            break;
        case 3:
            value = ~0x4F;  // Inverted value
            break;
        case 4:
            value = ~0x66;  // Inverted value
            break;
        case 5:
            value = ~0x6D;  // Inverted value
            break;
        case 6:
            value = ~0x7D;  // Inverted value
            break;
        case 7:
            value = ~0x07;  // Inverted value
            break;
        case 8:
            value = ~0x7F;  // Inverted value
            break;
        case 9:
            value = ~0x6F;  // Inverted value
            break;

        default:
            // Handle unsupported values or return a default value
            value = 0xFF;  // Blank segment (all segments turned off)
            break;
    }
    return value;
}

void DisplayValue(int32_t value)
{
	if(value < 0)
		value = -value;
	GetDigits(value);
	SPI_EnableChip(true);
	SPI_SEND(Digits, 2);
	SPI_EnableChip(false);
	EnableDisplays(true);
}

void EnableDisplays(bool enable)
{
	HAL_GPIO_WritePin(EN_SEG_GPIO_Port, EN_SEG_Pin, !enable);
}
