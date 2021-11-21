#ifndef BUTTON_MODULE_H
#define BUTTON_MODULE_H

#include "nrfx_gpiote.h"
#include "pca10059.h"
#include <stdbool.h>

#define DEVICE_BUTTON_DELAY_MIN 1000
#define DEVICE_BUTTON_DELAY_MAX 15000

void init_gpiote_button(nrfx_gpiote_evt_handler_t evt_handler);
bool is_long_press();
#endif