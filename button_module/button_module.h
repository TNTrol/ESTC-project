#ifndef BUTTON_MODULE_H
#define BUTTON_MODULE_H

#include "nrfx_gpiote.h"
#include "pca10059.h"
#include <stdbool.h>
#include "nrfx_rtc.h"


#define DEVICE_BUTTON_DELAY_MIN 1000
#define DEVICE_BUTTON_DELAY_MAX 7500
#define GET_DEFAULT_BUTTON_CTX          \
{                                       \
    .is_first_click = false,            \
    .is_long_press = false,             \
    .is_button_press = false,           \
    .is_button_range = false,           \
    .is_most_long = false,              \
    .prev_button_time = 0,              \
    .double_button_evt_handler = NULL   \
}

typedef struct 
{
    nrfx_gpiote_evt_handler_t double_button_evt_handler;
    uint32_t prev_button_time;
    bool is_first_click;
    bool is_long_press;
    bool is_button_press;
    bool is_button_range;
    bool is_most_long;
}button_module_ctx_t;


void init_gpiote_button(nrfx_gpiote_evt_handler_t double_button_evt_handler);
bool is_long_press();
#endif