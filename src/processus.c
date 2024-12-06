#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "processus.h"
#include "horloge.h"
#include "affichage.h"

#define SIZE_PILE 4096
#define NOM_MAX_SZ 100
#define SIZE_TABLE 2

struct processus table_process[SIZE_TABLE];
struct processus* elu; 
struct processus* activable; 

extern void ctx_sw(uint64_t a, uint64_t b);
int pid = 0;
int temps = 0;

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    table_process[0].etat = ACTIVABLE;
    ctx_sw(table_process[0].tab, table_process[1].tab);
}

void proc1(void)
{
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    table_process[1].etat = ELU;
    elu = &table_process[1];
    activable = &table_process[0];
    while (1){
        hlt();
    }
}

void proc2(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        enable_it();
        hlt();
        disable_it();
    }
}

void proc3(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        enable_it();
        hlt();
        disable_it();
    }
}

void init_proc(void){
    //Besoin de réinitialiser table_process ?
    strcpy(table_process[0].nom, "idle");
    table_process[0].etat = ELU;
    table_process[0].tab[0] = (uint64_t)&idle;

    pid = 1;
    elu = &table_process[0]; 
}

int new_proc(char *nom, void (*code)(void)){

    if (pid >= SIZE_TABLE){
        printf("Error in processus.c/new_proc : Nombre de processus maximum atteint, la création a echoué.\n");
        return -1;
    }

    if (sizeof(*nom) > NOM_MAX_SZ){
        printf("Error in processus.c/new_proc : Nom de processus trop long (taille maximale = 100 caractères).\n");
        return -1;
    }

    table_process[pid].pid = pid+1;
    strcpy(table_process[pid].nom, nom);
    table_process[pid].etat = ACTIVABLE;
    table_process[pid].tab[0] = (uint64_t)code;
    table_process[pid].tab[1] = (uint64_t)&table_process[pid].pile[SIZE_PILE];

    pid += 1;

    return pid;
}

void ordonnance(void){

    int decrement_reveil = 0;
    if (temps != nbr_secondes()){ //Regarde si une nouvelle seconde s'est écoulée pour mettre à jour les réveils
        temps = nbr_secondes();
        decrement_reveil = 1;
    }
    else {
        decrement_reveil = 0;
    }

    struct process* ancien = elu;
    elu = &table_process[(elu->pid+1)%pid]; //elu pointe vers le processus après celui actuellement élu dans table_process

    int position = ancien->pid;
    for (int i = 0; i < pid; i++){ //Parcourt tout table_process pour décrémenter les reveils et trouver le prochain processus élu
        if (elu->etat == ENDORMI){
            elu->reveil -= decrement_reveil;
        }
        if (elu->reveil == 0 && elu->etat != MORT){ //Se souvient de la position du 1er processus activable rencontré pour lui passer la main
            elu->etat = ACTIVABLE;
            if (position == ancien->pid){
                position = elu->pid;
            }
        elu = &table_process[(elu->pid+1)%pid];
        }
    }

    elu = &table_process[position-1];
    elu->etat = ELU;

    printf("Switch de processus : passage de %s à %s.\n", ancien->nom, elu->nom);
    affiche_etats();
    ctx_sw(ancien->tab, elu->tab);
}

int32_t mon_pid(void){
    return elu->pid;
}

char *mon_nom(void){
    return elu->nom;
}

void dors(uint32_t nbr_secs){
    if (nbr_secs < 1){
        printf("Error in processus.c/dors : nbr_secs doit être > 0.\n");
    }
    elif (mon_pid() == 0){
        printf("Error in processus.c/dors : dors() appelé pour le processus IDLE.\n");
    }
    else {
        elu->reveil = nbr_secs;
        elu->etat = ENDORMI;
        affiche_etats();
        ordonnance();
    }
}

void end_proc(void){
    if (mon_pid() == 0){
        printf("Error in processus.c/end_proc : end_proc() appelé pour le processus IDLE.\n");
    }
    else {
        elu->etat = MORT;
        //Décrémenter pid de 1 ? Impose de replacer les éléments de table_process pour que leur PID respecte leur position
        //Mais dans ce cas pourquoi le marquer comme MORT alors qu'il va disparaître -> inutile
        affiche_etats();
        ordonnance();
    }
}

void affiche_etats(void){
    //Vérifier que ca s'écrit bien sur la 1ère ligne
    place_curseur(0,0);
    printf("État des processus : ");
    for (int i = 0; i < pid; i++){
        switch (elu->etat){
        case ELU: printf("%s - ÉLU, ", elu->nom);
        case ACTIVABLE: printf("%s - ACTIVABLE, ", elu->nom);
        case ENDORMI: printf("%s - ENDORMI (%d), ", elu->nom, elu->reveil);
        case MORT: printf("%s - MORT, ", elu->nom);
        default: printf("%s - UNKNOWN, ", elu->nom);
        }
    }
    printf("%d processus disponibles.\n", SIZE_TABLE - pid-1);
}