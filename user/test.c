#include "test.h"
#include "string.h"
#include "stdio.h"
#include "ad770x.h"
#include "ads1256.h"

#define VOLT_DISCARD         (5)
#define VOLT_WINDOWS         (5)
#define VOLT_BUF_CNT         (VOLT_DISCARD * 2 + VOLT_WINDOWS)

#define FLOAT_SWAP(float1, float2)    do {\
    float temp = float1;                  \
    float1 = float2;                      \
    float2 = temp;                        \
} while(0);


int test_init(void)
{
    return 0;
}

float test_volt_get(void)
{    
    int i, j;
    float volt = 0.0;
    static float volt_buff[VOLT_BUF_CNT];

    for (i = 0; i < VOLT_BUF_CNT; i++)
        volt_buff[i] = ad7705_read();

    /* sort */
	for (i = 0; i < VOLT_BUF_CNT; i++) {
		for (j = VOLT_BUF_CNT - 1; j > i; j--) {
			if (volt_buff[j] < volt_buff[j - 1])
				FLOAT_SWAP(volt_buff[j], volt_buff[j - 1]);
        }
    }

    /* get avarage */
	for (i = VOLT_DISCARD; i < (VOLT_DISCARD + VOLT_WINDOWS); i++)
        volt += volt_buff[i];
    volt = volt / (float)VOLT_WINDOWS;

    return volt;
}
