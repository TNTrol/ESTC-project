
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
#include "nrf_gpio.h"
#include "gpio_module/gpio_module.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define DEVICE_ID 6579
#define DEVICE_COUNT_LED 4
#define DEVICE_DELAY 500
#define DEVICE_DISCRETE_DELAY 1

/**
 * @brief Function for application main entry.
 */

 void init_data(const uint32_t size, uint32_t id, uint8_t *array)
 {
    uint32_t temp = 0;
    for(int i = size - 1; i >= 0 ; --i)
    {
        temp = id % 10;
        id /= 10;
        array[i] = temp;
    }
 }

 void init_log(void)
 {
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_INFO("Starting up the test project with USB logging");
    NRF_LOG_DEFAULT_BACKENDS_INIT();
 }

int main(void)
{
    uint8_t blink_array[DEVICE_COUNT_LED] = {0};
    uint8_t index_led = 0, repeat = 0;
    uint32_t time = 0;
    uint8_t light = 0;
    
    init_data(DEVICE_COUNT_LED, DEVICE_ID, blink_array);
    init_leds();
    init_button();
    init_log();

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
        if(!get_value_button())
        {
            if(time++ < DEVICE_DELAY)  
            {
                if(blink_array[index_led] <= repeat)
                {
                    index_led = index_led + 1 < DEVICE_COUNT_LED ? index_led + 1 : 0;
                    repeat = 0;
                }   
                write_led(index_led,  light);
            }
            else
            {
                time = 0;
                light = ~light;
                repeat = light ? repeat : repeat + 1;
                if(!light)
                {
                    NRF_LOG_INFO("LEDS #%d is blinking\n", index_led);
                }
            }
        }
        else
        {
            write_led(index_led, 1);
        }
        nrf_delay_ms(DEVICE_DISCRETE_DELAY);
    }   
}

/**
 *@}
 **/
