#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "estc_service.h"

#define DEVICE_NAME                     "Semyon"
#define MANUFACTURER_NAME               "Nordic" 

typedef void (*connect_state_calback)();
typedef void (*recieve_calback)(uint8_t *data, uint16_t len);

typedef struct 
{
    connect_state_calback start_connect_callback;
    connect_state_calback stop_connect_callback;
    recieve_calback recieve_notify;
    recieve_calback recieve_inditify;
    value_char indication_value;
    value_char notification_value;
    bool is_notify_open;
}ble_context;

void ble_init(ble_context *context);

void idle_state_handle(void);

void send_data_notification(uint8_t *data, uint16_t len);

void send_data_indication(uint8_t *data, uint16_t len);