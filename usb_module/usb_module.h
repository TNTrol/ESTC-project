#ifndef USB_MODULE_H
#define USB_MODULE_H

#include "app_usbd.h"
#include "app_usbd_serial_num.h"
#include "app_usbd_cdc_acm.h"
#include "../convert_module/convert_cmd.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#define READ_SIZE 1

#define CDC_ACM_COMM_INTERFACE  2
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN3

#define CDC_ACM_DATA_INTERFACE  3
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN4
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT4

typedef void(*func_usb_callback)(bool flag, uint8_t data1, uint8_t data2, uint8_t data3);

void init_usb_module(func_usb_callback callback);

#endif