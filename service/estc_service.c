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

#include "estc_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

#define WRITE_PROP      0b1
#define READ_PROP       0b10
#define INDICATE_PROP   0b100
#define NOTIFY_PROP     0b1000

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service, value_char values[2]);

static ret_code_t estc_ble_add_uni_characteristic(uint16_t service_handle, 
                                                    const uint8_t *description, 
                                                    const uint16_t size, 
                                                    const uint8_t props, 
                                                    ble_uuid_t *ble_uuid, 
                                                    ble_gatts_char_handles_t *p_handles,
                                                    uint8_t *data,
                                                    uint16_t data_size);


ret_code_t estc_ble_service_init(ble_estc_service_t *service, value_char values[2])
{
    ret_code_t      error_code = NRF_SUCCESS;
    ble_uuid128_t   base_uuid = {ESTC_BASE_UUID};
    ble_uuid_t      ble_uuid = {.uuid = ESTC_SERVICE_UUID};

    error_code = sd_ble_uuid_vs_add(&base_uuid, &ble_uuid.type);
    APP_ERROR_CHECK(error_code);
    error_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &service->service_handle);
    APP_ERROR_CHECK(error_code);

    return estc_ble_add_characteristics(service, values);
}

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service, value_char values[2])
{
    ret_code_t      error_code  = NRF_SUCCESS;
    const uint8_t   desc2[]     = ESTC_OTHER_CHAR_DESCR;
    const uint8_t   desc3[]     = ESTC_NOTIFICATION_CHAR_DESCR;
    ble_uuid_t      ble_uuid2   = {.uuid = ESTC_GATT_CHAR_2_UUID, .type = BLE_UUID_TYPE_BLE};
    ble_uuid_t      ble_uuid3   = {.uuid = ESTC_GATT_CHAR_3_UUID, .type = BLE_UUID_TYPE_BLE};

    error_code = estc_ble_add_uni_characteristic(service->service_handle, 
                                                desc2, 
                                                sizeof(desc2),  
                                                READ_PROP | INDICATE_PROP, 
                                                &ble_uuid2, 
                                                &service->indication_characteristic, 
                                                values[0].value, 
                                                values[0].size);
    APP_ERROR_CHECK(error_code);
    return estc_ble_add_uni_characteristic(service->service_handle, 
                                            desc3,
                                            sizeof(desc3), 
                                            WRITE_PROP | READ_PROP | NOTIFY_PROP, 
                                            &ble_uuid3, 
                                            &service->notification_characteristic, 
                                            values[1].value, 
                                            values[1].size);
}

static ret_code_t estc_ble_add_uni_characteristic(uint16_t service_handle, 
                                                    const uint8_t *description, 
                                                    const uint16_t size, 
                                                    const uint8_t props, 
                                                    ble_uuid_t *ble_uuid, 
                                                    ble_gatts_char_handles_t *p_handles,
                                                    uint8_t *data,
                                                    uint16_t data_size)
{
    ret_code_t          error_code      = NRF_SUCCESS;
    ble_uuid128_t       base_uuid       = {ESTC_BASE_UUID};
    ble_gatts_attr_md_t attr_md         = { 0 };
    ble_gatts_char_md_t char_md         = { 0 };
    ble_gatts_attr_t    attr_char_value = { 0 };

    error_code = sd_ble_uuid_vs_add(&base_uuid, &ble_uuid->type);
    APP_ERROR_CHECK(error_code);
    
    char_md.char_user_desc_max_size = size;
    char_md.char_user_desc_size = size;
    char_md.p_char_user_desc = description;

    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    if(props & WRITE_PROP)
    {
        char_md.char_props.write = 1;
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    }
    if(props & READ_PROP)
    {
        char_md.char_props.read = 1;
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    }
    if(props & NOTIFY_PROP)
    {
        char_md.char_props.notify = 1;
    }
    if(props & INDICATE_PROP)
    {
        char_md.char_props.indicate = 1;
    }
    
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.p_uuid = ble_uuid;
    attr_char_value.max_len = data_size;
    attr_char_value.p_value = data;
    attr_char_value.init_len = data_size;
    return sd_ble_gatts_characteristic_add(service_handle, &char_md, &attr_char_value, p_handles);
}

void estc_update_characteristic_value(uint16_t connection, uint16_t value_handle, uint8_t type, uint8_t *value, uint16_t len)
{
    ble_gatts_hvx_params_t hvx_params;
    hvx_params.handle = value_handle;
    hvx_params.type   = type;
    hvx_params.offset = 0;
    hvx_params.p_len  = &len;
    hvx_params.p_data = value;

    sd_ble_gatts_hvx(connection, &hvx_params);
}