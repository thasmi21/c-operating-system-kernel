#ifndef _HORLOGE_H_
#define _HORLOGE_H_
#include <inttypes.h>

//Permet d'écrire une chaîne de caractères en haut à droite de l'écran (position de l'affichage de l'heure)
void write_timer(const char *s);

//Gère les interruptions timer et actualise l'horloge en conséquence
void trap_handler(uint64_t mcause, uint64_t mie, uint64_t mip);

//Initialise le timer en exécutant le traitant d'interruption
void init_traitant_timer(void (*traitant)(void));

//Autorise les interruptions timer
void enable_timer();

//Retourne le nombre de secondes écoulées depuis le lancement du terminal
uint32_t nbr_secondes(void);

extern void mon_traitant(void);

#endif