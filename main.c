#include <stdbool.h>
#include <stdint.h>
#include "gpio_module/gpio_module.h"
#include "pwm_module/pwm_module.h"
#include "button_module/button_module.h"
#include "util_module/utils.h"
#include "color_module/color_module.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define CONTROL_MASK 1
#define RGB_MASK ((1 << 1) | (1 << 2) | (1 << 3))
#define MAX_TIME_PWM_CICLE 20000

static uint16_t         m_pwm_step  = 200;
static volatile uint8_t m_phase     = 0;
static modificator_t    m_state     = NONE;
static rgb_t            m_rgb_color = {0, 0, 0};
static pwm_ctx_t        m_pwm       = GET_DEFAULT_CTX(0);
static pwm_ctx_t        m_pwm_rgb   = GET_DEFAULT_CTX(1);

void init_log(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_INFO("Starting up the test project with USB logging");
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void double_button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    m_state = m_state == V_MOD ? NONE : m_state + 1;
    m_phase = 0;
    switch (m_state)
    {
    case V_MOD:
    {
        m_pwm_step = 0;
        set_value_of_channel(&m_pwm ,m_phase >> 1, MAX_TIME_PWM_CICLE);
        break;
    }
    case S_MOD:
    {
        m_pwm_step = MAX_TIME_PWM_CICLE / 4;
        break;
    }
    case H_MOD:
    {
        m_pwm_step = 300;
        break;
    }
    default:
        set_value_of_channel(&m_pwm, m_phase >> 1, 0);
        break;
    }
}

static void pwn_control_led_handler(nrfx_pwm_evt_type_t event_type)
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
        uint16_t value = get_value_of_channel(&m_pwm, channel);
        if (down)
        {
            value -= m_pwm_step;
            next_phase = value == 0;
        }
        else
        {
            value += m_pwm_step;
            next_phase = value >= MAX_TIME_PWM_CICLE;
        }

        set_value_of_channel(&m_pwm, channel, value);
        if (next_phase)
        {
            m_phase = down ? m_phase - 1 : m_phase + 1;
        }
    }
}

void rgb_on()
{
    uint32_t step = MAX_TIME_PWM_CICLE / 255;
    uint32_t r = m_rgb_color.r * step, g = m_rgb_color.g * step, b = m_rgb_color.b * step;
    set_value_of_channel(&m_pwm_rgb, 1, r);
    set_value_of_channel(&m_pwm_rgb, 2, g);
    set_value_of_channel(&m_pwm_rgb, 3, b);
}

int main(void)
{
    hsv_t color = {0, 0, 0};
    uint16_t h = 0, s = 0, v = 0;
    init_log();
    init_leds();
    init_button();
    init_pwn_module_for_leds(&m_pwm, pwn_control_led_handler, MAX_TIME_PWM_CICLE, CONTROL_MASK);
    init_pwn_module_for_leds(&m_pwm_rgb, NULL, MAX_TIME_PWM_CICLE, RGB_MASK);
    init_gpiote_button(double_button_handler);

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
        if(m_state && is_long_press())
        {
            NRF_LOG_INFO("State %d Color r=%d, g=%d, b=%d", m_state, m_rgb_color.r, m_rgb_color.g, m_rgb_color.b);
            switch (m_state)
            {
            case H_MOD:
                h = h == 1023 ? 0 : h + 1;
                color.h = h / 4;
                hsv_to_rgb(&color, &m_rgb_color);
                rgb_on();
                break;
            case S_MOD:
                s = s == 1023 ? 0 : s + 1;
                color.s = s / 4;
                hsv_to_rgb(&color, &m_rgb_color);
                rgb_on();
                break;
            default:
                v = v == 1023 ? 0 : v + 1;
                color.v = v / 4;
                hsv_to_rgb(&color, &m_rgb_color);
                rgb_on();
                break;
            }
            nrf_delay_ms(1);
        }
    }
}