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
#define SIZE_TABLE 4

struct processus table_process[SIZE_TABLE];
struct processus* elu;

extern const char* etat_str[];
extern void ctx_sw(uint64_t* a, uint64_t* b);
int pid = 0;
uint32_t temps = 0;

const char* etat_str[] = {
    "ELU",
    "ACTIVABLE",
    "ENDORMI",
    "MORT"
};

void idle(void){

    for (;;) {
        enable_it();
        hlt();
        disable_it();
    }
}

void proc1(void){
    
    for (;;) {
        enable_it();
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(2);
        disable_it();
    }
}

void proc2(void) {
    
    for (;;) {
        enable_it();
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(3);
        disable_it();
    }
}

void proc3(void) {
    
    for (;;) {
        enable_it();
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(5);
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

int cree_processus(char *nom, void (*code)(void)){

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

    struct processus *ancien = elu;
    elu = &table_process[(elu->pid+1)%pid]; //elu pointe vers le processus après celui actuellement élu dans table_process
    
    int i = 1;
    while (i < pid && i != -1){//Parcourt tout table_process pour décrémenter les reveils et trouver le prochain processus élu
        if (elu->etat != ACTIVABLE){
            i += 1;
            elu = &table_process[(elu->pid+1)%pid];
        }
        else {
            ancien->etat = ACTIVABLE;
            elu->etat = ELU;
            i = -1;
        }
    }

    if (i == -1){
        printf("Switch de processus : passage de %s a %s.\n", ancien->nom, elu->nom);
        ctx_sw(ancien->tab, elu->tab);
    }
    else {
        printf("[%s] : Tous les autres processus sont endormis, retour à IDLE.\n", mon_nom());
        ancien->etat = ACTIVABLE;
        elu = &table_process[0];
        elu->etat = ELU;
        ctx_sw(ancien->tab, elu->tab);
    }
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
    }
}

void decrement_reveil(){
    for (int i = 0; i < pid; i++){

        if (table_process[i].reveil > 0){
            table_process[i].reveil -=1;
            if (table_process[i].reveil == 0){
                table_process[i].etat = ACTIVABLE;
            }
        }
        if (table_process[i].reveil < 0){
            printf("Error in processus.c/decrement_reveil : à %d secondes, %s.reveil < 0.\n", nbr_secondes(), mon_nom());
        }
    }
}

void end_proc(void){
//     if (mon_pid() == 0){
//         printf("Error in processus.c/end_proc : end_proc() appelé pour le processus IDLE.\n");
//     }
//     else {
//         elu->etat = MORT;
//         //Décrémenter pid de 1 ? Impose de replacer les éléments de table_process pour que leur PID respecte leur position
//         //Mais dans ce cas pourquoi le marquer comme MORT alors qu'il va disparaître -> inutile
//         ordonnance();
//     }
}

void affiche_etats(void){
//     //Vérifier que ca s'écrit bien sur la 1ère ligne
//     place_curseur(0,0);
//     printf("Etat des processus : ");
//     for (int i = 0; i < pid; i++){
//         switch (elu->etat){
//         case ELU:
//             printf("%s: ELU, ", elu->nom);
//             break;
//         case ACTIVABLE:
//             printf("%s: ACTIVABLE, ", elu->nom);
//             break;
//         case ENDORMI:
//             printf("%s: ENDORMI(%d), ", elu->nom, elu->reveil);
//             break;
//         case MORT:
//             printf("%s: MORT, ", elu->nom);
//             break;
//         default:
//             printf("%s: UNKNOWN, ", elu->nom);
//             break;
//         }
//     }
//     //printf("%d processus disponibles.\n", SIZE_TABLE - pid);
}