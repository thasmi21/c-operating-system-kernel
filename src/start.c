#include <inttypes.h>
#include <stdio.h>
#include <cpu.h>
#include <stddef.h>

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
    console_putbytes("Truc\nSalut\n1\t2\t3\nAB\bC\nBEEF\rRABBIT\n", 34);
    for (int i=0;i<84;i++) {
        console_putbytes("%d\n",i);
    }
    // console_putbytes('Hello\n', 6);
    // console_putbytes('\f', 1);

    /* on ne doit jamais sortir de kernel_start */
    while (1) {
        /* cette fonction arrete le processeur */
        hlt();
    }
}
