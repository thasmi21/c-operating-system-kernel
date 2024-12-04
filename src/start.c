#include <inttypes.h>
#include <stdio.h>
#include <cpu.h>
#include <stddef.h>
#include "string.h"
#include "horloge.h"
#include "processus.h"

extern void mon_traitant(void);

/* on peut s'entrainer a utiliser GDB avec ce code de base */
/* par exemple afficher les valeurs de n et res avec la commande display */
/* une fonction bien connue */
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{

    /* initialisations */
    enable_timer();
    init_traitant_timer(mon_traitant);
    
    struct process idle = {
        .pid = 0,
        .name = "idle",
        .etat = ELU,
    };

    struct process proc1 = {
        .pid = 1,
        .name = "proc1",
        .etat = ACTIVABLE,
    };

    proc1.tab[1] = (uint64_t)(proc1.pile + SIZE_PILE);
    proc1.tab[0] = (uint64_t)&proc1;

    table_process[0] = idle;
    table_process[1] = proc1;

    idle();

    /* démasquage des interruptions externes */
    //enable_it();

    /* on ne doit jamais sortir de kernel_start */
    while (1) {
        /* cette fonction arrete le processeur */
        hlt();
    }
}
