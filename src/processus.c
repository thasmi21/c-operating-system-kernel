#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "processus.h"

#define SIZE_TABLE 2
struct process table_process[SIZE_TABLE];

extern void ctx_sw(uint64_t a, uint64_t b);
int pid = 0;

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    ctx_sw(*table_process[0].tab, *table_process[1].tab);
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