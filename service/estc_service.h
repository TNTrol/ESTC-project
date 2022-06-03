/**
 * Copyright 2022 Evgeniy Morozov
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE
*/

#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

// TODO: 1. Generate random BLE UUID (Version 4 UUID) and define it in the following format:
//c2d32d38-c07d-11ec-9d64-0242ac120002
#define ESTC_BASE_UUID { 0x02, 0x00, 0x12, 0xAC, 0x42, 0x02, /* - */ 0x64, 0x9B, /* - */ 0xEC, 0x11, /* - */ 0x7D, 0xC0, /* - */ 0x38, 0x2D, 0xD3, 0xC2  }

// TODO: 2. Pick a random service 16-bit UUID and define it:
#define ESTC_SERVICE_UUID 0x6579

#define ESTC_GATT_CHAR_1_UUID  0x1234
#define ESTC_GATT_CHAR_2_UUID  0x1101
#define ESTC_GATT_CHAR_3_UUID  0x5301

#define ESTC_USER_CHAR_DESCR "User characteristic"
#define ESTC_OTHER_CHAR_DESCR "Other characteristic"
#define ESTC_NOTIFICATION_CHAR_DESCR "Notification characteristic"

#define ESTC_NOTIFY_CHAR_DEF_VAL      0xBEEU
#define ESTC_INDICATION_CHAR_DEF_VAL  0xFEED


typedef struct
{
    uint16_t                    service_handle;
    uint16_t                    connection_handle;
    ble_gatts_char_handles_t    first_characteristic;
    ble_gatts_char_handles_t    indication_characteristic;
    ble_gatts_char_handles_t    notification_characteristic;
} ble_estc_service_t;

typedef struct 
{
    uint8_t *value;
    uint16_t size;
}value_char;


ret_code_t estc_ble_service_init(ble_estc_service_t *service, value_char values[2]);

void estc_ble_service_on_ble_event(const ble_evt_t *ble_evt, void *ctx);

void estc_update_characteristic_1_value(ble_estc_service_t *service, uint8_t *value, uint16_t len);
void estc_update_characteristic_value(uint16_t connection, uint16_t value_handle, uint8_t type, uint8_t *value, uint16_t len);

#endif /* ESTC_SERVICE_H__ */