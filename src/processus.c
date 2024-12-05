#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "processus.h"

#define SIZE_PILE 4096
#define NOM_MAX_SZ 100
#define SIZE_TABLE 2

struct processus table_process[SIZE_TABLE];
struct processus* elu; 

extern void ctx_sw(uint64_t *a, uint64_t *b);
int pid = 0;

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    table_process[0].etat = ACTIVABLE;
    ctx_sw(table_process[0].tab, table_process[1].tab);
}

void proc1(void)
{
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    table_process[1].etat = ELU;
    elu = &table_process[1];
    hlt();
}

void init_proc(void){
    table_process[0].pid = 1;
    strcpy(table_process[0].nom, "idle");
    table_process[0].etat = ELU;
    table_process[0].tab[0] = (uint64_t)&idle;

    pid = 1;
    elu = &table_process[0]; 
}

int new_proc(char *nom, void (*code)(void)){

    if (pid >= SIZE_TABLE){
        printf("Error in processus.c/new_proc : Nombre de processus maximum atteint, la création a echoué.\n");
        return -1;
    }

    if (sizeof(*nom) > NOM_MAX_SZ){
        printf("Error in processus.c/new_proc : Nom de processus trop long (taille maximale = 100 caractères).\n");
        return -1;
    }

    table_process[pid].pid = pid+1;
    strcpy(table_process[pid].nom, nom);
    table_process[pid].etat = ACTIVABLE;
    table_process[pid].tab[0] = (uint64_t)code;
    table_process[pid].tab[1] = (uint64_t)&table_process[pid].pile[SIZE_PILE];

    pid += 1;

    return pid;
    
}