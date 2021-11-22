#ifndef PWN_MODUL_H
#define PWN_MODUL_H

#include "nrfx_pwm.h"

#define GET_DEFAULT_CTX(id) {.pwm = NRFX_PWM_INSTANCE(id)}

typedef struct 
{
    nrf_pwm_values_individual_t seq_values;
    nrfx_pwm_t pwm;
    nrf_pwm_sequence_t seq;
}pwm_ctx_t;

void init_pwn_module_for_leds(pwm_ctx_t* ctx, nrfx_pwm_handler_t  handler, uint32_t max_time, uint8_t mask);
uint16_t get_value_of_channel(pwm_ctx_t* ctx, uint8_t index);
void set_value_of_channel(pwm_ctx_t* ctx, uint8_t index, uint16_t value);

#endif