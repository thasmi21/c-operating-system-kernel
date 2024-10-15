#include <inttypes.h>
#include <stdio.h>
#include <cpu.h>

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
    /*for (int i = 0; i < 1280; i++){
        pixel(i, 5, 0x0FFFFFFF);
    }**/

    //place_curseur(0, 0);
    
    ecrit_car(1, 0, "M", 0x0FFFFFFF);

    /* on ne doit jamais sortir de kernel_start */
    while (1) {
        /* cette fonction arrete le processeur */
        hlt();
    }
}
