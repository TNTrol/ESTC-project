#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "gpio_module/gpio_module.h"
#include "nrfx_systick.h"
#include "nrfx_gpiote.h"


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
#define DEVICE_BUTTON_DELAY_MIN 25000
#define DEVICE_BUTTON_DELAY_MAX 100000

static volatile bool double_click = false;
static volatile bool freeze = false;
static nrfx_systick_state_t time_state;

 void init_log(void)
 {
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_INFO("Starting up the test project with USB logging");
    NRF_LOG_DEFAULT_BACKENDS_INIT();
 }

 void make_blink(uint8_t index_led, uint32_t time1, uint32_t time2)
 {
    invert_led(index_led);
    nrfx_systick_delay_us(DEVICE_TIME * (time1));
    invert_led(index_led);
    nrfx_systick_delay_us(DEVICE_TIME * time2);
 }
 
void button_pressed_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(double_click)
    {
        nrfx_systick_get(&time_state);
        double_click = true;
        return;
    }
    else if(nrfx_systick_test(&time_state, DEVICE_BUTTON_DELAY_MIN) && !nrfx_systick_test(&time_state, DEVICE_BUTTON_DELAY_MAX))
    {
        freeze = !freeze;
    }
    double_click = false;
    
}

 void init_gpiote(void)
 {
    nrfx_gpiote_init();
    const nrfx_gpiote_in_config_t btn_gpiote_cfg = {
        .sense = NRF_GPIOTE_POLARITY_HITOLO,
        .pull = NRF_GPIO_PIN_PULLUP,
        .is_watcher = false,
        .hi_accuracy = false,
        .skip_gpio_setup = true
    };
    nrfx_gpiote_in_init(BUTTON_1, &btn_gpiote_cfg, &button_pressed_handler);
    nrfx_gpiote_in_event_enable(BUTTON_1, true);
 }

int main(void)
{
    
    uint8_t blink_array[DEVICE_COUNT_LED] = DEVICE_ID_LIST;
    uint8_t index_led = 0, repeat = 0, light = 0;
    uint32_t time_cicle = 0, time_current = 0;

    nrfx_systick_init();
    init_log();
    init_leds();
    init_button();
    init_gpiote();

    while (true)
    {
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
        if(freeze)
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
}