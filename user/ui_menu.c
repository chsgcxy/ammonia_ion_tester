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
#include "stdio.h"
#include "main.h"
#include "beep.h"
#include "config.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)

#define ID_IMAGE_TEST        (GUI_ID_USER + 0x02)
#define ID_IMAGE_CHECK       (GUI_ID_USER + 0x03)
#define ID_IMAGE_DATA        (GUI_ID_USER + 0x04)
#define ID_IMAGE_SETTING     (GUI_ID_USER + 0x05)

#define ID_TEXT_TEST         (GUI_ID_USER + 0x06)
#define ID_TEXT_CHECK        (GUI_ID_USER + 0x07)
#define ID_TEXT_DATA         (GUI_ID_USER + 0x08)
#define ID_TEXT_SETTING      (GUI_ID_USER + 0x09)

#define ID_TEXT_VERSION      (GUI_ID_USER + 0x0A)

// USER START (Optionally insert additional defines)
extern const GUI_BITMAP bmchemical_72px;
extern const GUI_BITMAP bmelectricplug;
extern const GUI_BITMAP bmpic_view_72px;
extern const GUI_BITMAP bmprocess_running_72px;

extern const GUI_FONT GUI_FontHZ_yahei_20;
extern const GUI_FONT GUI_FontHZ_yahei_16;
// USER END

#define BMP_LINE1_YPOS      (50)
#define TEXT_LINE1_YPOS     (BMP_LINE1_YPOS + 85)

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },

    { IMAGE_CreateIndirect, "test", ID_IMAGE_TEST, 102, BMP_LINE1_YPOS, 72, 72, 0, 0, 0 },
    { IMAGE_CreateIndirect, "check", ID_IMAGE_CHECK, 274, BMP_LINE1_YPOS, 72, 72, 0, 0, 0 },
    { IMAGE_CreateIndirect, "data", ID_IMAGE_DATA, 446, BMP_LINE1_YPOS, 72, 72, 0, 0, 0 },
    { IMAGE_CreateIndirect, "setting", ID_IMAGE_SETTING, 620, BMP_LINE1_YPOS, 72, 72, 0, 0, 0 },

    { TEXT_CreateIndirect, "test", ID_TEXT_TEST, 100, TEXT_LINE1_YPOS, 120, 45, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "check", ID_TEXT_CHECK, 252, TEXT_LINE1_YPOS, 120, 45, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "data", ID_TEXT_DATA, 440, TEXT_LINE1_YPOS, 120, 45, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "setting", ID_TEXT_SETTING, 610, TEXT_LINE1_YPOS, 120, 45, 0, 0x0, 0 },

    { TEXT_CreateIndirect, SOFTWARE_VERSION_STR, ID_TEXT_VERSION, 725, 395, 90, 16, 0, 0x0, 0 },
    // USER START (Optionally insert additional widgets)
    // USER END
};

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN      hItem;
    int     NCode;
    int     Id;

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //
        hItem = pMsg->hWin;
        FRAMEWIN_SetTitleHeight(hItem, 50);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_SetFont(hItem, &GUI_FontHZ_yahei_20);
        FRAMEWIN_SetTextColor(hItem, GUI_BLACK_33);
        FRAMEWIN_SetClientColor(hItem, GUI_WHITE);
        FRAMEWIN_SetText(hItem, "全自动氨离子分析仪");
        //
        // Initialization of 'Image'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_TEST);
        IMAGE_SetBitmap(hItem, &bmchemical_72px);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TEST);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "离子分析");  

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_CHECK);
        IMAGE_SetBitmap(hItem, &bmelectricplug);        

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CHECK);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "电极校对");
        //
        // Initialization of 'Image'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_DATA);
        IMAGE_SetBitmap(hItem, &bmpic_view_72px);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DATA);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "数据处理");
        //
        // Initialization of 'Image'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_SETTING);
        IMAGE_SetBitmap(hItem, &bmprocess_running_72px);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SETTING);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "系统设置");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);
        TEXT_SetFont(hItem, GUI_FONT_20_ASCII);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_IMAGE_TEST:
        case ID_TEXT_TEST:
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                g_ui_msg.msg = UI_MSG_LOAD_TEST;
                GUI_EndDialog(pMsg->hWin, 0);
                break;
            default:
                break;
            }
            break;
        case ID_IMAGE_CHECK:
        case ID_TEXT_CHECK:
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                g_ui_msg.msg = UI_MSG_LOAD_CHECK;
                GUI_EndDialog(pMsg->hWin, 0);
                break;
            default:
                break;
            }
            break;
        case ID_IMAGE_DATA:
        case ID_TEXT_DATA:
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                g_ui_msg.msg = UI_MSG_LOAD_DATA;
                GUI_EndDialog(pMsg->hWin, 0);
                break;
            default:
                break;
            }
            break;
        case ID_IMAGE_SETTING:
        case ID_TEXT_SETTING:
            switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                g_ui_msg.msg = UI_MSG_LOAD_SETTING;
                GUI_EndDialog(pMsg->hWin, 0);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
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
int main_menu_creat(void)
{
    return GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
