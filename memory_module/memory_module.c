#include "memory_module.h"
#include "nrf_fstorage_sd.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"


static uint32_t m_address = 0;
static uint8_t  buffer[4];

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);

NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    .evt_handler = fstorage_evt_handler,
    .start_addr = START_FIRST_PAGE_FLASH,
    .end_addr   = END_PAGE_FLASH
};

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> Event received: ERROR while executing an fstorage operation.");
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        default:
            break;
    }
}

uint32_t find_end(uint32_t address)
{
    uint8_t *temp = (uint8_t*) address;
    if(temp[3] != 0 && temp[3] != 0xFF)
    {
        nrf_fstorage_erase(&fstorage, address, 1, NULL);
        return fstorage.start_addr + SIZE_STEP_FLASH;
    }
    while (temp[3] == 0)
    {
        temp = (uint8_t*) address;
        if(temp[3] != 0xFF)
        {
            if(temp[3] == 0)
            {
                address += SIZE_STEP_FLASH;
            }
            else
            {
                nrf_fstorage_erase(&fstorage, fstorage.start_addr, 1, NULL);
                address = fstorage.start_addr + SIZE_STEP_FLASH;
                break;
            }
        }
        if(address >= END_PAGE_FLASH)
        {
            nrf_fstorage_erase(&fstorage, fstorage.start_addr, 1, NULL);
            address = fstorage.start_addr + SIZE_STEP_FLASH;
            break;
        }
    }
    return address;
}

void init_memory_module_32(hsv_t *color)
{
    nrf_fstorage_init(&fstorage, &nrf_fstorage_sd, NULL);
    m_address = find_end(fstorage.start_addr);
    uint8_t *arr = (uint8_t*)(m_address- SIZE_STEP_FLASH);
    color->h = arr[0];
    color->s = arr[1];
    color->v = arr[2];
}

bool read_data_in_flash(hsv_t* out_data)
{
    if(m_address <= START_FIRST_PAGE_FLASH)
    {
        return false;
    }
    uint8_t *color = (uint8_t*)(m_address - SIZE_STEP_FLASH);
    out_data->h = color[0];
    out_data->s = color[1];
    out_data->v = color[2];
    return true;
}

void write_data_in_flash(const hsv_t* in_data)
{
    buffer[0] = in_data->h;
    buffer[1] = in_data->s;
    buffer[2] = in_data->v;
    buffer[3] = 0; 
    if(fstorage.end_addr < m_address + SIZE_STEP_FLASH || ((uint8_t*)m_address)[3] != 0xFF)
    {
        nrf_fstorage_erase(&fstorage, fstorage.start_addr, 1, NULL);
        m_address = fstorage.start_addr;
    }
    nrf_fstorage_write(&fstorage,
                     m_address,
                     buffer,
                     sizeof(buffer), NULL);
    m_address += SIZE_STEP_FLASH;
}

void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        (void) sd_app_evt_wait();
    }
}