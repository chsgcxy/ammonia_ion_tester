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
#include "main.h"
#include "beep.h"
#include "string.h"
#include "experiment.h"
#include "report.h"
#include "stdio.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0          (GUI_ID_USER + 0x00)
#define ID_TEXT_0            (GUI_ID_USER + 0x01)
#define ID_TEXT_1            (GUI_ID_USER + 0x03)
#define ID_TEXT_2            (GUI_ID_USER + 0x04)
#define ID_TEXT_3            (GUI_ID_USER + 0x05)
#define ID_TEXT_4            (GUI_ID_USER + 0x06)
#define ID_BUTTON_0          (GUI_ID_USER + 0x07)
#define ID_BUTTON_1          (GUI_ID_USER + 0x08)
#define ID_BUTTON_2          (GUI_ID_USER + 0x09)
#define ID_TEXT_PPM          (GUI_ID_USER + 0x0A)
#define ID_TEXT_PPM_VALUE    (GUI_ID_USER + 0x0B)
// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
extern const GUI_FONT GUI_FontHZ_yahei_16;
// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 151, 119, 500, 240, 0, 0x0, 0},
    {TEXT_CreateIndirect, "实验结果", ID_TEXT_0, 182, 12, 133, 25, 0, 0x64, 0},
    
    {TEXT_CreateIndirect, "Text", ID_TEXT_1, 10, 60, 250, 25, 0, 0x64, 0},
    {TEXT_CreateIndirect, "Text", ID_TEXT_2, 275, 60, 150, 25, 0, 0x64, 0},
    
    {TEXT_CreateIndirect, "Text", ID_TEXT_3, 10, 100, 250, 25, 0, 0x64, 0},
    {TEXT_CreateIndirect, "Text", ID_TEXT_4, 275, 100, 150, 25, 0, 0x64, 0},
    
    {TEXT_CreateIndirect, "PPM", ID_TEXT_PPM, 10, 140, 250, 25, 0, 0x64, 0},
    {TEXT_CreateIndirect, "0.02mol/L", ID_TEXT_PPM_VALUE, 275, 140, 150, 25, 0, 0x64, 0},

    {BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 10, 190, 120, 40, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 187, 190, 120, 40, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 368, 190, 120, 40, 0, 0x0, 0},
    // USER START (Optionally insert additional widgets)
    // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static struct exper_stat *stat;
// USER START (Optionally insert additional static code)
// USER END

static void ctrl_all_items(WM_HWIN hWin, int enable)
{
    WM_HWIN hItem;
    int id;

    for (id = ID_BUTTON_0; id <= ID_BUTTON_2; id++) {
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
    char buf[16];
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
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        switch (stat->stat) {
        case EXPER_STAT_DROPPER_FINISHED:
            TEXT_SetText(hItem, "滴定液用量");
            break;
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            TEXT_SetText(hItem, "10mL时AgNO3用量");
            break;
        default:
            TEXT_SetText(hItem, "AgNO3用量");
            break;
        }
        //
        // Initialization of 'Text'
        //
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
            sprintf(buf, "%.2fmL", stat->data.agno3_agno3_used);
            break;
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            sprintf(buf, "%.2fmL", stat->data.block_agno3_used);
            break;
        case EXPER_STAT_CL_FINISHED:
        case EXPER_STAT_STAND_FINISHED:
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            sprintf(buf, "%.2fmL", stat->data.cl_agno3_used);
            break;
        default:
            break;
        }
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        TEXT_SetFont(hItem, GUI_FONT_24_1);
        TEXT_SetText(hItem, buf);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
            TEXT_SetText(hItem, "AgNO3浓度");
            break;
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
            WM_HideWindow(hItem);
            break;
        case EXPER_STAT_CL_FINISHED:
            TEXT_SetText(hItem, "水泥氯离子质量分数");
            break;
        case EXPER_STAT_STAND_FINISHED:
            TEXT_SetText(hItem, "氯离子浓度");
            break;
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            TEXT_SetText(hItem, "20mL时AgNO3用量");
            break;
        default:
            break;
        }
        //
        // Initialization of 'Text'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
        TEXT_SetFont(hItem, GUI_FONT_24_1);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
            sprintf(buf, "%.4fmol/L", stat->data.agno3_dosage);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
            WM_HideWindow(hItem);
            break;
        case EXPER_STAT_CL_FINISHED:
            sprintf(buf, "%.3f%%", stat->data.cl_percentage);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_STAND_FINISHED:
            sprintf(buf, "%fmol/L", stat->data.cl_dosage);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
            sprintf(buf, "%.2fmL", stat->data.agno3_agno3_used2);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            sprintf(buf, "%.2fmL", stat->data.block_agno3_used2);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            sprintf(buf, "%.2fmL", stat->data.cl_agno3_used2);
            TEXT_SetText(hItem, buf);
            break;
        default:
            break;
        }

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PPM);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_CL_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            WM_HideWindow(hItem);
            break;
        case EXPER_STAT_STAND_FINISHED:
            break;
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
            TEXT_SetText(hItem, "AgNO3浓度");
            break;
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            TEXT_SetText(hItem, "试样氯离子质量分数");
            break;
        default:
            break;
        }

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PPM_VALUE);
        TEXT_SetFont(hItem, GUI_FONT_24_1);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_CL_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            WM_HideWindow(hItem);
            break;
        case EXPER_STAT_STAND_FINISHED:
            sprintf(buf, "%.1f", stat->data.ppm);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
            sprintf(buf, "%.4fmol/L", stat->data.agno3_dosage);
            TEXT_SetText(hItem, buf);
            break;
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            sprintf(buf, "%.3f%%", stat->data.cl_percentage);
            TEXT_SetText(hItem, buf);
            break;
        default:
            break;
        }

        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            WM_HideWindow(hItem);
            break;
        case EXPER_STAT_CL_FINISHED:
        case EXPER_STAT_STAND_FINISHED:
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
            BUTTON_SetText(hItem, "打印");
            break;
        default:
            break;
        }
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            BUTTON_SetText(hItem, "确定");
            break;
        case EXPER_STAT_CL_FINISHED:
        case EXPER_STAT_STAND_FINISHED:
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            WM_HideWindow(hItem);
            break;
        default:
            break;
        }
        //
        // Initialization of 'Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, 0, GUI_BLUE);
        switch (stat->stat) {
        case EXPER_STAT_AGNO3_FINISHED:
        case EXPER_STAT_BLOCK_FINISHED:
        case EXPER_STAT_DROPPER_FINISHED:
        case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
        case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
            WM_HideWindow(hItem);
            break;
        case EXPER_STAT_CL_FINISHED:
        case EXPER_STAT_STAND_FINISHED:
        case EXPER_STAT_CL_EXTEST_FINISHED2:
            BUTTON_SetText(hItem, "返回");
            break;
        default:
            break;
        }
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
                beep_clicked();
                switch (stat->stat) {
                case EXPER_STAT_CL_FINISHED:
                    ctrl_all_items(pMsg->hWin, 0);
                    WM_Exec();
                    exper_print_report(0);
                    ctrl_all_items(pMsg->hWin, 1);
                    break;
                case EXPER_STAT_STAND_FINISHED:
                    ctrl_all_items(pMsg->hWin, 0);
                    WM_Exec();
                    exper_print_report(1);
                    ctrl_all_items(pMsg->hWin, 1);
                    break;
                case EXPER_STAT_CL_EXTEST_FINISHED2:
                    ctrl_all_items(pMsg->hWin, 0);
                    WM_Exec();
                    exper_print_report(3);
                    ctrl_all_items(pMsg->hWin, 1);
                    break;
                default:
                    break;
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
        case ID_BUTTON_1: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                // USER START (Optionally insert code for reacting on notification message)
                switch (stat->stat) {
                case EXPER_STAT_AGNO3_FINISHED:
                case EXPER_STAT_BLOCK_FINISHED:
                case EXPER_STAT_DROPPER_FINISHED:
                case EXPER_STAT_AGNO3_EXTEST_FINISHED2:
                case EXPER_STAT_BLOCK_EXTEST_FINISHED2:
                    GUI_EndDialog(pMsg->hWin, 0);
                    break;
                default:
                    break;
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
        case ID_BUTTON_2: // Notifications sent by 'Button'
            switch (NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                GUI_EndDialog(pMsg->hWin, 0);
                // USER START (Optionally insert code for reacting on notification message)
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
int diag_res_creat(struct exper_stat *es)
{
    stat = es;
    return GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
