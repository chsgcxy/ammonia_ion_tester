#ifndef __TEST_H__
#define __TEST_H__

#include "WM.h"

#define TEST_LAST_CNT    10

#define TEST_VOLT_FMT    "%.2fmV"
#define TEST_CONCENT_FMT    "%.4fmg/L"

enum check_stat {
    CHECK_UNDO = 0,
    CHECK_PASS,
    CHECK_FAIL
};

extern int test_init(void);
extern float test_volt_get(void);
extern int test_progress(int cnt);
extern void test_enable_all_items(WM_HWIN hWin, int low, int high, int enable);

#endif
