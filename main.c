#include <stdbool.h>
#include <stdint.h>
#include "gpio_module/gpio_module.h"
#include "nrfx_rtc.h"
#include "pwm_module/pwm_module.h"
#include "button_module/button_module.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define DEVICE_ID_LIST {6, 5, 7, 9}
#define DEVICE_COUNT_LED 4U
#define DEVICE_DELAY_CICLE 100U
#define DEVICE_DISCRETE_DELAY 1U
#define DEVICE_DELAY_BLINK 500
#define DEVICE_TIME 10U
#define DEVICE_BUTTON_DELAY_MIN 1000
#define DEVICE_BUTTON_DELAY_MAX 15000

static volatile bool        m_double_click          = false;
static volatile bool        m_freeze                = false;
static nrfx_rtc_t           m_rtc_timer             = NRFX_RTC_INSTANCE(0);
static uint32_t             m_prev_button_time      = 0;
static uint16_t const       m_max_time              = 20000;
static uint16_t const       m_pwm_step              = 200;
static volatile uint8_t     m_phase                 = 0;
static const uint8_t        m_ids[DEVICE_COUNT_LED] = DEVICE_ID_LIST;
static volatile uint8_t     m_repeat                = 0;
static uint16_t             m_prev_value            = 0;

 void init_log(void)
 {
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_INFO("Starting up the test project with USB logging");
    NRF_LOG_DEFAULT_BACKENDS_INIT();
 }
 
void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(m_double_click)
    {
        uint32_t t = nrfx_rtc_counter_get(&m_rtc_timer) - m_prev_button_time;
        if(t > DEVICE_BUTTON_DELAY_MIN && t < DEVICE_BUTTON_DELAY_MAX)
        {
            m_freeze = !m_freeze;
            m_double_click = false;
            uint16_t temp = get_value_of_channel(m_phase >> 1);
            set_value_of_channel(m_phase >> 1, m_prev_value);
            m_prev_value = temp;
            return;
        }
    }
    m_prev_button_time = nrfx_rtc_counter_get(&m_rtc_timer);
    m_double_click = true;
}

void rtc_handler(nrfx_rtc_int_type_t int_type)
{}

 void init_rtc(void)
 {
    nrfx_rtc_config_t conf = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_rtc_init(&m_rtc_timer, &conf, rtc_handler);
    nrfx_rtc_enable(&m_rtc_timer);
 }
 static void pwn_custom_handler(nrfx_pwm_evt_type_t event_type)
{
    if(!m_freeze)
    {
        return;
    }
    if (event_type == NRFX_PWM_EVT_FINISHED)
    {
        uint8_t channel    = m_phase >> 1;
        bool    down       = m_phase & 1;
        bool    next_phase = false;
        uint16_t value = get_value_of_channel(channel);
        if (down)
        {
            value -= m_pwm_step;
            next_phase = value == 0;
        }
        else
        {
            value += m_pwm_step;
            next_phase = value >= m_max_time;
        }
        set_value_of_channel(channel, value); 
        if (next_phase)
        {
            if(++m_repeat >= m_ids[channel] * 2)
            { 
                m_repeat = 0;
                ++m_phase;
            }
            else
            {
                m_phase = down ? m_phase - 1 : m_phase + 1;
            }
            if (m_phase >= 2 * NRF_PWM_CHANNEL_COUNT)
            {
                m_phase = 0;
            }
        }
    }
}

int main(void)
{
    //nrfx_systick_init();
    init_rtc();
    init_log();
    init_leds();
    init_button();
    init_pwn_module_for_leds(pwn_custom_handler, m_max_time);
    init_gpiote_button(button_pressed_handler);

    while (true)
    {
        __WFE();
        __SEV();
        __WFE();
    }   
}