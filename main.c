#include <stdbool.h>
#include <stdint.h>
#include "gpio_module/gpio_module.h"
#include "nrfx_rtc.h"
#include "pwm_module/pwm_module.h"
#include "button_module/button_module.h"
#include "util_module/utils.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define DEVICE_ID_LIST \
    {                  \
        6, 5, 7, 9     \
    }
#define DEVICE_COUNT_LED 4U
#define DEVICE_DELAY_CICLE 100U
#define DEVICE_DISCRETE_DELAY 1U
#define DEVICE_DELAY_BLINK 500
#define DEVICE_TIME 10U

static uint16_t const m_max_time = 20000;
static uint16_t m_pwm_step = 200;
static volatile uint8_t m_phase = 0;
//static const uint8_t        m_ids[DEVICE_COUNT_LED] = DEVICE_ID_LIST;
static volatile uint8_t m_repeat = 0;
//static uint16_t             m_prev_value            = 0;
static modificator_t m_state = NONE;

void init_log(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_INFO("Starting up the test project with USB logging");
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    m_state = m_state == V_MOD ? NONE : m_state + 1;
    m_phase = 0;
    switch (m_state)
    {
    case V_MOD:
    {
        m_pwm_step = 0;
        set_value_of_channel(m_phase >> 1, m_max_time);
        break;
    }
    case S_MOD:
    {
        m_pwm_step = m_max_time / 4;
        break;
    }
    case H_MOD:
    {
        m_pwm_step = 300;
        break;
    }
    default:
        set_value_of_channel(m_phase >> 1, 0);
        break;
    }
}

static void pwn_custom_handler(nrfx_pwm_evt_type_t event_type)
{
    if (!m_state)
    {
        return;
    }
    if (event_type == NRFX_PWM_EVT_FINISHED)
    {
        uint8_t channel = m_phase >> 1;
        bool down = m_phase & 1;
        bool next_phase = false;
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
            m_phase = down ? m_phase - 1 : m_phase + 1;
        }
    }
}

int main(void)
{
    init_log();
    init_leds();
    init_button();
    init_pwn_module_for_leds(pwn_custom_handler, m_max_time);
    init_gpiote_button(button_pressed_handler);

    uint32_t t = 0;
    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
        if(m_state && is_long_press())
        {
            NRF_LOG_INFO("HEY %d", t++);
        }
        // __WFE();
        // __SEV();
        // __WFE();
        
    }
}