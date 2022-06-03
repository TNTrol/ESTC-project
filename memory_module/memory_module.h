#ifndef MEMORY_MODULE_H
#define MEMORY_MODULE_H

#include <stdint.h>
#include "nrfx_nvmc.h"
#include "../color_module/color_module.h"

#define START_FIRST_PAGE_FLASH 0x000DD000U
#define SIZE_PAGE_FLASH 0x1000U
#define SIZE_STEP_FLASH 0x4U
#define COUNT_PAGES_FLASH 2
#define DATA_MASK 0b11111111
#define CONTROL_MASK_FLASH ((0b11111111)<< 24)

void init_memory_module_32(void);

bool read_data_in_flash(hsv_t* out_data);

void write_data_in_flash(const hsv_t* in_data);

#endif