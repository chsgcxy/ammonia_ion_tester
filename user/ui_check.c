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
#include "test.h"
#include "ad770x.h"
#include "ui_diag.h"
#include "ltc2400.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)

#define ID_TEXT_GUIDE0  (GUI_ID_USER + 0x01)
#define ID_TEXT_GUIDE1  (GUI_ID_USER + 0x02)
#define ID_TEXT_GUIDE2  (GUI_ID_USER + 0x03)
#define ID_LISTVIEW_0   (GUI_ID_USER + 0x04)

#define ID_TEXT_VOLT_INFO    (GUI_ID_USER + 0x05)
#define ID_TEXT_VOLT_VALUE   (GUI_ID_USER + 0x06)

#define ID_TEXT_PROGRESS     (GUI_ID_USER + 0x07)
#define ID_PROGBAR_0         (GUI_ID_USER + 0x08)

#define ID_BUTTON_01mg       (GUI_ID_USER + 0x09)
#define ID_BUTTON_001mg      (GUI_ID_USER + 0x0A)
#define ID_BUTTON_0001mg     (GUI_ID_USER + 0x0B)

#define ID_BUTTON_RETURN     (GUI_ID_USER + 0x0C)

#define ID_TEXT_RESULT_INFO  (GUI_ID_USER + 0x0D)
#define ID_TEXT_RESULT_VALUE (GUI_ID_USER + 0x0E)

#define ID_TEXT_HEADER       (GUI_ID_USER + 0x0F)

extern const GUI_FONT GUI_FontHZ_yahei_20;
extern const GUI_FONT GUI_FontHZ_yahei_16;
// USER END

enum run_flags {
    RUN_IDLE = 0x0,
    RUN_01MG,
    RUN_001MG,
    RUN_0001MG
};

struct check_data {
    float volt_01;
    float volt_001;
    float volt_0001;
    int done_01;
    int done_001;
    int done_0001;
    float diff_01_001;
    float diff_001_0001;
};

static WM_HWIN wait_diag_handle;
static int run_flag = RUN_IDLE;
static int run_cnt = TEST_LAST_CNT;
static float volt = 0.0;
static struct check_data g_check_data;
static char strbuf[32];
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
    {TEXT_CreateIndirect, "电极校对", ID_TEXT_HEADER, 300, 2, 200, 40, 0, 0x0, 0},

    { TEXT_CreateIndirect, "guide0", ID_TEXT_GUIDE0, 5, 55, 150, 25, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "guide1", ID_TEXT_GUIDE1, 5, 90, 600, 25, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "guide2", ID_TEXT_GUIDE2, 5, 125, 600, 25, 0, 0x0, 0 },

    {LISTVIEW_CreateIndirect, "lv", ID_LISTVIEW_0, 5, 160, 530, 247, 0, 0x0, 0},

    {TEXT_CreateIndirect, "实时电位:", ID_TEXT_VOLT_INFO, 5, 410, 100, 30, 0, 0x0, 0},
    {TEXT_CreateIndirect, "00.00mV", ID_TEXT_VOLT_VALUE, 120, 415, 100, 30, 0, 0x0, 0},
    
    {TEXT_CreateIndirect, "结果汇总:", ID_TEXT_RESULT_INFO, 290, 410, 100, 30, 0, 0x0, 0},
    {TEXT_CreateIndirect, "未测量", ID_TEXT_RESULT_VALUE, 410, 410, 100, 30, 0, 0x0, 0},

    {TEXT_CreateIndirect, "测量进度", ID_TEXT_PROGRESS, 5, 442, 120, 30, 0, 0x0, 0},
    {PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 120, 445, 415, 25, 0, 0x0, 0},

    {BUTTON_CreateIndirect, "开始测量", ID_BUTTON_0001mg, 560, 205, 200, 40, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "开始测量", ID_BUTTON_001mg, 560, 285, 200, 40, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "开始测量", ID_BUTTON_01mg, 560, 365, 200, 40, 0, 0x0, 0},

    {BUTTON_CreateIndirect, "返回", ID_BUTTON_RETURN, 690, 5, 100, 40, 0, 0x0, 0}
};

// USER START (Optionally insert additional static code)
static int check_volt_diff(float diff)
{
    if (diff >= -60.0 && diff <= -40.0)
        return 0;
    else
        return 1;
}

// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN      hItem;
    int     NCode;
    int     Id;
    int i;
    HEADER_Handle hHeader;
    int     result;
    struct test_data *td;

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Framewin'
        //
        hItem = pMsg->hWin;
        WM_CreateTimer(hItem, 0, 1000, 0);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_HEADER);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_20);
        TEXT_SetTextColor(hItem, GUI_DARKBLUE);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GUIDE0);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "校对方法:");      

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GUIDE1);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "取10ml对应浓度氨离子标准溶液至100ml容量瓶中，稀释");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_GUIDE2);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);
        TEXT_SetText(hItem, "至刻度，摇匀，点击对应浓度右侧测量按钮进行测量");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        LISTVIEW_SetRowHeight(hItem, 40);
        LISTVIEW_SetBkColor(hItem, 0, GUI_GRAY);
        LISTVIEW_SetGridVis(hItem, 1);
        LISTVIEW_SetFont(hItem, GUI_FONT_24_ASCII);
        for (i = 0; i < 5; i++)
            LISTVIEW_AddRow(hItem, NULL);

        hHeader = LISTVIEW_GetHeader(hItem);
        HEADER_SetFont(hHeader, &GUI_FontHZ_yahei_16);
        HEADER_SetTextColor(hHeader, GUI_BLACK_33);
        HEADER_SetHeight(hHeader, 45);
        HEADER_SetBkColor(hHeader, GUI_GREEN);
        LISTVIEW_AddColumn(hItem, 170, "溶液浓度", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 120, "电极电位", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 120, "电极斜率", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 120, "测试结果", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_SetTextColor(hItem, 0, GUI_BLACK_33);
        LISTVIEW_SetFont(hItem, &GUI_FontHZ_yahei_16);
        LISTVIEW_SetItemText(hItem, 0, 0, "0.001mg/ml");
        LISTVIEW_SetItemText(hItem, 0, 2, "0.01mg/ml");
        LISTVIEW_SetItemText(hItem, 0, 4, "0.1mg/ml");
        LISTVIEW_SetItemText(hItem, 1, 0, "----");
        LISTVIEW_SetItemText(hItem, 1, 2, "----");
        LISTVIEW_SetItemText(hItem, 1, 4, "----");
        LISTVIEW_SetItemText(hItem, 2, 1, "----");
        LISTVIEW_SetItemText(hItem, 2, 3, "----");
        LISTVIEW_SetItemText(hItem, 3, 1, "----");
        LISTVIEW_SetItemText(hItem, 3, 3, "----");

        // realtime volt
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VOLT_INFO);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VOLT_VALUE);
        TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
        TEXT_SetTextColor(hItem, GUI_GREEN);

        // result
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RESULT_INFO);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RESULT_VALUE);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_RED);

        // test progress
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PROGRESS);
        TEXT_SetFont(hItem, &GUI_FontHZ_yahei_16);
        TEXT_SetTextColor(hItem, GUI_BLACK_33);        

        hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
        PROGBAR_SetFont(hItem, GUI_FONT_24_ASCII);
        PROGBAR_SetBarColor(hItem, 0, GUI_GREEN);
        PROGBAR_SetSkinClassic(hItem);
        PROGBAR_SetMinMax(hItem, 0, 100);
        PROGBAR_SetValue(hItem, 100);

        // button
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_01mg);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_001mg);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0001mg);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKBLUE);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RETURN);
        BUTTON_SetFont(hItem, &GUI_FontHZ_yahei_16);
        BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_DARKRED);

        g_check_data.volt_01 = 0.0;
        g_check_data.volt_001 = 0.0;
        g_check_data.volt_0001 = 0.0;
        g_check_data.done_01 = 0;
        g_check_data.done_001 = 0;
        g_check_data.done_0001 = 0;
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_BUTTON_RETURN:
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                printf("run main menu\r\n");
                g_ui_msg.msg = UI_MSG_LOAD_MENU;
                GUI_EndDialog(pMsg->hWin, 0);
                break;
            default:
                break;
            }
            break;
        case ID_BUTTON_01mg:
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 0);
                WM_Exec();
                g_diag_start.header = "电极校对";
                g_diag_start.str_lin1 = "取10mL 0.1mg/mL氨离子标准溶液至100mL";
                g_diag_start.str_lin2 = "容量瓶中，稀释至刻度，并启动磁力搅拌器";
                g_diag_start.str_lin3 = "准备好后，点击开始实验以启动检测";
                g_diag_start.btn_str = "开始实验";
                result = diag_start_creat();
                if (result)
                    test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 1);
                else {
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
                    PROGBAR_SetValue(hItem, 0);
                    run_cnt = TEST_LAST_CNT;
                    run_flag = RUN_01MG;
                    g_diag_wait.header = "提示";
                    g_diag_wait.str_lin1 = "正在进行电极校对测量";
                    g_diag_wait.str_lin2 = NULL;
                    g_diag_wait.str_lin3 = "请稍后......";
                    wait_diag_handle = diag_wait_creat(-1);
                    GUI_ExecCreatedDialog(wait_diag_handle);
                }
                break;
            default:
                break;
            }
            break;
        case ID_BUTTON_001mg:
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 0);
                WM_Exec();
                g_diag_start.header = "电极校对";
                g_diag_start.str_lin1 = "取10mL 0.01mg/mL氨离子标准溶液至100mL";
                g_diag_start.str_lin2 = "容量瓶中，稀释至刻度，并启动磁力搅拌器";
                g_diag_start.str_lin3 = "准备好后,点击开始实验以启动检测";
                g_diag_start.btn_str = "开始实验";
                result = diag_start_creat();
                if (result)
                    test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 1);
                else {
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
                    PROGBAR_SetValue(hItem, 0);
                    run_cnt = TEST_LAST_CNT;
                    run_flag = RUN_001MG;
                    g_diag_wait.header = "提示";
                    g_diag_wait.str_lin1 = "正在进行电极校对测量";
                    g_diag_wait.str_lin2 = NULL;
                    g_diag_wait.str_lin3 = "请稍后......";
                    wait_diag_handle = diag_wait_creat(-1);
                    GUI_ExecCreatedDialog(wait_diag_handle);
                }
                break;
            default:
                break;
            }
            break;
        case ID_BUTTON_0001mg:
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                beep_clicked();
                test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 0);
                WM_Exec();
                g_diag_start.header = "电极校对";
                g_diag_start.str_lin1 = "取10mL 0.001mg/mL氨离子标准溶液至100mL";
                g_diag_start.str_lin2 = "容量瓶中，稀释至刻度，并启动磁力搅拌器";
                g_diag_start.str_lin3 = "准备好后,点击开始实验以启动检测";
                g_diag_start.btn_str = "开始实验";
                result = diag_start_creat();
                if (result)
                    test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 1);
                else {
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
                    PROGBAR_SetValue(hItem, 0);
                    run_cnt = TEST_LAST_CNT;
                    run_flag = RUN_0001MG;
                    g_diag_wait.header = "提示";
                    g_diag_wait.str_lin1 = "正在进行电极校对测量";
                    g_diag_wait.str_lin2 = NULL;
                    g_diag_wait.str_lin3 = "请稍后......";
                    wait_diag_handle = diag_wait_creat(-1);
                    GUI_ExecCreatedDialog(wait_diag_handle);
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    case WM_TIMER:
        if (run_flag != RUN_IDLE) {
            run_cnt--;
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
            PROGBAR_SetValue(hItem, test_progress(run_cnt));
            volt = ltc2400_read_data(); // take some time
            printf("%s: read volt = %f\r\n", __FUNCTION__, volt);
            if (run_cnt <= 0) {
                //volt = ltc2400_read_data(); // take some time
                //printf("%s: read volt = %f\r\n", __FUNCTION__, volt);
                sprintf(strbuf, TEST_VOLT_FMT, volt);
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VOLT_VALUE);
                TEXT_SetText(hItem, strbuf);
                WM_Exec(); // update progress after got volt
                beep_finished();

                /* update listview */
                hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
                switch (run_flag) {
                case RUN_01MG:
                    g_check_data.volt_01 = volt;
                    g_check_data.done_01++;
                    LISTVIEW_SetItemText(hItem, 1, 4, strbuf);
                    break;
                case RUN_001MG:
                    g_check_data.volt_001 = volt;
                    g_check_data.done_001++;
                    LISTVIEW_SetItemText(hItem, 1, 2, strbuf);
                    break;
                case RUN_0001MG:
                    g_check_data.volt_0001 = volt;
                    g_check_data.done_0001++;
                    LISTVIEW_SetItemText(hItem, 1, 0, strbuf);
                    break;
                default:
                    break;
                }

                /* auto update result after run */
                if (g_check_data.done_01 &&
                    g_check_data.done_001) {
                    g_check_data.diff_01_001 = g_check_data.volt_01 -
                        g_check_data.volt_001;
                    sprintf(strbuf, TEST_VOLT_FMT, g_check_data.diff_01_001);
                    LISTVIEW_SetItemText(hItem, 2, 3, strbuf);
                    if (check_volt_diff(g_check_data.diff_01_001))
                        LISTVIEW_SetItemText(hItem, 3, 3, "失败");
                    else {}
                        LISTVIEW_SetItemText(hItem, 3, 3, "通过");
                }

                if (g_check_data.done_001 &&
                    g_check_data.done_0001) {
                    g_check_data.diff_001_0001 = g_check_data.volt_001 -
                        g_check_data.volt_0001;
                    sprintf(strbuf, TEST_VOLT_FMT, g_check_data.diff_001_0001);
                    LISTVIEW_SetItemText(hItem, 2, 1, strbuf);
                    if (check_volt_diff(g_check_data.diff_001_0001))
                        LISTVIEW_SetItemText(hItem, 3, 1, "失败");
                    else
                        LISTVIEW_SetItemText(hItem, 3, 1, "通过");
                }

                GUI_EndDialog(wait_diag_handle, 0);

                /* Analyze the final result */
                if (g_check_data.done_01 &&
                    g_check_data.done_001 &&
                    g_check_data.done_0001) {
                    td = data_obj();
                    td->volt_0001 = g_check_data.volt_0001;
                    td->volt_001 = g_check_data.volt_001;
                    td->volt_01 = g_check_data.volt_01;
                    data_calc_coeff(td);

                    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RESULT_VALUE);
                    g_diag_ok.header = "结果";
                    g_diag_ok.str_lin1 = "电极校对结果为：";
                    if (!check_volt_diff(g_check_data.diff_001_0001) &&
                            !check_volt_diff(g_check_data.diff_001_0001)) {
                        TEXT_SetText(hItem, "通过");
                        g_diag_ok.str_lin2 = "通过";
                        g_diag_ok.str_lin3 = "满足实验要求，可以进行实验";
                        td->elect_stat = CHECK_PASS;
                    } else {
                        TEXT_SetText(hItem, "未通过");
                        g_diag_ok.str_lin2 = "未通过";
                        g_diag_ok.str_lin3 = "请及时检查或更换电极";
                        td->elect_stat = CHECK_FAIL;
                    }
                    diag_ok_creat();
                }
                test_enable_all_items(pMsg->hWin, ID_TEXT_GUIDE0, ID_TEXT_HEADER, 1);
                run_flag = RUN_IDLE;
            } else {
                /* on progress not finished */
                //volt = ad7705_read();
                sprintf(strbuf, TEST_VOLT_FMT, volt);
                hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VOLT_VALUE);
                TEXT_SetText(hItem, strbuf);
                WM_Exec();
                //beep_clicked(); // update progress after update UI
            }
        } else {
            /* idle status */
            //volt = ad7705_read();
            sprintf(strbuf, TEST_VOLT_FMT, volt);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VOLT_VALUE);
            TEXT_SetText(hItem, strbuf);
        }

        WM_RestartTimer(pMsg->Data.v, 1000);
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
int ui_check_creat(void)
{
    return GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
