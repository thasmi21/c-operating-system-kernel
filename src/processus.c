#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE_PILE 4096
#define SIZE_TABLE 2
#define NOM_MAX_SZ 100

enum etat {
    ELU,
    ACTIVABLE
};

int pid = 0;

struct process {
    int pid;
    char nom[NOM_MAX_SZ];
    enum etat etat; 
    uint64_t tab[17];
    uint64_t pile[SIZE_PILE];
};

struct process table_process[SIZE_TABLE];

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    ctx_sw(&table_process[0].tab, &table_process[1].tab);
}

void proc1(void)
{
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    hlt();
}


// void init_proc(struct process idle){
//     pid = 0;
//     idle.etat = ELU;
//     uint64_t newpile[SIZE_PILE], newtas[17];
//     idle.pile = newpile;
//     idle.tas = newtas;
// }