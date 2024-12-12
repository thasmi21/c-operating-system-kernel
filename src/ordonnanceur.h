#ifndef _ORDONNANCEUR_H_
#define _ORDONNANCEUR_H_

#include "include.h"

//Gère l'ordonnancement des différents processus en passant 20x/sec la main aux processus activables
void ordonnance(void);

//Endort un processus pendant nbr_secs secondes
void dors(uint32_t nbr_secs);

//Decrémente le temps de sommeil des processus endormi chaque seconde et les réveille quand c'est nécessaire 
void decrement_reveil();

#endif