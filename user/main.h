#ifndef __MAIN_H__
#define __MAIN_H__

#include "usbh_usr.h"

#define GUI_BLACK_33      0x33333333
#define GUI_GRAY_EE       0xEEEEEEEE
#define GUI_GRAY_CC       0xCCCCCCCC

struct ui_msg {
#define UI_MSG_LOAD_MENU         0x01
#define UI_MSG_LOAD_CHECK        0x02
#define UI_MSG_LOAD_TEST         0x03
#define UI_MSG_LOAD_SETTING      0x04
#define UI_MSG_LOAD_DATA         0x05
#define UI_MSG_LOAD_DETAIL       0x06
#define UI_MSG_LOAD_TOUCH_CALC   0x07
    int msg;
    int param0;
};

extern struct ui_msg g_ui_msg;
extern USBH_HOST USB_Host;
extern USB_OTG_CORE_HANDLE USB_OTG_Core;

#endif
