#ifndef _PROCESSUS_H_
#define _PROCESSUS_H_
#include <inttypes.h>

#define SIZE_PILE 4096
#define NOM_MAX_SZ 100

extern void ctx_sw(uint64_t a, uint64_t b);

enum etat {
    ELU,
    ACTIVABLE
};

struct processus {
    int pid;
    char nom[NOM_MAX_SZ];
    enum etat etat; 
    uint64_t tab[17];
    uint64_t pile[SIZE_PILE];
};

void idle(void);

void proc1(void);

void init_proc(void);

int new_proc(char *nom, void (*code)(void));

#endif