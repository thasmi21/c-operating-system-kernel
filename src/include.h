#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include <font.h>

#include "curseur.h"
#include "affichage.h"
#include "horloge.h"
#include "processus.h"
#include "ordonnanceur.h"

#define NB_OCTETS 40960

#define IT_FREQ 20
#define TIMER_FREQ 10000000
#define CLINT_TIMER 0x0200bff8
#define CLINT_TIMER_CMP 0x02004000

#define SIZE_TABLE 4
#define SIZE_PILE 4096
#define NOM_MAX_SZ 100