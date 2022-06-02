    /**
     * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
     *
     * All rights reserved.
     *
     * Redistribution and use in source and binary forms, with or without modification,
     * are permitted provided that the following conditions are met:
     *
     * 1. Redistributions of source code must retain the above copyright notice, this
     *    list of conditions and the following disclaimer.
     *
     * 2. Redistributions in binary form, except as embedded into a Nordic
     *    Semiconductor ASA integrated circuit in a product or a software update for
     *    such product, must reproduce the above copyright notice, this list of
     *    conditions and the following disclaimer in the documentation and/or other
     *    materials provided with the distribution.
     *
     * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
     *    contributors may be used to endorse or promote products derived from this
     *    software without specific prior written permission.
     *
     * 4. This software, with or without modification, must only be used with a
     *    Nordic Semiconductor ASA integrated circuit.
     *
     * 5. Any software provided in binary form under this license must not be reverse
     *    engineered, decompiled, modified and/or disassembled.
     *
     * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
     * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
     * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
     * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
     * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
     * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
     * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
     * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
     * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
     * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     *
     */
    /** @file
     *
     * @defgroup estc_gatt main.c
     * @{
     * @ingroup estc_templates
     * @brief ESTC-GATT project file.
     *
     * This file contains a template for creating a new BLE application with GATT services. It has
     * the code necessary to advertise, get a connection, restart advertising on disconnect.
     */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "estc_timer.h"
#include "estc_ble.h"

static uint8_t color[3];

void log_init()
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void stop()
{
    NRF_LOG_INFO("Disconnected");
    timer_stop_indification();
    timer_stop_notification();
}

static void start()
{
    NRF_LOG_INFO("Connected");
    timer_start_notification();
    timer_start_indification();
}

static void recieve_notification(uint8_t *data, uint16_t len)
{
    NRF_LOG_INFO("Recieve: %d\n", len);
    for(uint8_t i = 0; i < len; ++i)
    {
        NRF_LOG_INFO("data%d: %d\n", i, data[i]);
    }
}

static void recieve_indification(uint8_t *data, uint16_t len)
{
    NRF_LOG_INFO("Recieve: %d\n", len);
    for(uint8_t i = 0; i < len; ++i)
    {
        NRF_LOG_INFO("data%d: %d\n", i, data[i]);
    }
}

static uint32_t notification_value = 0xFAA;
static uint32_t indication_value = 0xFFAAA;

static void notifying_char_timeout_handler(void *p_context)
{
    
    notification_value += notification_value * 7 % 0xFFFF;
    send_data_notification((uint8_t*)&notification_value, 3);
}

static void indicating_char_timeout_handler(void *p_context)
{   
    indication_value = indication_value * 7 % 0xFFFFF;
    send_data_indication((uint8_t*)&indication_value, 3);
}


int main(void)
{
    // Initialize.
    log_init();
    timer_init(notifying_char_timeout_handler, indicating_char_timeout_handler);
    ble_context context = {.recieve_notify = recieve_notification, 
                            .start_callback = start, 
                            .stop_callback = stop,
                            .recieve_inditify =recieve_indification,
                            .notification_value = {.value =color, .size = 3},
                            .indication_value = {.value = color, .size=3},
                            .defult_value = {.value = NULL, .size = 0}};
    ble_init(&context);

    // Enter main loop.
    for (;;)
    {
        idle_state_handle();
    }
}


    /**
     * @}
     */
