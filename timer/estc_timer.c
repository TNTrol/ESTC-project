#include "estc_timer.h"


APP_TIMER_DEF(m_indication_timer_id);                                   /**< Indicationing characteristic timer id */
APP_TIMER_DEF(m_notification_timer_id);  

static uint32_t s_timeout = 0;
static uint32_t e_timeout = 0;


void timer_init(app_timer_timeout_handler_t notifying_char_timeout_handler, app_timer_timeout_handler_t indicating_char_timeout_handler, uint32_t time_down, uint32_t time_up)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    e_timeout = time_up;
    s_timeout = time_down;
    app_timer_create(&m_notification_timer_id, APP_TIMER_MODE_REPEATED, notifying_char_timeout_handler);
    app_timer_create(&m_indication_timer_id, APP_TIMER_MODE_REPEATED, indicating_char_timeout_handler);
    
}

void timer_start_first()
{
    app_timer_start(m_notification_timer_id, s_timeout, NULL);
}

void timer_start_second()
{
    app_timer_start(m_indication_timer_id, e_timeout, NULL);
}

void timer_stop_first()
{
    app_timer_stop(m_notification_timer_id);         
}

void timer_stop_second()
{
    app_timer_stop(m_indication_timer_id);
}