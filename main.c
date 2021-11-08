
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

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

//#include "app_usbd.h"
//#include "app_usbd_serial_num.h"

#define DEVICE_ID 6579
#define DEVICE_LED1 NRF_GPIO_PIN_MAP(0, 6)
#define DEVICE_LED2 NRF_GPIO_PIN_MAP(0, 8)
#define DEVICE_LED3 NRF_GPIO_PIN_MAP(1, 9)
#define DEVICE_LED4 NRF_GPIO_PIN_MAP(0, 12)
#define DEVICE_LEDS {DEVICE_LED1, DEVICE_LED2, DEVICE_LED3, DEVICE_LED4}
#define DEVICE_COUNT_LED 4
#define DEVICE_DELAY 500
#define DEVICE_DISCRETE_DELAY 1
#define DEVICE_BUTTON NRF_GPIO_PIN_MAP(1, 6)

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

 void init_leds(const uint8_t *leds, const uint32_t size)
 {
    for(int i = 0; i < size; ++i)
    {
        nrf_gpio_cfg_output(leds[i]);
        nrf_gpio_pin_write(leds[i], 1);
    }
 }

 void init_button(uint8_t button)
 {
     nrf_gpio_cfg_input(button, NRF_GPIO_PIN_PULLUP);
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
    const uint8_t leds[DEVICE_COUNT_LED] = DEVICE_LEDS;
    uint8_t index_led = 0, repeat = 0;
    uint32_t time = 0;
    uint8_t light = 0;
    
    init_data(DEVICE_COUNT_LED, DEVICE_ID, blink_array);
    init_leds(leds, DEVICE_COUNT_LED);
    init_button(DEVICE_BUTTON);
    init_log();

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        if(!nrf_gpio_pin_read(DEVICE_BUTTON))
        {
            if(time++ < DEVICE_DELAY)  
            {
                if(blink_array[index_led] <= repeat)
                {
                    index_led = index_led + 1 < DEVICE_COUNT_LED ? index_led + 1 : 0;
                    repeat = 0;
                }   
                nrf_gpio_pin_write(leds[index_led], light);
            }
            else
            {
                time = 0;
                light = ~light;
                repeat = light ? repeat : repeat + 1;
                if(!light)
                {
                    NRF_LOG_INFO("LEDS #%d is blinking\n", index_led);
                    NRF_LOG_PROCESS();
                }
            }
        }
        else
        {
            nrf_gpio_pin_write(leds[index_led], 1);
        }
        nrf_delay_ms(DEVICE_DISCRETE_DELAY);
    }   
}

/**
 *@}
 **/
