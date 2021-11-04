
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
#define DEVICE_LEDS {BSP_LED_0, BSP_LED_1, BSP_LED_2, BSP_LED_3}
#define DEVICE_SIZE 4
#define DEVICE_TIME 500
#define MY_LED BSP_LED_0

/**
 * @brief Function for application main entry.
 */

 void init_data(int (*array)[2])
 {
    
    int temp = 0, number = DEVICE_ID;
    static int arr[DEVICE_SIZE] = DEVICE_LEDS;
    for(int i = DEVICE_SIZE - 1; i >= 0 ; --i)
    {
        temp = number % 10;
        number /= 10;
        array[i][1] = temp;
        array[i][0] = arr[i];
        nrf_gpio_cfg_output(arr[i]);
        nrf_gpio_pin_write(arr[i], 1);
    }
 }

 void make_blink(const int id_led, const int count)
 {
     
    for (int j = 0; j < count; ++j)
    {
        nrf_gpio_pin_write(id_led, 0);
        nrf_delay_ms(DEVICE_TIME);
        nrf_gpio_pin_write(id_led, 1);
        nrf_delay_ms(DEVICE_TIME);
    }
 }

int main(void)
{
    
    int blink_array[DEVICE_SIZE][2] = {0};
    init_data(blink_array);
    while (true)
    {
        for(int i = 0; i < DEVICE_SIZE; ++i)
        {
            make_blink(blink_array[i][0], blink_array[i][1]);
        }
    }
}

/**
 *@}
 **/
