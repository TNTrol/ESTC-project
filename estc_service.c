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

#define WRITE_PROP 0b1
#define READ_PROP 0b10


// static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service);

static ret_code_t estc_ble_add_uni_characteristics(uint16_t service_handle, 
                                                    const uint8_t *description, 
                                                    const uint16_t size, 
                                                    const uint8_t props, 
                                                    uint16_t uuid, 
                                                    ble_gatts_char_handles_t *p_handles);

ret_code_t estc_ble_service_init(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;
    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};
    ble_uuid_t ble_uuid = {.uuid = ESTC_SERVICE_UUID};

    error_code = sd_ble_uuid_vs_add(&base_uuid, &ble_uuid.type);
    APP_ERROR_CHECK(error_code);
    error_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &service->service_handle);
    APP_ERROR_CHECK(error_code);

    // return estc_ble_add_characteristics(service);

    const uint8_t desc[] = ESTC_USER_CHAR_1_DESCR;
    error_code = estc_ble_add_uni_characteristics(service->service_handle, desc, sizeof(desc), READ_PROP | WRITE_PROP, ESTC_GATT_CHAR_1_UUID, &service->first_characteristic);
    APP_ERROR_CHECK(error_code);
    const uint8_t desc1[] = ESTC_OTHER_CHAR_2_DESCR;
    return estc_ble_add_uni_characteristics(service->service_handle, desc1, sizeof(desc1), READ_PROP, ESTC_GATT_CHAR_2_UUID, &service->second_characteristic);

}

// static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service)
// {
//     ret_code_t error_code = NRF_SUCCESS;

//     // TODO: 6.1. Add custom characteristic UUID using `sd_ble_uuid_vs_add`, same as in step 4
//     ble_uuid128_t base_uuid = {ESTC_BASE_UUID};
//     ble_uuid_t ble_uuid = {.uuid = ESTC_GATT_CHAR_1_UUID};
//     const uint8_t desc[] = ESTC_USER_CHAR_1_DESCR;

//     error_code = sd_ble_uuid_vs_add(&base_uuid, &ble_uuid.type);
//     APP_ERROR_CHECK(error_code);
//     // TODO: 6.5. Configure Characteristic metadata (enable read and write)
//     ble_gatts_char_md_t char_md = { 0 };
//     char_md.char_props.read = 1;
//     char_md.char_props.write = 1;
//     char_md.char_user_desc_max_size = sizeof(desc);
//     char_md.char_user_desc_size = sizeof(desc);
//     char_md.p_char_user_desc = desc;
//     // char_md.


//     // Configures attribute metadata. For now we only specify that the attribute will be stored in the softdevice
//     ble_gatts_attr_md_t attr_md = { 0 };
//     attr_md.vloc = BLE_GATTS_VLOC_STACK;
    


//     // TODO: 6.6. Set read/write security levels to our attribute metadata using `BLE_GAP_CONN_SEC_MODE_SET_OPEN`
//     BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
//     BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

//     // TODO: 6.2. Configure the characteristic value attribute (set the UUID and metadata)
//     ble_gatts_attr_t attr_char_value = { 0 };
//     attr_char_value.p_attr_md = &attr_md;
//     attr_char_value.p_uuid = &ble_uuid;
    


//     // TODO: 6.7. Set characteristic length in number of bytes in attr_char_value structure
//     attr_char_value.init_len = sizeof(attr_char_value);
//     attr_char_value.max_len = sizeof(attr_char_value);

//     // TODO: 6.4. Add new characteristic to the service using `sd_ble_gatts_characteristic_add`
//     error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, &(service->first_characteristic));
//     APP_ERROR_CHECK(error_code);
//     return NRF_SUCCESS;
// }

static ret_code_t estc_ble_add_uni_characteristics(uint16_t service_handle, 
                                                    const uint8_t *description, 
                                                    const uint16_t size, 
                                                    const uint8_t props, 
                                                    uint16_t uuid, 
                                                    ble_gatts_char_handles_t *p_handles)
{
    ret_code_t error_code = NRF_SUCCESS;
    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};
    ble_uuid_t ble_uuid = {.uuid = uuid};
    ble_gatts_attr_md_t attr_md = { 0 };
    ble_gatts_char_md_t char_md = { 0 };
    ble_gatts_attr_t attr_char_value = { 0 };

    error_code = sd_ble_uuid_vs_add(&base_uuid, &ble_uuid.type);
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
    
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.init_len = sizeof(attr_char_value);
    attr_char_value.max_len = sizeof(attr_char_value);
    return sd_ble_gatts_characteristic_add(service_handle, &char_md, &attr_char_value, p_handles);
}