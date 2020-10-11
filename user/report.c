#include "rd-tprinter.h"
#include "report.h"
#include "string.h"
#include "stdio.h"
/** @notes:
 * Chinese characters are not used directly because it depends on the encoding of the editor.
 * use amo tools function ascii2hex to creat this string.
 * */
#define STRING_DATE             "测试日期:"
#define STRING_RESULT           "氨离子含量:"
#define STRING_SAMPLE_VOLT      "试样电极电位:"
#define STRING_BLOCKAVG_VOLT    "空白实验平均电极电位:"
#define STRING_BLOCK2_VOLT      "空白实验2电极电位:"
#define STRING_BLOCK1_VOLT      "空白实验1电极电位:"
#define STRING_SAMPLE_VOLUME    "试样体积:"
#define STRING_SAMPLE_WEIGHT    "试样质量:"
#define STRING_INDEX            "实验编号:"
#define STRING_TITLE            "氨离子检测报告"
#define STRING_ECECT_PASS       "电极状态: 正常"
#define STRING_ECECT_FAIL       "电极状态: 异常"

uint8_t char_space = 0x20;
uint8_t char_colon = 0x3A;
uint8_t char_percent = 0x25;

static struct tprinter *report_printer;

int report_init(struct tprinter *printer)
{
    report_printer = printer;
    return 0;
}

int report_show(struct test_data *td)
{
    char buf[64];
    
    tprinter_prepare(report_printer);
    tprinter_zoomin(report_printer, 1, 1);

    tprinter_send(report_printer, STRING_DATE, sizeof(STRING_DATE));
    sprintf(buf, "20%02d年%02d月%02d日  %02d:%02d",
        td->year, td->month, td->day, td->hour, td->minute);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_RESULT, sizeof(STRING_RESULT));
    sprintf(buf, "%.2fmg/kg", td->result);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_SAMPLE_VOLT, sizeof(STRING_SAMPLE_VOLT));
    sprintf(buf, "%.2fmV", td->concent_sample);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_BLOCKAVG_VOLT, sizeof(STRING_BLOCKAVG_VOLT));
    sprintf(buf, "%.2fmV", td->concent_blockave);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_BLOCK2_VOLT, sizeof(STRING_BLOCK2_VOLT));
    sprintf(buf, "%.2fmV", td->concent_block2);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_BLOCK1_VOLT, sizeof(STRING_BLOCK1_VOLT));
    sprintf(buf, "%.2fmV", td->concent_block1);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_SAMPLE_VOLUME, sizeof(STRING_SAMPLE_VOLUME));
    sprintf(buf, "%.2fmL", td->volume_sample);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_SAMPLE_WEIGHT, sizeof(STRING_SAMPLE_WEIGHT));
    sprintf(buf, "%.3fg", td->weight_sample);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);

    switch (td->elect_stat) {
    case CHECK_PASS:
        tprinter_send(report_printer, STRING_ECECT_PASS, sizeof(STRING_ECECT_PASS));
        break;
    case CHECK_UNDO:
    case CHECK_FAIL:
    default:
        tprinter_send(report_printer, STRING_ECECT_FAIL, sizeof(STRING_ECECT_FAIL));
        break;
    }
    tprinter_newline(report_printer);

    tprinter_send(report_printer, STRING_INDEX, sizeof(STRING_INDEX));
    sprintf(buf, "%03d", td->index);
    tprinter_send(report_printer, (uint8_t *)buf, strlen(buf));
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);

    tprinter_send(report_printer, &char_space, 1);
    tprinter_zoomin(report_printer, 2, 2);
    tprinter_send(report_printer, STRING_TITLE, sizeof(STRING_TITLE));
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);
    tprinter_newline(report_printer);

    return 0;
}
