#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE_PILE 4096

struct process {
    int pid;
    char nom;
    enum etat {ELU, ACTIVABLE};
    uint64_t tab[17];
    uint64_t pile[SIZE_PILE];
};