#ifndef __UI_DIAG_H__
#define __UI_DIAG_H__

struct diag_ok {
    const char *header;
    const char *str_lin1;
    const char *str_lin2;
    const char *str_lin3;
};

extern struct diag_ok g_diag_ok;


extern int diag_ok_creat(void);
extern int numpad_creat(void);
extern char *numpad_get(void);

#endif
