#include "app_timer.h"



void timer_init(app_timer_timeout_handler_t first_char_timeout_handler, app_timer_timeout_handler_t second_char_timeout_handler, uint32_t first_timer, uint32_t second_timer);

void timer_start_first();
void timer_start_second();

void timer_stop_first();
void timer_stop_second();