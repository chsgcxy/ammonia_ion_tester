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
#include "data.h"
#include "usbh_usr.h"
#include "delay.h"
#include "ui_diag.h"
#include "test.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0       (GUI_ID_USER + 0x00)
#define ID_LISTBOX_0        (GUI_ID_USER + 0x01)
#define ID_BUTTON_LOOK      (GUI_ID_USER + 0x02)
#define ID_BUTTON_DEL       (GUI_ID_USER + 0x03)
#define ID_BUTTON_EXP       (GUI_ID_USER + 0x04)
#define ID_BUTTON_RETURN    (GUI_ID_USER + 0x05)
#define ID_BUTTON_EXPALL    (GUI_ID_USER + 0x06)
#define ID_BUTTON_DELALL    (GUI_ID_USER + 0x07)
#define ID_TEXT_HEADER      (GUI_ID_USER + 0x08)


// USER START (Optionally insert additional defines)
extern const GUI_FONT GUI_FontHZ_yahei_20;
extern const GUI_FONT GUI_FontHZ_yahei_16;
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {WINDOW_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0},
    {TEXT_CreateIndirect, "数据处理", ID_TEXT_HEADER, 300, 2, 200, 40, 0, 0x0, 0},

    {LISTBOX_CreateIndirect, "Listbox", ID_LISTBOX_0, 5, 55, 380, 415, 0, 0x0, 0},

    {BUTTON_CreateIndirect, "查看", ID_BUTTON_LOOK, 430, 80, 140, 60, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "返回", ID_BUTTON_RETURN, 640, 80, 140, 60, 0, 0x0, 0},

    {BUTTON_CreateIndirect, "删除", ID_BUTTON_DEL, 430, 230, 140, 60, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "全部删除", ID_BUTTON_DELALL, 640, 230, 140, 60, 0, 0x0, 0},

    {BUTTON_CreateIndirect, "导出至U盘", ID_BUTTON_EXPALL, 430, 380, 350, 60, 0, 0x0, 0},
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

void uidata_usb_cmd_load(struct data_usb_cmd *cmd)
{
    cmd->cmd = USB_EXPORT;
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
    int i;
    struct desc_reorder_list *list = desc_reorder_list_get();
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
        // Initialization of 'Listbox'
        //
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LOOK);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DEL);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DELALL);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RETURN);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_RED);
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXPALL);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
        LISTBOX_SetFont(hItem, GUI_FONT_32_ASCII);
        LISTBOX_SetAutoScrollV(hItem, 1);
        LISTBOX_SetScrollbarWidth(hItem, 30);

        data_reorder_list_update(list);
        for (i = 0; i < list->cnt; i++) {
            hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
            LISTBOX_AddString(hItem, list->descs[i]->string);
        }

        // USER START (Optionally insert additional code for further widget initialization)
        // USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_LISTBOX_0: // Notifications sent by 'Listbox'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_LOOK: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
                i = LISTBOX_GetSel(hItem);
                if (i < list->cnt) {
                    g_diag_detail.data_idx = list->descs[i]->idx;
                    g_ui_msg.msg = UI_MSG_LOAD_DETAIL;
                    GUI_EndDialog(pMsg->hWin, 0);
                } else if (list->cnt == 0) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                    WM_Exec();
                    g_diag_ok.header = "错误";
                    g_diag_ok.str_lin1 = "没有可查看的数据";
                    g_diag_ok.str_lin2 = NULL;
                    g_diag_ok.str_lin3 = NULL;
                    diag_ok_creat();
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                } else {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                    WM_Exec();
                    g_diag_ok.header = "错误";
                    g_diag_ok.str_lin1 = "系统异常，找不到对应数据";
                    g_diag_ok.str_lin2 = NULL;
                    g_diag_ok.str_lin3 = NULL;
                    diag_ok_creat();
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                }
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
        case ID_BUTTON_DEL: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
                i = LISTBOX_GetSel(hItem);
                if (list->cnt == 0) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                    WM_Exec();
                    g_diag_ok.header = "错误";
                    g_diag_ok.str_lin1 = "没有可删除数据";
                    g_diag_ok.str_lin2 = NULL;
                    g_diag_ok.str_lin3 = NULL;
                    diag_ok_creat();
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    break;
                } else if (i >= list->cnt) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                    WM_Exec();
                    g_diag_ok.header = "错误";
                    g_diag_ok.str_lin1 = "系统异常，找不到对应数据";
                    g_diag_ok.str_lin2 = NULL;
                    g_diag_ok.str_lin3 = NULL;
                    diag_ok_creat();
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    break;
                }

                if (!list->descs[i])
                    break;

                test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                WM_Exec();
                g_diag_start.header = "数据删除";
                g_diag_start.str_lin1 = "将要删除实验数据：";
                g_diag_start.str_lin2 = list->descs[i]->string;
                g_diag_start.str_lin3 = "删除后数据不可恢复，请谨慎操作";
                g_diag_start.btn_str = "删除";
                if (diag_start_creat()) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    break;
                }

                /* I don't know why LISTBOX_DeleteItem can not work
                 * so, reload this windows to runaway this bug
                 */
                if (data_delete_one(i, list->descs[i]->idx)) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    WM_Exec();
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
                    LISTBOX_DeleteItem(hItem, i);
                    //g_ui_msg.msg = UI_MSG_LOAD_DATA;
                    //GUI_EndDialog(pMsg->hWin, 0);
                } else {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                }
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
        case ID_BUTTON_DELALL: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();

                if (list->cnt == 0) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                    WM_Exec();
                    g_diag_ok.header = "错误";
                    g_diag_ok.str_lin1 = "没有可删除数据";
                    g_diag_ok.str_lin2 = NULL;
                    g_diag_ok.str_lin3 = NULL;
                    diag_ok_creat();
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    break;
                }

                test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                WM_Exec();
                g_diag_start.header = "数据删除";
                g_diag_start.str_lin1 = "将要删除所有实验数据";
                g_diag_start.str_lin2 = "删除后数据不可恢复，请谨慎操作";
                g_diag_start.str_lin3 = NULL;
                g_diag_start.btn_str = "删除";
                if (diag_start_creat()) {
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    break;
                }

                test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                
                data_delete_all();
                
                /* I don't know why LISTBOX_DeleteItem can not work
                 * so, reload this windows to runaway this bug
                 */
                g_ui_msg.msg = UI_MSG_LOAD_DATA;
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
        case ID_BUTTON_EXPALL: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
                test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 0);
                WM_Exec();

                if (list->cnt <= 0) {
                    g_diag_ok.header = "错误";
                    g_diag_ok.str_lin1 = "没有可导出数据";
                    g_diag_ok.str_lin2 = NULL;
                    g_diag_ok.str_lin3 = NULL;
                    diag_ok_creat();
                    test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                    break;
                }

                diag_usb_creat();
                test_enable_all_items(pMsg->hWin, ID_LISTBOX_0, ID_BUTTON_DELALL, 1);
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on notification message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further notification handling)
                // USER END
            }
            break;
        case ID_BUTTON_RETURN: // Notifications sent by 'Button'
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

int ui_data_creat(void)
{
    return GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
