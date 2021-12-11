#ifndef USB_MODULE_H
#define USB_MODULE_H

#include "app_usbd.h"
#include "app_usbd_serial_num.h"
#include "app_usbd_cdc_acm.h"


#define READ_SIZE 1U

#define CDC_ACM_COMM_INTERFACE  2
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN3

#define CDC_ACM_DATA_INTERFACE  3
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN4
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT4

typedef void(*func_usb_callback)(const char *chars, const uint8_t size);

void init_usb_module(func_usb_callback callback);
void usb_write_msg(const char *msg, const uint8_t size);

#endif