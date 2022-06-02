#include "estc_timer.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

APP_TIMER_DEF(m_indication_timer_id);                                   /**< Indicationing characteristic timer id */
APP_TIMER_DEF(m_notification_timer_id);  


void timer_init(app_timer_timeout_handler_t notifying_char_timeout_handler, app_timer_timeout_handler_t indicating_char_timeout_handler)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    app_timer_create(&m_notification_timer_id, APP_TIMER_MODE_REPEATED, notifying_char_timeout_handler);
    app_timer_create(&m_indication_timer_id, APP_TIMER_MODE_REPEATED, indicating_char_timeout_handler);
    
}

void timer_start_notification()
{
    app_timer_start(m_notification_timer_id, APP_NOTIFICATION_TIMER_TIMEOUT, NULL);
    NRF_LOG_INFO("Start notification");
}

void timer_start_indification()
{
    app_timer_start(m_indication_timer_id, APP_INDICATION_TIMER_TIMEOUT, NULL);
    NRF_LOG_INFO("Start indication");
}

void timer_stop_notification()
{
    app_timer_stop(m_notification_timer_id);    
    NRF_LOG_INFO("Stop notification");      
}

void timer_stop_indification()
{
    app_timer_stop(m_indication_timer_id);
    NRF_LOG_INFO("Stop indication");
}