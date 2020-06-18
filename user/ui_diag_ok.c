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
#include "beep.h"
#include "main.h"
#include "string.h"
#include "stepmotor.h"
#include "ui_diag.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_IMAGE_0     (GUI_ID_USER + 0x01)
#define ID_TEXT_HEADER (GUI_ID_USER + 0x02)
#define ID_TEXT_lINE1  (GUI_ID_USER + 0x03)
#define ID_TEXT_lINE2  (GUI_ID_USER + 0x04)
#define ID_TEXT_lINE3  (GUI_ID_USER + 0x05)
#define ID_BUTTON_OK   (GUI_ID_USER + 0x06)
// USER START (Optionally insert additional defines)
// USER END
extern const GUI_FONT GUI_FontHZ_yahei_16;
extern const GUI_BITMAP bminfor_32px;

struct diag_ok g_diag_ok;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 150, 120, 500, 240, 0, 0x0, 0},
    {IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 11, 10, 32, 32, 0, 0, 0},
    {TEXT_CreateIndirect, "header", ID_TEXT_HEADER, 50, 5, 100, 25, 0, 0x0, 0},
    {TEXT_CreateIndirect, "line1", ID_TEXT_lINE1, 5, 45, 480, 25, 0, 0x0, 0},
    {TEXT_CreateIndirect, "line2", ID_TEXT_lINE2, 5, 75, 480, 25, 0, 0x0, 0},
    {TEXT_CreateIndirect, "line3", ID_TEXT_lINE3, 5, 105, 480, 25, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "ȷ��", ID_BUTTON_OK, 180, 190, 135, 40, 0, 0x0, 0},
    // USER START (Optionally insert additional widgets)
    // USER END
};

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
        // Initialization of 'Window'
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_DARKBLUE);
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_HEADER);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetText(hItem, g_diag_ok.header);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        IMAGE_SetBitmap(hItem, &bminfor_32px);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_lINE1);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        if (g_diag_ok.str_lin1)
            TEXT_SetText(hItem, g_diag_ok.str_lin1);
        else
            WM_HideWindow(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_lINE2);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        if (g_diag_ok.str_lin2)
            TEXT_SetText(hItem, g_diag_ok.str_lin2);
        else
            WM_HideWindow(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_lINE3);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        if (g_diag_ok.str_lin3)
            TEXT_SetText(hItem, g_diag_ok.str_lin3);
        else
            WM_HideWindow(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);

    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_BUTTON_OK: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on notification message)
                beep_clicked();
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
*       CreateWindow
*/
int diag_ok_creat(void)
{
    return GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
