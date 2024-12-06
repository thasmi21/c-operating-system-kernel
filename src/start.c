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
    place_curseur(0,0); //Tests pôur essayer d'afficher l'horloge à la ligne 0
    printf("Test");     //Write_timer fait un printf(%s) plutot qu'une boucle avec un ecrit_car par caractère car printf arrive a afficher sur la première ligne
    
    /* initialisations */
    enable_timer();
    init_traitant_timer(mon_traitant);
    
    init_proc();

    int err = cree_processus("proc1", &proc1);
    if (err < 0){
        printf("Error in start.c/kernel_start : Impossible de créer proc1.\n");
    }
    err = cree_processus("proc2", &proc2);
    if (err < 0){
        printf("Error in start.c/kernel_start : Impossible de créer proc2.\n");
    }
    err = cree_processus("proc3", &proc3);
    if (err < 0){
        printf("Error in start.c/kernel_start : Impossible de créer proc3.\n");
    }

    idle();

    /* démasquage des interruptions externes */
    enable_it();

    /* on ne doit jamais sortir de kernel_start */
    while (1) {
        /* cette fonction arrete le processeur */
        //affiche_etats();
        hlt();
    }
}
