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

#include "estc_ble.h"

void log_init()
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void stop()
{
    NRF_LOG_DEBUG("Disconnected");
}

static void start()
{
    NRF_LOG_DEBUG("Connected");
}

static void recieve(uint8_t *data, uint16_t len)
{
    NRF_LOG_DEBUG("Recieve: %d\n", len);
    for(uint8_t i = 0; i < len; ++i)
    {
        NRF_LOG_DEBUG("data%d: %d\n", i, data[i]);
    }
}

// void context_ble_init()
// {
//     ble_context context = {.recieve = recieve, .start_callback = start, .stop_callback = stop};
//     ble_init(&context);
// }

    /**@brief Function for application main entry.
     */
int main(void)
{
    // Initialize.
    log_init();
    ble_context context = {.recieve = recieve, .start_callback = start, .stop_callback = stop};
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
