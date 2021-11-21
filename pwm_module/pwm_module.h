#ifndef PWN_MODUL_H
#define PWN_MODUL_H

#include "nrfx_pwm.h"

void init_pwn_module_for_leds(nrfx_pwm_handler_t  handler, uint32_t max_time);
uint16_t get_value_of_channel(uint8_t index);
void set_value_of_channel(uint8_t index, uint16_t value);

#endif