#ifndef GPIO_MODULE_H
#define GPIO_MODULE_H

#include "pca10059.h"

void init_leds(void);
void write_led(uint8_t index, uint8_t value);
void invert_led(uint8_t index);
void on_led(uint8_t index);
void off_led(uint8_t index);
void off_leds(void);

#if (BUTTONS_NUMBER != 0)
void init_button(void);
uint8_t get_value_button(void);
#endif

#endif