#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define DEVICE_ID_LIST {6, 5, 7, 9}
#define DEVICE_COUNT_LED 4U
#define DEVICE_DELAY_CICLE 100U
#define DEVICE_DISCRETE_DELAY 1U
#define DEVICE_DELAY_BLINK 500
#define DEVICE_TIME 10U
#define DEVICE_BUTTON_DELAY_MIN 1000
#define DEVICE_BUTTON_DELAY_MAX 15000

static volatile bool double_click = false;
static volatile bool freeze = false;
//static nrfx_systick_state_t time_state;

static nrfx_rtc_t rtc_timer =  NRFX_RTC_INSTANCE(0);
static uint32_t prev_time = 0;


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
    invert_led(index_led);
    nrfx_systick_delay_us(DEVICE_TIME * (time1));
    invert_led(index_led);
    nrfx_systick_delay_us(DEVICE_TIME * time2);
 }
 
void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(!double_click)
    {
        prev_time = nrfx_rtc_counter_get(&rtc_timer);
        double_click = true;
        return;
    }
    uint32_t t = nrfx_rtc_counter_get(&rtc_timer) - prev_time;
    if(t > DEVICE_BUTTON_DELAY_MIN && t < DEVICE_BUTTON_DELAY_MAX)
    {
        nrf_gpio_cfg_output(leds[i]);
        nrf_gpio_pin_write(leds[i], 1);
    }
    double_click = false;
}

 inline void make_blink(const uint8_t pin, const uint32_t time)
 {
    nrf_gpio_pin_write(pin, 0);
    nrf_delay_ms(time);
    nrf_gpio_pin_write(pin, 1);
    nrf_delay_ms(time);
 }

void rtc_handler(nrfx_rtc_int_type_t int_type)
{

}

 void init_rtc(void)
 {
    nrfx_rtc_config_t conf = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_rtc_init(&rtc_timer, &conf, rtc_handler);
    nrfx_rtc_enable(&rtc_timer);
 }

int main(void)
{
    uint8_t blink_array[DEVICE_COUNT_LED] = DEVICE_ID_LIST;
    uint8_t index_led = 0, repeat = 0, light = 0;
    uint32_t time_cicle = 0, time_current = 0;

    nrfx_systick_init();
    init_rtc();
    init_log();

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        if(!nrf_gpio_pin_read(DEVICE_BUTTON))
        {
            if(blink_array[index_led] <= repeat)
            {
                index_led = index_led + 1 < DEVICE_COUNT_LED ? index_led + 1 : 0;
                repeat = 0;
            }   
            make_blink(leds[index_led], DEVICE_BLINK);
            ++repeat;

            NRF_LOG_INFO("LEDS #%d is blinking\n", index_led);
            NRF_LOG_PROCESS();
        }
    }   
}

