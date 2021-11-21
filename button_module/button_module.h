#ifndef BUTTON_MODULE_H
#define BUTTON_MODULE_H
#include "nrfx_gpiote.h"
#include "pca10059.h"

void init_gpiote_button(nrfx_gpiote_evt_handler_t evt_handler);
#endif