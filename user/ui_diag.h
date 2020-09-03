#ifndef __UI_DIAG_H__
#define __UI_DIAG_H__

struct diag_info {
    const char *header;
    const char *str_lin1;
    const char *str_lin2;
    const char *str_lin3;
};

struct result_item {
    const char *item;
    char value[20];
};

#define RESULT_ITEMS_MAX     8

struct diag_result {
    const char *header;
    int cnt;
    struct result_item items[RESULT_ITEMS_MAX];
};

extern struct diag_info g_diag_ok;
extern struct diag_info g_diag_start;
extern struct diag_info g_diag_wait;
extern struct diag_result g_diag_res;


extern int diag_ok_creat(void);
extern int diag_start_creat(void);
extern int diag_result_creat(void);
extern WM_HWIN diag_wait_creat(int x0);
extern int numpad_creat(void);
extern char *numpad_get(void);

#endif
