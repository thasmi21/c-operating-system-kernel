#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE_PILE 4096
#define SIZE_TABLE 2

extern void ctx_sw(uint64_t a, uint64_t b);

struct process {
    int pid;
    char nom;
    enum etat {ELU, ACTIVABLE};
    uint64_t* tab[17];
    uint64_t pile[SIZE_PILE];
};

struct process table_process[SIZE_TABLE];

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    ctx_sw(idle->tab[1], proc1->tab[1]);
}

void proc1(void)
{
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    hlt();
}
