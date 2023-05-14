/*
 * components.c
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */
#include "compontents.h"
void enterCriticalSection() {
    __disable_irq();  // Disable interrupts globally
}

void exitCriticalSection() {
    __enable_irq();  // Enable interrupts globally
}
