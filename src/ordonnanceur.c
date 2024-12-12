#include "include.h"

extern struct processus table_process[SIZE_TABLE];
extern struct processus* elu;
extern void ctx_sw(uint64_t* a, uint64_t* b);
extern int pid;


void ordonnance(void){

    struct processus *ancien = elu;
    elu = &table_process[(elu->pid+1)%(pid+1)]; //elu pointe vers le processus après celui actuellement élu dans table_process
    if (ancien->etat == ELU){
        ancien->etat = ACTIVABLE;
    }
    int i = 0;
    while (i <= pid && i != -1){//Parcourt tout table_process pour décrémenter les reveils et trouver le prochain processus élu
        if (elu->etat != ACTIVABLE){
            i += 1;
            elu = &table_process[(elu->pid+1)%(pid+1)];
        }
        else {
            elu->etat = ELU;
            i = -1;
        }
    }

    if (i == -1){
        affiche_etats();
        printf("Switch de processus : passage de %s a %s.\n", ancien->nom, elu->nom);
        ctx_sw(ancien->tab, elu->tab);
    }
    else {
        printf("[%s] : Tous les autres processus sont endormis, retour à IDLE.\n", mon_nom());
        ancien->etat = ACTIVABLE;
        elu = &table_process[0];
        elu->etat = ELU;
        affiche_etats();
        ctx_sw(ancien->tab, elu->tab);
    }
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
        printf("\n%s s'endort pendant %d secondes.\n", elu->nom, elu->reveil);
        ordonnance();
    }
}

void decrement_reveil(){
    for (int i = 0; i <= pid; i++){

        if (table_process[i].reveil > 0){
            table_process[i].reveil -=1;
            printf("\n%s dort pendant encore %d secondes.\n", table_process[i].nom, table_process[i].reveil);
            if (table_process[i].reveil == 0){
                table_process[i].etat = ACTIVABLE;
            }
        }
        if (table_process[i].reveil < 0){
            printf("Error in processus.c/decrement_reveil : à %d secondes, %s.reveil < 0.\n", nbr_secondes(), mon_nom());
        }
    }
}