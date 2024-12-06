#ifndef _PROCESSUS_H_
#define _PROCESSUS_H_
#include <inttypes.h>

#define SIZE_PILE 4096
#define NOM_MAX_SZ 100

extern void ctx_sw(uint64_t* a, uint64_t* b);

enum etat {
    ELU,
    ACTIVABLE,
    ENDORMI,
    MORT
};

struct processus {
    int pid;
    char nom[NOM_MAX_SZ];
    enum etat etat; 
    uint64_t tab[17];
    uint64_t pile[SIZE_PILE];
    int reveil; 
};

//Passe la main à proc1
void idle(void);

//Renvoie la main à idle
void proc1(void);

//(Ré)initialise le tableau de processus et crée le processus initial idle
void init_proc(void);

//Créer un nouveau processus dont le registre ra pointe vers la fonction à exécuter 
int new_proc(char *nom, void (*code)(void));

//Automatise le changement de processus, en mettant à jour leur état et le pointeur global
void ordonnance(void);

//Renvoie le pid du processus actif
int32_t mon_pid(void);

//Renvoie le nom du processus actif
char *mon_nom(void);

//Fait dormir le processus élu pendant nbr_secs secondes
void dors(uint32_t nbr_secs);

//Met fin à un processus en le declarant comme MORT
void end_proc(void);

//Affiche en haut à gauche tous les processus et leur état (+ temps restant avant réveil si endormi) et le nombre de processus disponibles
void affiche_etats(void);

#endif 