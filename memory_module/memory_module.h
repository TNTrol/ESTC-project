#ifndef MEMORY_MODULE_H
#define MEMORY_MODULE_H

#include <stdint.h>
#include "nrf_fstorage.h"
#include "../color_module/color_module.h"

#define START_FIRST_PAGE_FLASH 0xDE000U
#define END_PAGE_FLASH 0xDEFFFU
#define SIZE_STEP_FLASH 0x4U
#define COUNT_PAGES_FLASH 2

void init_memory_module_32(hsv_t *color);

bool read_data_in_flash(hsv_t* out_data);

void write_data_in_flash(const hsv_t* in_data);

void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage);

#endif