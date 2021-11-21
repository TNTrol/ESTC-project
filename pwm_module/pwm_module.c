#include "pwm_module.h"
#include "pca10059.h"
#include "app_util_platform.h"

static nrf_pwm_values_individual_t m_demo1_seq_values;
static nrfx_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(0);
static nrf_pwm_sequence_t const    m_demo1_seq =
{
    .values.p_individual = &m_demo1_seq_values,
    .length              = NRF_PWM_VALUES_LENGTH(m_demo1_seq_values),
    .repeats             = 0,
    .end_delay           = 0
};

void init_pwn_module_for_leds(nrfx_pwm_handler_t  handler, uint32_t max_time)
{
    nrfx_pwm_config_t const config0 =
    {
        .output_pins =
        {
            LED_1 | NRFX_PWM_PIN_INVERTED, // channel 0
            LED_2 | NRFX_PWM_PIN_NOT_USED, // channel 1
            LED_3 | NRFX_PWM_PIN_NOT_USED, // channel 2
            LED_4 | NRFX_PWM_PIN_NOT_USED // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = max_time,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    nrfx_pwm_init(&m_pwm0, &config0, handler);
    m_demo1_seq_values.channel_0 = 0;
    m_demo1_seq_values.channel_1 = 0;
    m_demo1_seq_values.channel_2 = 0;
    m_demo1_seq_values.channel_3 = 0;
    (void)nrfx_pwm_simple_playback(&m_pwm0, &m_demo1_seq, 1,
                                      NRFX_PWM_FLAG_LOOP);
}

uint16_t get_value_of_channel(uint8_t index)
{
    return ((uint16_t *)&m_demo1_seq_values)[index];
}

void set_value_of_channel(uint8_t index, uint16_t value)
{
    ((uint16_t *)&m_demo1_seq_values)[index] = value;
}