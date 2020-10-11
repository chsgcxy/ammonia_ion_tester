#ifndef __DATA_H__
#define __DATA_H__

#include "stm32f2xx.h"
#include "test.h"

#define DATA_DEBUG    printf

#define DATA_MAX_NUM         100

struct test_data {
    /* fill while save to flash */
    uint8_t magic;
    uint8_t crc;
    uint16_t index;
    uint8_t valid;

    /* fill while creat */
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    
    uint8_t placeholder1;
    uint8_t elect_stat;

    /**
     * ���� volt_01 Ϊ 0.1mg/mL�ı�׼��Һ��ȡ10mL��ϡ����100mL
     * ��ʵ��Ũ��Ϊ 10mg/L, ͬ�� volt_001 ��Ӧ 1mg/L
     * volt_0001 ��Ӧ 0.1mg/mL
     * �����ڼ���ʱ��Ҫ���һ�����Ժ�����y = ax + b,
     * ���� x Ϊ��ӦĳһŨ�ȵĵ�ѹֵ����λΪ mV
     * ���� y Ϊ�õ�ѹֵ��Ӧ��Ũ��ֵ��Ϊ�˸��ü��㣬
     * �����޸�Ũ�ȵĵ�λ���޸�Ϊ ug/L, ��ô
     * volt_01 Ũ��Ϊ 10000ug/L
     * volt_001 Ũ��Ϊ 1000ug/L
     * volt_0001 Ũ��Ϊ 100ug/L
     * 
     */
    float volt_01;
    float volt_001;
    float volt_0001;

    float weight_sample;
    float volume_sample;
    float concent_block1;
    float concent_block2;
    float concent_blockave;
    float concent_sample;
    float result;
};

struct data_desc {
    char string[29];
    uint8_t valid;
    uint16_t idx;
};

/**
 * only record valid test_data, ordered by time
 * for fast foreach
 */
struct desc_reorder_list {
    int cnt;
    struct data_desc* descs[DATA_MAX_NUM];
};

struct data_usb_cmd {
#define USB_INVALID       0xA0    
#define USB_READY         0xA1
#define USB_EXPORT        0xA2
#define USB_ERROR         0xA3
#define USB_QUIT          0xA4
#define USB_MKDIR         0xA5    
    int cmd;
};

extern int data_init(void);
extern int data_num_get(void);

extern int data_save(struct test_data *stream);
extern int data_read(struct test_data *stream, int idx);
extern int data_delete_one(int desc_idx, int data_idx);
extern int data_delete_all(void);

extern struct data_desc *data_desc_get(int index);
extern struct test_data *data_obj(void);

extern int data_usb_detect(void);
extern int data_file_export(struct desc_reorder_list *list);
extern int data_file_mkdir(void);
extern void data_add_timestamp(struct test_data *td);

extern struct desc_reorder_list *desc_reorder_list_get(void);
extern int data_reorder_list_update(struct desc_reorder_list *list);

extern void data_calc_coeff(struct test_data *td);
extern float data_calc_concentration(float volt);

#endif
