#ifndef BUTTON_MODULE_H
#define BUTTON_MODULE_H

#include "nrfx_gpiote.h"
#include "pca10059.h"
#include <stdbool.h>
#include "nrfx_rtc.h"

#define DEVICE_BUTTON_DELAY_MIN 1000
#define DEVICE_BUTTON_DELAY_MAX 15000
#define GET_DEFAULT_BUTTON_CTX(id)      \
{                                       \
    .is_first_click = false,            \
    .is_long_press = false,             \
    .is_button_press = false,           \
    .rtc_timer = NRFX_RTC_INSTANCE(id), \
    .prev_button_time = 0,              \
    .double_button_evt_handler = NULL   \
}

typedef struct 
{
    bool is_first_click;
    bool is_long_press;
    bool is_button_press;
    nrfx_rtc_t rtc_timer;
    uint32_t prev_button_time;
    nrfx_gpiote_evt_handler_t double_button_evt_handler ;
}button_module_ctx_t;


void init_gpiote_button(nrfx_gpiote_evt_handler_t double_button_evt_handler);
bool is_long_press();
#endif