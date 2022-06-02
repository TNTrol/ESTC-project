#include "app_timer.h"

#define APP_INDICATION_TIMER_TIMEOUT    500
#define APP_NOTIFICATION_TIMER_TIMEOUT  300

void timer_init(app_timer_timeout_handler_t notifying_char_timeout_handler, app_timer_timeout_handler_t indicating_char_timeout_handler);

void timer_start_notification();
void timer_start_indification();

void timer_stop_notification();
void timer_stop_indification();