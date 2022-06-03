#include "estc_timer.h"


APP_TIMER_DEF(m_second_timer_id);
APP_TIMER_DEF(m_first_timer_id);  

static uint32_t first_timeout   = 0;
static uint32_t second_timeout  = 0;


void timer_init(app_timer_timeout_handler_t first_char_timeout_handler, app_timer_timeout_handler_t second_char_timeout_handler, uint32_t first_timer, uint32_t second_timer)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    second_timeout = second_timer;
    first_timeout = first_timer;
    app_timer_create(&m_first_timer_id, APP_TIMER_MODE_REPEATED, first_char_timeout_handler);
    app_timer_create(&m_second_timer_id, APP_TIMER_MODE_REPEATED, second_char_timeout_handler);
    
}

void timer_start_first()
{
    app_timer_start(m_first_timer_id, first_timeout, NULL);
}

void timer_start_second()
{
    app_timer_start(m_second_timer_id, second_timeout, NULL);
}

void timer_stop_first()
{
    app_timer_stop(m_first_timer_id);         
}

void timer_stop_second()
{
    app_timer_stop(m_second_timer_id);
}