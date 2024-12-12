#ifndef _CURSEUR_H_
#define _CURSEUR_H_

#include "include.h"

extern uint32_t position_curseur_col;
extern uint32_t position_curseur_lig;

//Place le curseur à la ligne et à la colonne renseignées en paramètres
void place_curseur(uint32_t lig, uint32_t col);

//Vérifie que le curseur n'est pas en bout de ligne ou en fin de page
void check_curseur(uint32_t lig, uint32_t col);

#endif