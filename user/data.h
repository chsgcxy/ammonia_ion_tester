#ifndef __DATA_H__
#define __DATA_H__

#include "stm32f2xx.h"
#include "test.h"

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
    uint8_t placeholder0;
    uint8_t placeholder1;

    float weight_sample;
    float volume_sample;
    float volt_block1;
    float volt_block2;
    float volt_blockagv;
    float volt_sample;
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

#endif
