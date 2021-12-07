#include "usb_module.h"

static func_usb_callback m_callback;
static command_t m_commands[] = {{.name = "HSV\0", .count_argument = 3}, {.name = "RGB\0", .count_argument = 3}, {.name = "help\0", .count_argument = 0}};

static void usb_ev_handler(app_usbd_class_inst_t const * p_inst,
                           app_usbd_cdc_acm_user_event_t event);

static char m_rx_buffer[READ_SIZE];

APP_USBD_CDC_ACM_GLOBAL_DEF(usb_cdc_acm,
                            usb_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250);

static void usb_ev_handler(app_usbd_class_inst_t const * p_inst,
                           app_usbd_cdc_acm_user_event_t event)
{
    switch (event)
    {
    case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
    {
        ret_code_t ret;
        ret = app_usbd_cdc_acm_read(&usb_cdc_acm, m_rx_buffer, READ_SIZE);
        UNUSED_VARIABLE(ret);
        break;
    }
    case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
    {
        break;
    }
    case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
    {
        break;
    }
    case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
    {
        ret_code_t ret;   
        do
        {
            app_usbd_cdc_acm_rx_size(&usb_cdc_acm); //size_t size = 
            print_char(m_rx_buffer[0]);
            if (m_rx_buffer[0] == '\r' || m_rx_buffer[0] == '\n')
            {
                ret = app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\n", 2);
            }
            else
            {
                ret = app_usbd_cdc_acm_write(&usb_cdc_acm, m_rx_buffer, READ_SIZE);
            }
            /* Fetch data until internal buffer is empty */
            ret = app_usbd_cdc_acm_read(&usb_cdc_acm, m_rx_buffer, READ_SIZE);
        } while (ret == NRF_SUCCESS);
        break;
    }
    default:
        break;
    }
}

static void function_callback_convert(state_parse_t state, uint8_t index, uint8_t *args)
{
    switch (state)
    {
        case SUCCESS:
        {
            if(index < 2)
            {
                m_callback(index == 1, args[0], args[1], args[2]);
            }
            else
            {
                app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\nCommands: RGB, HSV, help\n\r", 28);
            }
            break;
        }
        case COMMAND_NOT_FOUND_ERROR:
        {
            app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\nNot found command\n\r", 21);
            break;
        }
        case MISSING_ARGUMENT_ERROR:
        {
            app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\nMissing argument\n\r", 20);
            break;
        }
        case UNEXPECTED_ARGUMENT_ERROR:
        {
            app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\nUnexpected argument\n\r", 23);
            break;
        }
        case PARSE_ARGUMENT_ERROR:
        {
            app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\nIncorect argument\n\r", 21);
            break;
        }   
        default:
        {
            app_usbd_cdc_acm_write(&usb_cdc_acm, "\r\nBuffer is full\n\r", 18);
            break;
        }
    }
}


void init_usb_module(func_usb_callback callback)
{
    m_callback = callback;
    init_parse(function_callback_convert, 3, m_commands);
    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&usb_cdc_acm);
    app_usbd_class_append(class_cdc_acm);
}
