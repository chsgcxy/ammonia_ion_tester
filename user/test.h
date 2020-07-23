#ifndef __TEST_H__
#define __TEST_H__

#include "WM.h"

#define TEST_LAST_CNT    10

#define TEST_VOLT_FMT    "%.2fmV"

struct test_data {
    float weight;
    float volume;
    float blockv1;
    float blockv2;
    float blockagv;
    float testv;
};

extern int test_init(void);
extern float test_volt_get(void);
extern int test_progress(int cnt);
extern void test_enable_all_items(WM_HWIN hWin, int low, int high, int enable);
#endif
