#include "button_module.h"

void init_gpiote_button(nrfx_gpiote_evt_handler_t evt_handler)
{
       nrfx_gpiote_init();
    const nrfx_gpiote_in_config_t btn_gpiote_cfg = {
        .sense = NRF_GPIOTE_POLARITY_HITOLO,
        .pull = NRF_GPIO_PIN_PULLUP,
        .is_watcher = false,
        .hi_accuracy = false,
        .skip_gpio_setup = true
    };
    nrfx_gpiote_in_init(BUTTON_1, &btn_gpiote_cfg, evt_handler);
    nrfx_gpiote_in_event_enable(BUTTON_1, true);
}