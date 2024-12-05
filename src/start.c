#include <inttypes.h>
#include <stdio.h>
#include <cpu.h>
#include <stddef.h>
#include "string.h"
#include "horloge.h"
#include "processus.h"
//#include "ordonnanceur.h"

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
    
    init_proc();

    new_proc("proc1", &proc1);

    idle();

    /* démasquage des interruptions externes */
    enable_it();

    /* on ne doit jamais sortir de kernel_start */
    while (1) {
        /* cette fonction arrete le processeur */
        hlt();
    }
}
