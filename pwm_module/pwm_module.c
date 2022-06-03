#include "pwm_module.h"
#include "pca10059.h"
#include "app_util_platform.h"

#define UNMASK(s, n) (((s) >> (n)) & 1 ? NRFX_PWM_PIN_INVERTED : NRFX_PWM_PIN_NOT_USED)

static void init_seq(pwm_ctx_t* ctx)
{
    ctx->seq_values.channel_0       = 0;
    ctx->seq_values.channel_1       = 0;
    ctx->seq_values.channel_2       = 0;
    ctx->seq_values.channel_3       = 0;

    ctx->seq.values.p_individual    = &ctx->seq_values;
    ctx->seq.length                 = NRF_PWM_VALUES_LENGTH(ctx->seq_values);
    ctx->seq.repeats                = 0;
    ctx->seq.end_delay              = 0;
}

void init_pwn_module_for_leds(pwm_ctx_t* ctx, nrfx_pwm_handler_t  handler, uint32_t max_time, uint8_t mask)
{
    nrfx_pwm_config_t const config =
    {
        .output_pins =
        {
            LED_1 | UNMASK(mask, 0),
            LED_2 | UNMASK(mask, 1),
            LED_3 | UNMASK(mask, 2),
            LED_4 | UNMASK(mask, 3)
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = max_time,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    nrfx_pwm_init(&ctx->pwm, &config, handler);
    init_seq(ctx);
    (void)nrfx_pwm_simple_playback(&ctx->pwm, &ctx->seq, 1, NRFX_PWM_FLAG_LOOP);
}

uint16_t get_value_of_channel(pwm_ctx_t* ctx, uint8_t index)
{
    return ((uint16_t *)&(ctx->seq_values))[index];
}

void set_value_of_channel(pwm_ctx_t* ctx, uint8_t index, uint16_t value)
{
    ((uint16_t *)&(ctx->seq_values))[index] = value;
}