/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "main.h"
#include "beep.h"
#include "stm32f2xx_rtc.h"
#include "ui_diag.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0      (GUI_ID_USER + 0x01)
#define ID_BUTTON_1      (GUI_ID_USER + 0x02)
#define ID_SPINBOX_0     (GUI_ID_USER + 0x04)
#define ID_TEXT_0        (GUI_ID_USER + 0x05)
#define ID_SPINBOX_1     (GUI_ID_USER + 0x06)
#define ID_TEXT_1        (GUI_ID_USER + 0x07)
#define ID_SPINBOX_2     (GUI_ID_USER + 0x08)
#define ID_TEXT_2        (GUI_ID_USER + 0x09)
#define ID_SPINBOX_3     (GUI_ID_USER + 0x0A)
#define ID_SPINBOX_4     (GUI_ID_USER + 0x0B)
#define ID_TEXT_3        (GUI_ID_USER + 0x0C)
#define ID_TEXT_4        (GUI_ID_USER + 0x0D)
#define ID_SPINBOX_5     (GUI_ID_USER + 0x0F)
#define ID_TEXT_5        (GUI_ID_USER + 0x10)
#define ID_TEXT_6        (GUI_ID_USER + 0x11)
#define ID_BUTTON_3      (GUI_ID_USER + 0x12)
#define ID_TEXT_HEADER   (GUI_ID_USER + 0x13)

// USER START (Optionally insert additional defines)
extern const GUI_FONT GUI_FontHZ_yahei_16;
extern const GUI_FONT GUI_FontHZ_yahei_20;
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//extern int diag_err_creat(struct ui_exper_info *info);
//extern int diag_info_creat(struct ui_exper_info *info);
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {WINDOW_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0},
    {TEXT_CreateIndirect, "系统设置", ID_TEXT_HEADER, 300, 2, 200, 40, 0, 0x0, 0},
    
    {BUTTON_CreateIndirect, "触摸屏校准", ID_BUTTON_0, 10, 50, 770, 140, 0, 0x0, 0},

    {SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 10, 280, 110, 70, 0, 0x0, 0},
    {TEXT_CreateIndirect, "年", ID_TEXT_0, 123, 300, 25, 35, 0, 0x64, 0},
    {SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_1, 150, 280, 110, 70, 0, 0x0, 0},
    {TEXT_CreateIndirect, "月", ID_TEXT_1, 263, 300, 25, 35, 0, 0x64, 0},
    {SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_2, 290, 280, 110, 70, 0, 0x0, 0},
    {TEXT_CreateIndirect, "日", ID_TEXT_2, 403, 300, 25, 35, 0, 0x64, 0},
    {SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_3, 430, 280, 110, 70, 0, 0x0, 0},
    {TEXT_CreateIndirect, "时", ID_TEXT_3, 543, 300, 25, 35, 0, 0x64, 0},
    {SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_4, 570, 280, 110, 70, 0, 0x0, 0},
    {TEXT_CreateIndirect, "分", ID_TEXT_4, 683, 300, 25, 35, 0, 0x64, 0},
    {BUTTON_CreateIndirect, "保存", ID_BUTTON_1, 710, 280, 80, 70, 0, 0x0, 0},
    
    {BUTTON_CreateIndirect, "返回", ID_BUTTON_3, 638, 420, 146, 50, 0, 0x0, 0},
    // USER START (Optionally insert additional widgets)
    // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END
static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_DateTypeDef  RTC_DateStructure;

static void ctrl_all_items(WM_HWIN hWin, int enable)
{
    WM_HWIN hItem;
    int id;

    for (id = ID_BUTTON_0; id <= ID_BUTTON_3; id++) {
        hItem = WM_GetDialogItem(hWin, id);
        if (enable)
            WM_EnableWindow(hItem);
        else
            WM_DisableWindow(hItem);
    }    
}
/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_HEADER);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_20);
        TEXT_SetTextColor(hItem, GUI_DARKBLUE);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
        //
        // Initialization of 'Spinbox'
        // year
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
        SPINBOX_SetFont(hItem, GUI_FONT_32_ASCII);
        SPINBOX_SetRange(hItem, 0, 99);
        SPINBOX_SetValue(hItem, RTC_DateStructure.RTC_Year);
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        //
        // Initialization of 'Spinbox'
        // month
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
        SPINBOX_SetFont(hItem, GUI_FONT_32_ASCII);
        SPINBOX_SetRange(hItem, 1, 12);
        SPINBOX_SetValue(hItem, RTC_DateStructure.RTC_Month);
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        //
        // Initialization of 'Spinbox'
        // day
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
        SPINBOX_SetFont(hItem, GUI_FONT_32_ASCII);
        SPINBOX_SetRange(hItem, 1, 31);
        SPINBOX_SetValue(hItem, RTC_DateStructure.RTC_Date);
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        //
        // Initialization of 'Spinbox'
        // hour
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3);
        SPINBOX_SetFont(hItem, GUI_FONT_32_ASCII);
        SPINBOX_SetRange(hItem, 0, 23);
        SPINBOX_SetValue(hItem, RTC_TimeStructure.RTC_Hours);
        //
        // Initialization of 'Spinbox'
        // minute
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_4);
        SPINBOX_SetFont(hItem, GUI_FONT_32_ASCII);
        SPINBOX_SetRange(hItem, 0, 59);
        SPINBOX_SetValue(hItem, RTC_TimeStructure.RTC_Minutes);
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        //
        // Initialization of 'Button'
        //   
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_RED);
        // USER START (Optionally insert additional code for further widget initialization)
        // USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_BUTTON_0: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                // touch calc
                beep_clicked();
                g_ui_msg.msg = UI_MSG_LOAD_TOUCH_CALC;
                GUI_EndDialog(pMsg->hWin, 0);
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_1: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                
                hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
                RTC_DateStructure.RTC_Year = SPINBOX_GetValue(hItem);
		        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
                RTC_DateStructure.RTC_Month = SPINBOX_GetValue(hItem);
		        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
                RTC_DateStructure.RTC_Date = SPINBOX_GetValue(hItem);
		        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3);
                RTC_TimeStructure.RTC_Hours = SPINBOX_GetValue(hItem);
		        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_4);
                RTC_TimeStructure.RTC_Minutes = SPINBOX_GetValue(hItem);
		        RTC_TimeStructure.RTC_Seconds = 0;

                RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
                RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

                ctrl_all_items(pMsg->hWin, 0);
                WM_DisableWindow(pMsg->hWin);
                WM_Exec();
                g_diag_ok.header = "信息";
                g_diag_ok.str_lin1 = "时间设置成功!";
                g_diag_ok.str_lin2 = NULL;
                g_diag_ok.str_lin3 = NULL;
                diag_ok_creat();
                WM_EnableWindow(pMsg->hWin);
                ctrl_all_items(pMsg->hWin, 1);
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_SPINBOX_0: // Notifications sent by 'Spinbox'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();              
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_SPINBOX_1: // Notifications sent by 'Spinbox'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_SPINBOX_2: // Notifications sent by 'Spinbox'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_SPINBOX_3: // Notifications sent by 'Spinbox'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_SPINBOX_4: // Notifications sent by 'Spinbox'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_3: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                g_ui_msg.msg = UI_MSG_LOAD_MENU;
                GUI_EndDialog(pMsg->hWin, 0);
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
            // USER START (Optionally insert additional code for further Ids)
            // USER END
        }
        break;
    // USER START (Optionally insert additional message handling)
    // USER END
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateFramewin
*/
int ui_setting_creat(void)
{
    return GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
