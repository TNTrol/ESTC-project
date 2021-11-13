#include "gpio_module.h"

static uint8_t leds[LEDS_NUMBER] = LEDS_LIST;
static uint8_t buttons[BUTTONS_NUMBER] = BUTTONS_LIST;

void init_leds(void)
{
    for(int i = 0; i < LEDS_NUMBER; ++i)
    {
        nrf_gpio_cfg_output(leds[i]);
        nrf_gpio_pin_write(leds[i], 1);
    }
}

void write_led(uint8_t index, uint8_t value)
{
    nrf_gpio_pin_write(leds[index], value);
}

void invert_led(uint8_t index)
{
    nrf_gpio_pin_toggle(leds[index]);
}

void on_led(uint8_t index)
{
    nrf_gpio_pin_write(leds[index], 0);
}

void off_led(uint8_t index)
{
    nrf_gpio_pin_write(leds[index], 1);
}

void off_leds(void)
{
    for(int i = 0; i < LEDS_NUMBER; ++i)
    {
        nrf_gpio_pin_write(leds[i], 1);
    }
}

#if (BUTTONS_NUMBER != 0)
void init_button(void)
{
    for(int i = 0; i < BUTTONS_NUMBER; ++i)
    {
        nrf_gpio_cfg_input(buttons[i], NRF_GPIO_PIN_PULLUP);
    }
}

uint8_t get_value_button(void)
{
    return nrf_gpio_pin_read(buttons[0]);
}
#endif