/*
 * button.h
 *
 *  Created on: Sep 25, 2023
 *      Author: HaHuyen
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "spi.h"
#include "gpio.h"

// Button definitions
#define BTN_MODE    0   // Button mode (View -> Edit -> Alarm -> Update)
#define BTN_UP      1   // Value increment button
#define BTN_ENTER   12  // Enter/Next parameter button
#define BTN_UPDATE  3   // Direct entry to Update mode

#define N0_OF_BUTTONS 16

extern uint16_t button_count[N0_OF_BUTTONS];

void button_init();
void button_Scan();

#endif /* INC_BUTTON_H_ */

