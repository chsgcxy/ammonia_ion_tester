#ifndef __TEST_H__
#define __TEST_H__

#include "WM.h"

#define TEST_LAST_CNT    10

#define TEST_VOLT_FMT    "%.2fmV"

struct test_data {
    float weight_sample;
    float volume_sample;
    float volt_block1;
    float volt_block2;
    float volt_blockagv;
    float volt_sample;
    float result;
};

extern int test_init(void);
extern float test_volt_get(void);
extern int test_progress(int cnt);
extern void test_enable_all_items(WM_HWIN hWin, int low, int high, int enable);
#endif
