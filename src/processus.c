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

extern void ctx_sw(uint64_t* a, uint64_t* b);
int pid = 0;
uint32_t temps = 0;

void idle(void)
{
    for (int i = 0; i < 2; i++){
        printf("\n[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }

    printf("[idle] je bloque le systeme.\n");
    while (1){
        hlt();
    }
}

void proc1(void)
{
    for (int i = 0; i < 2; i++){
        printf("\n\n[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
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
    table_process[0].pid = 0;
    table_process[0].etat = ELU;
    table_process[0].tab[0] = (uint64_t)&idle;
    table_process[0].tab[1] = (uint64_t)&table_process[0].pile[SIZE_PILE];
    table_process[0].reveil = 0;

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

    table_process[pid].pid = pid;
    strcpy(table_process[pid].nom, nom);
    table_process[pid].etat = ACTIVABLE;
    table_process[pid].tab[0] = (uint64_t)code;
    table_process[pid].tab[1] = (uint64_t)&table_process[pid].pile[SIZE_PILE];

    pid += 1;

    return pid;
}

void ordonnance(void){

    // int decrement_reveil = 0;
    // if (temps != nbr_secondes()){ //Regarde si une nouvelle seconde s'est écoulée pour mettre à jour les réveils
    //     temps = nbr_secondes();
    //     decrement_reveil = 1;
    // } 
    // else {
    //     decrement_reveil = 0;
    // }

    struct processus *ancien = elu;
    printf("ancien=%s\n, ancien->nom");
    elu = &table_process[(elu->pid+1)%pid]; //elu pointe vers le processus après celui actuellement élu dans table_process
    printf("elu=%s\n, elu->nom\n");

    int position = ancien->pid;
    for (int i = 0; i < pid; i++){ //Parcourt tout table_process pour décrémenter les reveils et trouver le prochain processus élu
        // if (elu->etat == ENDORMI){
        //     elu->reveil -= decrement_reveil;
        // }
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

    printf("Switch de processus : passage de %s a %s.\n", ancien->nom, elu->nom);
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
    else if (mon_pid() == 0){
        printf("Error in processus.c/dors : dors() appelé pour le processus IDLE.\n");
    }
    else {
        elu->reveil = nbr_secs;
        elu->etat = ENDORMI;
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
        ordonnance();
    }
}

void affiche_etats(void){
    //Vérifier que ca s'écrit bien sur la 1ère ligne
    place_curseur(0,0);
    printf("Etat des processus : ");
    for (int i = 0; i < pid; i++){
        switch (elu->etat){
        case ELU:
            printf("%s: ELU, ", elu->nom);
            break;
        case ACTIVABLE:
            printf("%s: ACTIVABLE, ", elu->nom);
            break;
        case ENDORMI:
            printf("%s: ENDORMI(%d), ", elu->nom, elu->reveil);
            break;
        case MORT:
            printf("%s: MORT, ", elu->nom);
            break;
        default:
            printf("%s: UNKNOWN, ", elu->nom);
            break;
        }
    }
    //printf("%d processus disponibles.\n", SIZE_TABLE - pid);
}