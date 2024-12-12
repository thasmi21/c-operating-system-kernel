#ifndef _PROCESSUS_H_
#define _PROCESSUS_H_

#include "include.h"

extern void ctx_sw(uint64_t* a, uint64_t* b);

//Liste des états accessibles par un processus au cours de son exécution
enum etat {
    ELU,
    ACTIVABLE,
    ENDORMI,
    MORT
};

//Structure d'un processus
struct processus {
    int pid;
    char nom[NOM_MAX_SZ];
    enum etat etat; 
    uint64_t tab[17];
    uint64_t pile[SIZE_PILE];
    int reveil; 
};

//Permet d'afficher l'état d'un processus sous forme de string
const char* get_etat();

//Gère les interruptions et passe la main 
void idle(void);

//Passe la main 
void proc1(void);

//Renvoie la main
void proc2(void);

//Renvoie la main
void proc3(void);

//(Ré)initialise le tableau de processus et crée le processus initial idle
void init_proc(void);

//Créer un nouveau processus dont le registre ra pointe vers la fonction à exécuter 
int cree_processus(char *nom, void (*code)(void));

//Renvoie le pid du processus actif
int32_t mon_pid(void);

//Renvoie le nom du processus actif
char *mon_nom(void);

//Met fin à un processus en le declarant comme MORT
void fin_processus(void);

//Affiche en haut à gauche tous les processus et leur état (+ temps restant avant réveil si endormi) et le nombre de processus disponibles
void affiche_etats(void);

#endif 