#include "data.h"
#include "w25xxx.h"
#include "stdio.h"
#include "main.h"
#include "diskio.h"
#include "string.h"

#define DATA_HEADER_SECTOR   W25X20_BLOCK_TO_SECTOR(1)
#define DATA_START_SECTOR    W25X20_BLOCK_TO_SECTOR(2)
#define DATA_HEADER_MAGIC    0x56
#define DATA_MAGIC           0xA6

struct data_header {
    uint8_t magic;
    uint8_t crc;
    uint16_t start_sector;
    uint16_t next_sector;
    uint16_t cnt;
};

/* a tmp data for flash read and write */
static struct test_data data_tmp;

static const uint8_t crc_table[] = {
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,
    0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,
    0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,
    0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,
    0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,
    0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,
    0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,
    0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,
    0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,
    0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,
    0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,
    0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,
    0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,
    0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,
    0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,
    0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,
    0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};

static uint8_t crc8(uint8_t *buf, int len) 
{
    uint8_t crc = 0x00;

    while (len--)
        crc = crc_table[crc ^ *buf++];
    return crc;
}

static void data_flash_read(uint8_t *buf, u32 sector, int len)
{
    w25xxx_read_sector(buf, sector, len);
}

static void data_flash_write(uint8_t *buf, u32 sector, int len)
{
    w25xxx_write_sector(buf, sector, len);
}

static struct data_header *data_header_obj(void)
{
    static struct data_header hdr;
    return &hdr;
}

struct test_data *data_obj(void)
{
    static struct test_data td;
    return &td;
}

int data_header_init(struct data_header *hdr)
{
    //uint8_t crc;
    data_flash_read((uint8_t *)hdr,
        DATA_HEADER_SECTOR, sizeof(struct data_header));
    if (hdr->magic != DATA_HEADER_MAGIC) {
        printf("%s: data config bad magic\r\n", __FUNCTION__);
        hdr->magic = DATA_HEADER_MAGIC;
        hdr->start_sector = DATA_START_SECTOR;
        hdr->next_sector = DATA_START_SECTOR;
        hdr->cnt = 0;
        return 1;
    }

#if 0
    crc = crc8(&hdr->crc_start, sizeof(struct data_header) - 2);
    if (hdr->crc != crc) {
        printf("%s: data config bad crc\r\n", __FUNCTION__);
        return -1;
    }    
#endif
    return 0;
}

static void data_header_save(struct data_header *hdr)
{
    hdr->crc = crc8((uint8_t *)(&hdr->start_sector),
        sizeof(struct data_header) - 2);
    data_flash_write((uint8_t *)hdr, DATA_HEADER_SECTOR,
        sizeof(struct data_header));
}

/** 
 * read data from flash is too too too slow
 * so build a data table to save data to mem
 * and data table must init when system init
 */
struct data_desc *data_desc_get(int index)
{
    static struct data_desc desc_table[DATA_MAX_NUM];
    DATA_DEBUG("%s: get desc index %d\r\n", __FUNCTION__, index);
    if (index < DATA_MAX_NUM)
        return &desc_table[index];
    else
        return NULL;
}

static void data_desc_update(int idx, const struct test_data *td)
{
    struct data_desc *desc = data_desc_get(idx);

    DATA_DEBUG("%s: desc.index %d, data.valid %d, data.index %d\r\n",
        __FUNCTION__, idx, td->valid, td->index);

    if (td->magic == DATA_MAGIC) {
        desc->idx = td->index;
        desc->valid = td->valid;
    } else
        desc->valid = 0;

    if (desc->valid)
        sprintf(desc->string, "data-%03d-20%02d%02d%02d_%02d%02d",
            td->index, td->year, td->month,
            td->day, td->hour, td->minute);            
}

int data_save(struct test_data *stream)
{
    struct data_header *hdr = data_header_obj();

    stream->magic = DATA_MAGIC;
    stream->index = hdr->next_sector - hdr->start_sector;
    DATA_DEBUG("%s: data index %d\r\n", __FUNCTION__, stream->index);
    stream->valid = 1;
    stream->crc = crc8((uint8_t *)(&stream->index),
        sizeof(struct test_data) - 2);
    data_flash_write((uint8_t *)stream,
        hdr->next_sector, sizeof(struct test_data));
 
    if (hdr->cnt < DATA_MAX_NUM)
        hdr->cnt++;

    data_desc_update(stream->index, stream);

    if (hdr->next_sector - hdr->start_sector > DATA_MAX_NUM)
        hdr->next_sector = hdr->start_sector;
    else
        hdr->next_sector++;
        
    data_header_save(hdr);

    return stream->index;
}

int data_delete_one(int desc_idx, int data_idx)
{
    struct data_header *hdr = data_header_obj();

    DATA_DEBUG("%s: delete desc %d data %d total %d\r\n",
        __FUNCTION__, desc_idx, data_idx, hdr->cnt);
    if (data_idx < hdr->cnt) {
        data_flash_read((uint8_t *)&data_tmp,
            hdr->start_sector + data_idx, sizeof(struct test_data));
        data_tmp.valid = 0;
        data_flash_write((uint8_t *)&data_tmp,
            hdr->start_sector + data_idx, sizeof(struct test_data));
        data_desc_update(desc_idx, &data_tmp);
        return 1;
    } else
        return 0; 
}

int data_delete_all(void)
{
    int i;
    struct data_header *hdr = data_header_obj();
    struct data_desc *desc;

    hdr->magic = DATA_HEADER_MAGIC;
    hdr->start_sector = DATA_START_SECTOR;
    hdr->next_sector = DATA_START_SECTOR;
    hdr->cnt = 0;

    for (i = 0; i < DATA_MAX_NUM; i++) {
        desc = data_desc_get(i);
        desc->valid = 0;
    }

    data_header_save(hdr);
    return 0;
}

int data_read(struct test_data *stream, int idx)
{
    struct data_header *hdr = data_header_obj();

    DATA_DEBUG("%s: read data %d total %d\r\n",
        __FUNCTION__, idx, hdr->cnt);
    if (idx < hdr->cnt) {
        data_flash_read((uint8_t *)stream,
            hdr->start_sector + idx, sizeof(struct test_data));
        if (stream->magic != DATA_MAGIC)
            stream->valid = 0;
        return 1;
    }
    return 0;
}

int data_num_get(void)
{
    struct data_header *hdr = data_header_obj();
    return hdr->cnt;
}

struct desc_reorder_list *desc_reorder_list_get(void)
{
    static struct desc_reorder_list list;
    return &list;
}

int data_reorder_list_update(struct desc_reorder_list *list)
{
    int i;
    int id = 0;
    int cnt, latest;
    struct data_desc *desc;
    struct data_header *hdr = data_header_obj();

    DATA_DEBUG("%s: update list, head %d, tail %d total %d\r\n",
        __FUNCTION__, hdr->start_sector, hdr->next_sector,
        hdr->cnt);
    cnt = hdr->next_sector - hdr->start_sector;
    latest = cnt - 1;
    for (i = latest; i >= 0; i--) {
        desc = data_desc_get(i);
        if (desc->valid) {
            DATA_DEBUG("%s: add desc %d to list %d\r\n",
                __FUNCTION__, i, id);
            list->descs[id++] = desc;
        }
    }

    if (hdr->cnt > cnt) {
        for (i = hdr->cnt - 1; i > latest; i--) {
            desc = data_desc_get(i);
            if (desc->valid) {
                DATA_DEBUG("%s: add desc %d to list %d\r\n",
                    __FUNCTION__, i, id);
                list->descs[id++] = desc;
            }
        }
    }

    list->cnt = id;
    DATA_DEBUG("%s: total %d valid data record\r\n",
        __FUNCTION__, id);

    return id;
}

int data_init(void)
{
    int data_idx;
    struct data_header *hdr = data_header_obj();
    struct test_data *td = data_obj();

    td->elect_stat = CHECK_UNDO;
    data_header_init(hdr);
    DATA_DEBUG("%s: init total %d records\r\n",
        __FUNCTION__, hdr->cnt);
    
    for (data_idx = 0; data_idx < hdr->cnt; data_idx++) {
        data_flash_read((uint8_t *)&data_tmp,
            hdr->start_sector + data_idx, sizeof(struct test_data));
        data_desc_update(data_idx, &data_tmp);
    }
    return 0;
}

int data_usb_detect(void)
{   
    USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID,
        &USB_Host, &USBH_MSC_cb, &USR_cb);
    return 0;
}
	
static char dir_name[32];
static char fname[64];
static char line[128];
static FIL file;


int data_file_mkdir(void)
{
    int i = 0;
    int res;
    while (i < 10000) {
        sprintf(dir_name, "%d:TCZY%04d", USB, i);
        //printf("try mkdir %s\r\n", dir_name);
        res = f_mkdir(dir_name);
        if (res == FR_OK) {
            //printf("ok!\r\n");
            break;
        } else if (res == FR_EXIST)
            i++;
        else {
            //printf("mkdir error %d\r\n", res);
            return -res;
        }
    }
    return 0;
}

int data_file_export(struct desc_reorder_list *list)
{
    FRESULT res;
    int i;
    UINT bw;

    for (i = 0; i < list->cnt; i++) {
        data_read(&data_tmp, list->descs[i]->idx);

        sprintf(fname, "%s/data%03d.txt", dir_name, data_tmp.index);   
        //printf("creat file %s\r\n", fname);
        res = f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE);
        if (res != FR_OK) {
            //printf("creat file fail %d\r\n", res);
            continue;
        }
            
        sprintf(line, "    氨离子检测报告\r\n\r\n");
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "序号:  %d\r\n", data_tmp.index);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "时间:  20%02d-%02d-%02d    %02d:%02d\r\n",
            data_tmp.year, data_tmp.month, data_tmp.day,
            data_tmp.hour, data_tmp.minute);
        f_write(&file, line, strlen(line), &bw);
        switch (data_tmp.elect_stat) {
        case CHECK_PASS:
            sprintf(line, "电极状态:  正常\r\n");
            break;
        case CHECK_FAIL:
        case CHECK_UNDO:        
        default:
            sprintf(line, "电极状态:  异常\r\n");
            break;
        }
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "试样质量:  %.3fg\r\n",
            data_tmp.weight_sample);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "滤液体积:  %.2fmL\r\n",
            data_tmp.volume_sample);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "空白实验1溶液浓度:  %.4fmg/L\r\n",
            data_tmp.concent_block1);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "空白实验2溶液浓度:  %.4fmg/L\r\n",
            data_tmp.concent_block2);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "空白实验平均溶液浓度:  %.4fmg/L\r\n",
            data_tmp.concent_blockave);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "试样溶液浓度:  %.4fmg/L\r\n",
            data_tmp.concent_sample);
        f_write(&file, line, strlen(line), &bw);
        sprintf(line, "氨离子含量:  %.2fmg/kg\r\n",
            data_tmp.result);
        f_write(&file, line, strlen(line), &bw);
        f_close(&file);
    }
    return 0;
}

void data_add_timestamp(struct test_data *td)
{
    RTC_TimeTypeDef  RTC_TimeStructure;
    RTC_DateTypeDef  RTC_DateStructure;

    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

    td->year = RTC_DateStructure.RTC_Year;
    td->month = RTC_DateStructure.RTC_Month;
    td->day = RTC_DateStructure.RTC_Date;
    td->hour = RTC_TimeStructure.RTC_Hours;
    td->minute = RTC_TimeStructure.RTC_Minutes;
}

/**
 * y = ax + b
 * 其中 y 为溶液浓度，单位为 ug/L
 * 其中 x 为对应浓度下测得的电压值，单位为 mV
*/
struct data_coeff {
    float a;
    float b;
};

static struct data_coeff dcoeff;

struct drecord {
    float volt;
    float concent;
};

void data_calc_coeff(struct test_data *td)
{
    float sum_volt = 0.0;
    float sum_concent = 0.0;
    float volt_avg = 0.0;
    float concent_avg = 0.0;
    float up = 0.0;
    float down = 0.0;
    float a, b;
    int i = 0;

    struct drecord data_record[3];

    data_record[0].concent = 1.0;
    data_record[0].volt = 10.0;
    //data_record[0].volt = td->volt_0001;
    data_record[1].concent = 10.0;
    data_record[1].volt = -35.0;
    //data_record[1].volt = td->volt_001;
    data_record[2].concent = 100.0;
    //data_record[2].volt = -90.0;
    data_record[2].volt = td->volt_01;

    // for (i = 0; i < 3; i++) {
    //     volt_avg += data_record[i].volt;
    //     concent_avg += data_record[i].concent;
    //     sum_vc += data_record[i].volt * data_record[i].concent;
    //     sum_volt2 += data_record[i].volt * data_record[i].volt;
    // }
        
    // volt_avg /= 3;
    // concent_avg /= 3;
    // nxy_avg = volt_avg * concent_avg * 3;
    // nvolt_avg2 = volt_avg * volt_avg * 3;

    // a = (sum_vc - nxy_avg) / (sum_volt2 - nvolt_avg2);
    // b = concent_avg - a * volt_avg;

    // for (i = 0; i < 3; i++) {
    //     sum_volt += data_record[i].volt;      
    //     sum_concent += data_record[i].concent;
    // }
    // volt_avg = volt_avg / 3;
    // concent_avg = concent_avg / 3; 
    // for (i = 0; i < 3; i++) {     
    //     up += (data_record[i].concent - concent_avg) * (data_record[i].volt - volt_avg);    
    //     down += (data_record[i].volt - volt_avg) * (data_record[i].volt - volt_avg);    
    // }    

    // if(down == 0) 
    //     a = 0.0;  
    // else
    //     a = (up / down);       

    // b = (sum_concent - a * sum_volt) / 3;
    printf("data 0 volt = %f, concent = %f\r\n", data_record[0].volt, data_record[0].concent);
    printf("data 1 volt = %f, concent = %f\r\n", data_record[1].volt, data_record[1].concent);
    a = (data_record[1].concent - data_record[0].concent) / (data_record[1].volt - data_record[0].volt);
    b = data_record[1].concent - a * data_record[1].volt;

    dcoeff.a = a;
    dcoeff.b = b;
    DATA_DEBUG("%s: calc coeff a = %f, b = %f\r\n",
        __FUNCTION__, a, b);
}

/* return a concent (mg/L) */
float data_calc_concentration(float volt)
{
    float concent = dcoeff.a * volt + dcoeff.b;
    DATA_DEBUG("%s: volt %fmV  convert to %fug/L\r\n",
        __FUNCTION__, volt, concent);
    return concent;
}

void data_calc_result(struct test_data *td)
{
    td->result = (td->concent_sample - td->concent_blockave) *
        td->volume_sample / td->weight_sample;
}
