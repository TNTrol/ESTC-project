#include "button_module.h"
#include "../timer/estc_timer.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"


static button_module_ctx_t m_btn_ctx = GET_DEFAULT_BUTTON_CTX(3);
static bool m_btn_range = false;
static bool m_long = false;

static void start_timer_handler()
{
    m_btn_range = true;
    timer_stop_first();
}

static void stop_timer_handler()
{
    m_btn_range = false;
    timer_stop_second();
    m_long = m_btn_ctx.is_long_press;
}

// static void init_rtc(void)
// {
//     nrfx_rtc_config_t conf = NRFX_RTC_DEFAULT_CONFIG;
//     nrfx_rtc_init(&m_btn_ctx.rtc_timer, &conf, rtc_handler);
//     nrfx_rtc_enable(&m_btn_ctx.rtc_timer);
// }




static void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    m_btn_ctx.is_button_press ^= true;
    if (m_btn_ctx.is_button_press)
    {
        if (m_btn_ctx.is_first_click)
        {
            //nrfx_rtc_counter_get(&m_btn_ctx.rtc_timer) - m_btn_ctx.prev_button_time;
            if (m_btn_range)
            {
                m_btn_ctx.is_first_click = false;
                m_btn_ctx.double_button_evt_handler(pin, action);
                return;
            }
        }
        m_btn_ctx.prev_button_time = 0;//nrfx_rtc_counter_get(&m_btn_ctx.rtc_timer);
        m_btn_ctx.is_first_click = true;
        m_btn_ctx.is_long_press = true;
        m_long = false;
        timer_stop_first();
        timer_stop_second();
        timer_start_first();
        timer_start_second();
    }
    else
    {
        m_btn_ctx.is_long_press = false;
        m_long = false;
    }
}

void init_gpiote_button(nrfx_gpiote_evt_handler_t double_button_evt_handler)
{
    nrfx_gpiote_init();
    const nrfx_gpiote_in_config_t btn_gpiote_cfg = {
        .sense              = NRF_GPIOTE_POLARITY_TOGGLE,
        .pull               = NRF_GPIO_PIN_PULLUP,
        .is_watcher         = false,
        .hi_accuracy        = false,
        .skip_gpio_setup    = true};
    m_btn_ctx.double_button_evt_handler = double_button_evt_handler;
    nrfx_gpiote_in_init(BUTTON_1, &btn_gpiote_cfg, button_pressed_handler);
    nrfx_gpiote_in_event_enable(BUTTON_1, true);
    timer_init(start_timer_handler, stop_timer_handler, DEVICE_BUTTON_DELAY_MIN, DEVICE_BUTTON_DELAY_MAX);
    // init_rtc();
}

bool is_long_press()
{
    return m_btn_ctx.is_long_press && m_long;
    // return m_btn_ctx.is_long_press && nrfx_rtc_counter_get(&m_btn_ctx.rtc_timer) - m_btn_ctx.prev_button_time > DEVICE_BUTTON_DELAY_MAX;
}