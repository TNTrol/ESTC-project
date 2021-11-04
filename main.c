
/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "nrf_gpio.h"

#define DEVICE_ID 6579
#define DEVICE_LEDS {NRF_GPIO_PIN_MAP(0, 6), NRF_GPIO_PIN_MAP(0, 8), NRF_GPIO_PIN_MAP(1, 9), NRF_GPIO_PIN_MAP(0, 12)}
#define DEVICE_SIZE 4
#define DEVICE_TIME 500

/**
 * @brief Function for application main entry.
 */

 struct pin_blink_t
 {
     uint8_t pin;
     uint8_t repeat;
 };

 typedef struct pin_blink_t PinBlink;

 void init_data(PinBlink *array)
 {
    
    int temp = 0, number = DEVICE_ID;
    static const int arr[DEVICE_SIZE] = DEVICE_LEDS;
    for(int i = DEVICE_SIZE - 1; i >= 0 ; --i)
    {
        temp = number % 10;
        number /= 10;
        array[i].repeat = temp;
        array[i].pin = arr[i];
        nrf_gpio_cfg_output(arr[i]);
        nrf_gpio_pin_write(arr[i], 1);
    }
 }

 void make_blink(const int pin, const int count)
 {
    for (int j = 0; j < count; ++j)
    {
        nrf_gpio_pin_write(pin, 0);
        nrf_delay_ms(DEVICE_TIME);
        nrf_gpio_pin_write(pin, 1);
        nrf_delay_ms(DEVICE_TIME);
    }
 }

int main(void)
{
    
    PinBlink blink_array[DEVICE_SIZE] = {0};
    init_data(blink_array);
    while (true)
    {
        for(int i = 0; i < DEVICE_SIZE; ++i)
        {
            make_blink(blink_array[i].pin, blink_array[i].repeat);
        }
    }
}

/**
 *@}
 **/
