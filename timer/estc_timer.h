#include "app_timer.h"



void timer_init(app_timer_timeout_handler_t notifying_char_timeout_handler, app_timer_timeout_handler_t indicating_char_timeout_handler, uint32_t time_down, uint32_t time_up);

void timer_start_first();
void timer_start_second();

void timer_stop_first();
void timer_stop_second();