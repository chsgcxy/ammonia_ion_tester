#ifndef __UI_DIAG_H__
#define __UI_DIAG_H__

struct diag_info {
    const char *header;
    const char *str_lin1;
    const char *str_lin2;
    const char *str_lin3;
};

extern struct diag_info g_diag_ok;
extern struct diag_info g_diag_start;
extern struct diag_info g_diag_wait;


extern int diag_ok_creat(void);
extern int diag_start_creat(void);
extern WM_HWIN diag_wait_creat(int x0);
extern int numpad_creat(void);
extern char *numpad_get(void);

#endif
