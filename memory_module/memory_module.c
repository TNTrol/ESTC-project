#include "memory_module.h"


static volatile uint32_t m_address = 0;
static uint8_t m_number_page = 0;

uint32_t find_end(uint32_t address) // вынес для дебага
{
    uint32_t temp = 0;
    while (temp == 0)
    {
        temp = *((uint32_t*) address);
        temp = DATA_MASK & (temp >> 24);
        if(temp == 0)
        {
            address += SIZE_STEP_FLASH;
        }
        if(address >= START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * (m_number_page + 1))
        {
            ++m_number_page;
        }
    }
    return address;
}

void init_memory_module_32(void)
{
    m_address = find_end(START_FIRST_PAGE_FLASH);
}

bool read_data_in_flash(hsv_t* out_data)
{
    if(m_address <= START_FIRST_PAGE_FLASH)
    {
        return false;
    }
    uint32_t value = *((uint32_t*) (m_address - SIZE_STEP_FLASH));
    out_data->h = value & (DATA_MASK);
    out_data->v = (value & (DATA_MASK << 8)) >> 8;
    out_data->s = (value & (DATA_MASK << 16)) >> 16;
    return true;
}

void write_data_in_flash(const hsv_t* in_data)
{
    uint32_t out = 0U;
    out |= in_data->h;
    out |= in_data->v << 8;
    out |= in_data->s << 16;
    if(START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * (m_number_page + 1) < m_address + SIZE_STEP_FLASH)
    {
        m_number_page = (m_number_page + 1) % COUNT_PAGES_FLASH;
        nrfx_nvmc_page_erase(START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * m_number_page);
        m_address = START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * m_number_page;
    }
    nrfx_nvmc_word_write(m_address, out);
    m_address += SIZE_STEP_FLASH; 
}
