#include "button_module.h"
#include "nrfx_rtc.h"

static volatile bool                m_is_first_click            = false;
static nrfx_rtc_t                   m_rtc_timer                 = NRFX_RTC_INSTANCE(0);
static uint32_t                     m_prev_button_time          = 0;
static nrfx_gpiote_evt_handler_t    m_double_button_evt_handler = NULL;
static volatile bool                m_is_long_press             = false;
static volatile bool                m_is_button_press           = false;

static void rtc_handler(nrfx_rtc_int_type_t int_type)
{
}

static void init_rtc(void)
{
    nrfx_rtc_config_t conf = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_rtc_init(&m_rtc_timer, &conf, rtc_handler);
    nrfx_rtc_enable(&m_rtc_timer);
}

static void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    m_is_button_press ^= true;
    if (m_is_button_press)
    {
        if (m_is_first_click)
        {
            uint32_t t = nrfx_rtc_counter_get(&m_rtc_timer) - m_prev_button_time;
            if (t > DEVICE_BUTTON_DELAY_MIN && t < DEVICE_BUTTON_DELAY_MAX)
            {
                m_is_first_click = false;
                m_double_button_evt_handler(pin, action);
                return;
            }
        }
        m_prev_button_time = nrfx_rtc_counter_get(&m_rtc_timer);
        m_is_first_click = true;
        m_is_long_press = true;
    }
    else
    {
        m_is_long_press = false;
    }
}

void init_gpiote_button(nrfx_gpiote_evt_handler_t double_button_evt_handler)
{
    nrfx_gpiote_init();
    const nrfx_gpiote_in_config_t btn_gpiote_cfg = {
        .sense = NRF_GPIOTE_POLARITY_TOGGLE,
        .pull = NRF_GPIO_PIN_PULLUP,
        .is_watcher = false,
        .hi_accuracy = false,
        .skip_gpio_setup = true};
    m_double_button_evt_handler = double_button_evt_handler;
    nrfx_gpiote_in_init(BUTTON_1, &btn_gpiote_cfg, button_pressed_handler);
    nrfx_gpiote_in_event_enable(BUTTON_1, true);
    init_rtc();
}

bool is_long_press()
{
    return m_is_long_press && nrfx_rtc_counter_get(&m_rtc_timer) - m_prev_button_time > DEVICE_BUTTON_DELAY_MAX;
}