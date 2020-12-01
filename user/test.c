#include "test.h"
#include "string.h"
#include "stdio.h"
#include "ad770x.h"
#include "ads1256.h"
#include "ltc2400.h"

#define VOLT_DISCARD         (5)
#define VOLT_WINDOWS         (5)
#define VOLT_BUF_CNT         (VOLT_DISCARD * 2 + VOLT_WINDOWS)

#define FLOAT_SWAP(float1, float2)    do {\
    float temp = float1;                  \
    float1 = float2;                      \
    float2 = temp;                        \
} while(0);

float test_volt_get(void)
{    
    int i, j;
    float volt = 0.0;
    static float volt_buff[VOLT_BUF_CNT];

    for (i = 0; i < VOLT_BUF_CNT; i++)
        volt_buff[i] = ltc2400_read_data();

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

int test_progress(int cnt)
{
    return ((TEST_LAST_CNT - cnt) * 100 / TEST_LAST_CNT);
}

void test_enable_all_items(WM_HWIN hWin, int low, int high, int enable)
{
    WM_HWIN hItem;
    int id;

    if (enable)
        for (id = low; id <= high; id++) {
            hItem = WM_GetDialogItem(hWin, id);
            WM_EnableWindow(hItem);
        }
    else
        for (id = low; id <= high; id++) {
            hItem = WM_GetDialogItem(hWin, id);
            WM_DisableWindow(hItem);
        }
}


static int check_stat = 0;


int test_init(void)
{
    return 0;
}
