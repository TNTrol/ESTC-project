#include "button_module.h"
#include "../timer/estc_timer.h"

static button_module_ctx_t m_btn_ctx = GET_DEFAULT_BUTTON_CTX;

static void start_timer_handler()
{
    m_btn_ctx.is_button_range = true;
    timer_stop_first();
}

static void stop_timer_handler()
{
    m_btn_ctx.is_button_range = false;
    timer_stop_second();
    m_btn_ctx.is_most_long = m_btn_ctx.is_long_press;
}

static void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    m_btn_ctx.is_button_press ^= true;
    if (m_btn_ctx.is_button_press)
    {
        if (m_btn_ctx.is_first_click)
        {
            if (m_btn_ctx.is_button_range)
            {
                m_btn_ctx.is_first_click = false;
                m_btn_ctx.double_button_evt_handler(pin, action);
                return;
            }
        }
        m_btn_ctx.prev_button_time = 0;//nrfx_rtc_counter_get(&m_btn_ctx.rtc_timer);
        m_btn_ctx.is_first_click = true;
        m_btn_ctx.is_long_press = true;
        m_btn_ctx.is_most_long = false;
        timer_stop_first();
        timer_stop_second();
        timer_start_first();
        timer_start_second();
    }
    else
    {
        m_btn_ctx.is_long_press = false;
        m_btn_ctx.is_most_long = false;
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
}

bool is_long_press()
{
    return m_btn_ctx.is_long_press && m_btn_ctx.is_most_long;
}