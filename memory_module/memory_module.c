#include "memory_module.h"
#include "nrf_fstorage_sd.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"


// static volatile uint32_t m_address = 0;
// static uint8_t m_number_page = 0;

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);
NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    /* Set a handler for fstorage events. */
    .evt_handler = fstorage_evt_handler,
    .start_addr = START_FIRST_PAGE_FLASH,
    .end_addr   = END_PAGE_FLASH
};

static uint32_t nrf5_flash_end_addr_get()
{
    uint32_t const bootloader_addr = BOOTLOADER_ADDRESS;
    uint32_t const page_sz         = NRF_FICR->CODEPAGESIZE;
    uint32_t const code_sz         = NRF_FICR->CODESIZE;

    return (bootloader_addr != 0xFFFFFFFF ?
            bootloader_addr : (code_sz * page_sz));
}

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
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x. %x",
                         p_evt->len, p_evt->addr, nrf5_flash_end_addr_get());
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x. %x",
                         p_evt->len, p_evt->addr, nrf5_flash_end_addr_get());
        } break;

        default:
            break;
    }
}

// uint32_t find_end(uint32_t address)
// {
//     uint32_t temp = 0;
//     temp = 0;
//     nrf_fstorage_read(&fstorage, address, &temp, sizeof(temp));
//     if(((temp >> 24) & DATA_MASK) != DATA_MASK && ((temp >> 24) & DATA_MASK) != 0)
//     {
//         // nrfx_nvmc_page_erase(address);
//         nrf_fstorage_erase(&fstorage, address, 1, NULL);

//     }
//     temp = 0;
//     // while (temp == 0)
//     // {
//     //     temp = 0;//*((uint32_t*) address);
//     //     nrf_fstorage_read(&fstorage, address, &temp, sizeof(temp));
//     //     if(temp != 0xFFFFFFFF)
//     //     {
//     //         temp = DATA_MASK & (temp >> 24);
//     //         if(temp == 0)
//     //         {
//     //             address += SIZE_STEP_FLASH;
//     //         }
//     //         else
//     //         {
//     //             address = START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * (m_number_page);
//     //             // nrfx_nvmc_page_erase(address);
//     //             nrf_fstorage_erase(&fstorage, address, 1, NULL);
//     //         }
//     //         temp = 0;
//     //     }
//     //     if(address >= START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * (m_number_page + 1))
//     //     {
//     //         ++m_number_page;
//     //     }
//     // }
//     return address;
// }

void init_memory_module_32(hsv_t *color)
{
    nrf_fstorage_init(&fstorage, &nrf_fstorage_sd, NULL);
    // uint8_t *arr = (uint8_t*)(fstorage.start_addr);
    // color->h = arr[0];
    // color->s = arr[1];
    // color->v = arr[2];
    // nrf_fstorage_erase(&fstorage, fstorage.start_addr, 1, NULL);
    // nrf_fstorage_erase(&fstorage, fstorage.start_addr, 2, NULL);
    // m_address = find_end(START_FIRST_PAGE_FLASH);
}

static uint8_t temp[4];

bool read_data_in_flash(hsv_t* out_data)
{
    // if(m_address <= START_FIRST_PAGE_FLASH)
    // {
    //     return false;
    // }//*((uint32_t*) (m_address - SIZE_STEP_FLASH));
    // nrf_fstorage_read(&fstorage, fstorage.start_addr, &temp2, sizeof(temp2));
    // nrf_fstorage_read(&fstorage, m_address - SIZE_STEP_FLASH, &value, sizeof(value));
    uint8_t *color = (uint8_t*)(fstorage.start_addr);
    out_data->h = color[0];
    out_data->s = color[1];
    out_data->v = color[2];
    return true;
}



void write_data_in_flash(const hsv_t* in_data)
{
    temp[0] = in_data->h;
    temp[1] = in_data->s;
    temp[2] = in_data->v;
    temp[3] = 0; 
    // if(START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * (m_number_page + 1) < m_address + SIZE_STEP_FLASH)
    // {
    //     m_number_page = (m_number_page + 1) % COUNT_PAGES_FLASH;
    //     // nrfx_nvmc_page_erase(START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * m_number_page);
    //     nrf_fstorage_erase(&fstorage, START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * m_number_page, 1, NULL);
    //     m_address = START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * m_number_page;
    // }
    // temp = 0;//*((uint32_t*) m_address);
    // nrf_fstorage_read(&fstorage, m_address, &temp, sizeof(temp));
    // if(temp != 0xFFFFFFFF)
    // {
    //     m_address = START_FIRST_PAGE_FLASH + SIZE_PAGE_FLASH * m_number_page;
    //     // nrfx_nvmc_page_erase(m_address);
    //     nrf_fstorage_erase(&fstorage, m_address, 1, NULL);
    // }
    // // nrfx_nvmc_word_write(m_address, out);
    // nrf_fstorage_write(&fstorage,
    //                  fstorage.start_addr,
    //                  &out,
    //                  sizeof(out), NULL);
    // m_address += SIZE_STEP_FLASH; 
    nrf_fstorage_erase(&fstorage, fstorage.start_addr, 1, NULL);
    nrf_fstorage_write(&fstorage,
                     fstorage.start_addr,
                     temp,
                     sizeof(temp), NULL);
    // NRF_LOG_INFO("Ret %d", ret);
    // ret = nrf_fstorage_read(&fstorage, fstorage.start_addr, arr, sizeof(arr));
    
    // uint8_t *ar2 = (uint8_t*)(fstorage.start_addr);
    // NRF_LOG_INFO("Write %d %d %d %d", temp[0], temp[1], temp[2], temp[3]);
    // NRF_LOG_INFO("Read %d %d %d %d ", ~arr[0], ~arr[1], ~arr[2], ~arr[3]);
    // NRF_LOG_INFO("Read2 %d %d %d %d ", ar2[0], ar2[1], ar2[2], ar2[3]);
}

void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        (void) sd_app_evt_wait();
    }
}