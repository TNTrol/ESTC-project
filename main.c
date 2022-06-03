#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#if ESTC_USB_CLI_ENABLED
#include "usb_module/usb_module.h"
#include "command_module/command.h"
#endif

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "service/estc_ble.h"
#include "gpio_module/gpio_module.h"
#include "pwm_module/pwm_module.h"
#include "button_module/button_module.h"
#include "util_module/utils.h"
#include "color_module/color_module.h"
#include "nrf_delay.h"
#include "app_timer.h"

#define UPDATE_TIMEOUT 400
#define CONTROL_MASK 1
#define RGB_MASK ((1 << 1) | (1 << 2) | (1 << 3))
#define MAX_TIME_PWM_CICLE 20000
#define MAX_VALUE_COUNTER (1024 * 8)
#define MAX_VALUE_STEP_COUNTER (16)

static uint16_t         m_pwm_step      = 200;
static volatile uint8_t m_phase         = 0;
static modificator_t    m_state         = MOD_NONE;
static rgb_t            m_rgb_color     = {0, 0, 0};
static hsv_t            m_hsv_color     = {0, 100, 100};
static pwm_ctx_t        m_pwm           = GET_DEFAULT_CTX(0);
static pwm_ctx_t        m_pwm_rgb       = GET_DEFAULT_CTX(1);
APP_TIMER_DEF(m_update_timer_id);

#if ESTC_USB_CLI_ENABLED
static void handler_hsv_command(const char *line, uint8_t count_word);
static void handler_rgb_command(const char *line, uint8_t count_word);
static void handler_help_command(const char *line, uint8_t count_word);
static void handler_save_command(const char *line, uint8_t count_word);

static command_t        m_commands[]    = {
        {.name = "HSV", .handler = handler_hsv_command},
        {.name = "RGB", .handler = handler_rgb_command},
        {.name = "help", .handler = handler_help_command},
        {.name = "save", .handler = handler_save_command}
};
#endif

void rgb_on()
{
    uint32_t step   = MAX_TIME_PWM_CICLE / 255;
    uint32_t r      = m_rgb_color.r * step;
    uint32_t g      = m_rgb_color.g * step;
    uint32_t b      = m_rgb_color.b * step;
    set_value_of_channel(&m_pwm_rgb, 1, r);
    set_value_of_channel(&m_pwm_rgb, 2, g);
    set_value_of_channel(&m_pwm_rgb, 3, b);
}

static void save()
{
    // write_data_in_flash(&m_hsv_color);
    NRF_LOG_INFO("Save");
}

static void update_timer_handler(void *p)
{
    send_data_notification((uint8_t*) &m_rgb_color, 3);
    send_data_indication((uint8_t*) &m_rgb_color, 3);
}

void log_init()
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void stop()
{
    NRF_LOG_INFO("Disconnected");
    app_timer_stop(m_update_timer_id);
}

static void start()
{
    NRF_LOG_INFO("Connected");
    app_timer_start(m_update_timer_id, UPDATE_TIMEOUT, NULL);
}

static void recieve_notification(uint8_t *data, uint16_t len)
{
    NRF_LOG_INFO("Recieve: %d\n", len);
    m_rgb_color.r = data[0];
    m_rgb_color.g = data[1];
    m_rgb_color.b = data[2];
    rgb_on();
    rgb_to_hsv(&m_rgb_color, &m_hsv_color);
    save();
}

void init_log(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_INFO("Starting up the test project with USB logging");
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void double_button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    m_state = MOD_INCREMENT(m_state);
    m_phase = 0;
    switch (m_state)
    {
    case MOD_V:
    {
        m_pwm_step = 0;
        set_value_of_channel(&m_pwm, m_phase, MAX_TIME_PWM_CICLE);
        break;
    }
    case MOD_S:
    {
        m_pwm_step = MAX_TIME_PWM_CICLE / 4;
        break;
    }
    case MOD_H:
    {
        m_pwm_step = 300;
        break;
    }
    default:
        set_value_of_channel(&m_pwm, m_phase, 0);
        save();
        break;
    }
}

static void pwn_control_led_handler(nrfx_pwm_evt_type_t event_type)
{
    if (m_state == MOD_NONE)
    {
        return;
    }
    if (event_type == NRFX_PWM_EVT_FINISHED)
    {
        uint8_t channel = m_phase >> 1;
        bool down = m_phase & 1;
        bool next_phase = false;
        uint16_t value = get_value_of_channel(&m_pwm, channel);
        if (down)
        {
            value -= m_pwm_step;
            next_phase = value == 0;
        }
        else
        {
            value += m_pwm_step;
            next_phase = value >= MAX_TIME_PWM_CICLE;
        }
        set_value_of_channel(&m_pwm, channel, value);
        if (next_phase)
        {
            m_phase = down ? m_phase - 1 : m_phase + 1;
        }
    }
}

#if ESTC_USB_CLI_ENABLED
static bool parse_chars_to_numbers(const char *line, const uint8_t size, uint8_t *args)
{
    const char *start = line;
    char *end = NULL;
    long number = 0;
    for(uint8_t i = 0; i < size; ++i)
    {
        number = strtol(start, &end, 10);
        if(end == start || *end != '\0' || number < 0 || number > UINT8_MAX)
        {
            return false;
        }
        start = end + 1;
        args[i] = number;
    }
   
    return true;
}

static void func_convert_to_color(bool is_rgb, const char *line, uint8_t count_word)
{
    if(count_word != 3)
    {
        usb_write_msg("\r\nUnexpected count arguments\n\r");
        return;
    }
    color_t color;
    if(!parse_chars_to_numbers(line, 3, color.components))
    {
        usb_write_msg("\r\nIncorect argument\n\r");
        return;
    }
    rgb_t rgb;
    hsv_t hsv;
    if(is_rgb)
    {
        rgb = color.rgb;
        rgb_to_hsv(&rgb, &hsv);       
    }
    else
    {
        hsv = color.hsv;
        hsv_to_rgb(&hsv, &rgb);
    }
    m_hsv_color = hsv;
    m_rgb_color = rgb;
    rgb_on();
}

static void handler_rgb_command(const char *line, uint8_t count_word)
{
    func_convert_to_color(true, line, count_word);
}

static void handler_hsv_command(const char *line, uint8_t count_word)
{
    func_convert_to_color(false, line, count_word);
}

static void handler_help_command(const char *line, uint8_t count_word)
{
    if(count_word == 0)
    {
        usb_write_msg("\r\nCommands: RGB, HSV, help, save\n\r");
    }
    else
    {
        usb_write_msg("\r\nUnexpected count arguments\n\r");
    }
}

static void handler_save_command(const char *line, uint8_t count_word)
{
    if(count_word == 0)
    {
        save();
    }
    else
    {
        usb_write_msg("\r\nUnexpected count arguments\n\r");
    }
}

static void handler_usb_read(const char *msg, const uint8_t size)
{
    push_char_to_command(msg[0]);
}

static void handler_unknown_command(const char *line, uint8_t count_word)
{
    usb_write_msg("\r\nNot found command\n\r");
}
#endif

int main(void)
{
    uint16_t h = 0, s = 0, v = 0;

    app_timer_create(&m_update_timer_id , APP_TIMER_MODE_REPEATED, update_timer_handler);
    init_log();
    ble_context context = {.recieve_notify = recieve_notification, 
                            .start_connect_callback = start, 
                            .stop_connect_callback = stop,
                            .recieve_inditify = NULL,
                            .notification_value = {.value = (uint8_t*)&m_rgb_color, .size = 3},
                            .indication_value = {.value = (uint8_t*)&m_rgb_color, .size=3}};
    ble_init(&context);
    init_leds();
    init_button();
    init_pwn_module_for_leds(&m_pwm, pwn_control_led_handler, MAX_TIME_PWM_CICLE, CONTROL_MASK);
    init_pwn_module_for_leds(&m_pwm_rgb, NULL, MAX_TIME_PWM_CICLE, RGB_MASK);
    init_gpiote_button(double_button_handler);
    #if ESTC_USB_CLI_ENABLED
    init_usb_module(handler_usb_read);
    init_parse(ARRAY_SIZE(m_commands), m_commands, handler_unknown_command);
    #endif
    // init_memory_module_32();
    hsv_to_rgb(&m_hsv_color, &m_rgb_color);
    rgb_on();

    while (true)
    {
        idle_state_handle();
        #if ESTC_USB_CLI_ENABLED
        app_usbd_event_queue_process();
        #endif
        if(m_state != MOD_NONE && is_long_press())
        {
            NRF_LOG_INFO("State %d Color r=%d, g=%d, b=%d", m_state, m_rgb_color.r, m_rgb_color.g, m_rgb_color.b);
            switch (m_state)
            {
            case MOD_H:
                h = circle_increment(h, MAX_VALUE_COUNTER);
                m_hsv_color.h = h / MAX_VALUE_STEP_COUNTER;
                break;
            case MOD_S:
                s = circle_increment(s, MAX_VALUE_COUNTER);
                m_hsv_color.s = s / MAX_VALUE_STEP_COUNTER;
                break;
            default:
                v = circle_increment(v, MAX_VALUE_COUNTER);
                m_hsv_color.v = v / MAX_VALUE_STEP_COUNTER;
                break;
            }
            hsv_to_rgb(&m_hsv_color, &m_rgb_color);
            rgb_on();
            nrf_delay_ms(1);
        }
    }
}