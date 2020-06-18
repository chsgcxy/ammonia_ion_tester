#ifndef __TEST_H__
#define __TEST_H__

#define TEST_LAST_CNT    10

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

#endif
