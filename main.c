#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_systick.h"
#include "nrf_delay.h"
#include "gpio_module/gpio_module.h"


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
<<<<<<< HEAD
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
=======

>>>>>>> af02e6933d79b68cd5595455e49c2bc6819fa3ed

}

 void init_rtc(void)
 {
    nrfx_rtc_config_t conf = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_rtc_init(&rtc_timer, &conf, rtc_handler);
    nrfx_rtc_enable(&rtc_timer);
 }

 void make_blink(uint8_t index_led, uint32_t time1, uint32_t time2)
 {
    invert_led(index_led);
    nrfx_systick_delay_us(DEVICE_TIME * (time1));
    invert_led(index_led);
    nrfx_systick_delay_us(DEVICE_TIME * time2);
 }

int main(void)
{
<<<<<<< HEAD
=======
    
>>>>>>> af02e6933d79b68cd5595455e49c2bc6819fa3ed
    uint8_t blink_array[DEVICE_COUNT_LED] = DEVICE_ID_LIST;
    uint8_t index_led = 0, repeat = 0, light = 0;
    uint32_t time_cicle = 0, time_current = 0;

    nrfx_systick_init();
<<<<<<< HEAD
    init_rtc();
=======
>>>>>>> af02e6933d79b68cd5595455e49c2bc6819fa3ed
    init_log();
    init_leds();
    init_button();

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
        if(!get_value_button())
        {
            if(DEVICE_DELAY_CICLE > time_cicle)  
            {
                if(!(++time_current % (DEVICE_DELAY_BLINK / DEVICE_DELAY_CICLE)))
                {
                    time_cicle++;
                }
                if(blink_array[index_led] <= repeat)
                {
                    index_led = index_led + 1 < DEVICE_COUNT_LED ? index_led + 1 : 0;
                    time_current = 0;
                    time_cicle = 0;
                    repeat = 0;
                }
                if(light)
                {
                    make_blink(index_led, DEVICE_DELAY_CICLE - time_cicle, time_cicle);
                }
                else
                {    
                    make_blink(index_led, time_cicle, DEVICE_DELAY_CICLE - time_cicle);    
                }
            }
            else
            {
                time_cicle = 0;
                time_current = 0;
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
            off_led(index_led);
        }
        nrf_delay_ms(DEVICE_DISCRETE_DELAY);
    }   
<<<<<<< HEAD
}

=======
}
>>>>>>> af02e6933d79b68cd5595455e49c2bc6819fa3ed
