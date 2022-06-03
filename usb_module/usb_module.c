#include "usb_module.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
#include "string.h"

static func_usb_callback m_callback;


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
            m_callback(m_rx_buffer, READ_SIZE);
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

void init_usb_module(func_usb_callback callback)
{
    m_callback = callback;
    app_usbd_class_inst_t const * class_cdc_acm = app_usbd_cdc_acm_class_inst_get(&usb_cdc_acm);
    app_usbd_class_append(class_cdc_acm);
}

void usb_write_msg(const char *msg)
{
    app_usbd_cdc_acm_write(&usb_cdc_acm, msg, strlen(msg));
}
