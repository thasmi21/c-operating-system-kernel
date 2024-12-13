#include "include.h"

extern struct processus table_process[SIZE_TABLE];
extern struct processus* elu;
extern void ctx_sw(uint64_t* a, uint64_t* b);
extern int pid;


void ordonnance(void){

    //Créé un pointeur vers le processus actif afin d'actualiser elu
    struct processus *ancien = elu;
    elu = &table_process[(elu->pid+1)%(pid+1)]; //elu pointe vers le processus après celui actuellement élu dans table_process
    //Si le processus qui vient de se terminer n'est ni mort ni endormi, il devient activable
    if (ancien->etat == ELU){
        ancien->etat = ACTIVABLE;
    }

    int i = 0;
    while (i <= pid && i != -1){//Parcourt tout table_process (une seule fois) pour trouver le prochain processus élu
        if (elu->etat != ACTIVABLE){ //Si état = MORT ou ENDORMI, on passe au prochain
            i += 1;
            elu = &table_process[(elu->pid+1)%(pid+1)];
        }
        else { //On a trouvé un processus activable qui devient actif
            elu->etat = ELU;
            i = -1;
        }
    }

    if (i == -1){ //Switch vers le nouveau processus élu
        affiche_etats();
        printf("Switch de processus : passage de %s a %s.\n", ancien->nom, elu->nom);
        ctx_sw(ancien->tab, elu->tab);
    }
    else { //Cas où aucun autre processus n'était activable (seul IDLE n'est pas endormi/mort, on reste donc sur lui)
        printf("[%s] : Tous les autres processus sont endormis, retour à IDLE.\n", mon_nom());
        
        //On restaure IDLE comme processus actif
        ancien->etat = ACTIVABLE;
        elu = &table_process[0];
        elu->etat = ELU;
        affiche_etats();
        ctx_sw(ancien->tab, elu->tab);
    }
}


void dors(uint32_t nbr_secs){
    //Vérification de la conformité des paramètres
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
        ordonnance(); //Le processus s'est endormi, on doit donc en élire un nouveau
    }
}

void decrement_reveil(){
    for (int i = 0; i <= pid; i++){ //Parcourt des processus 

        //Recherhe des processus endormis 
        if (table_process[i].reveil > 0){
            table_process[i].reveil -=1; //Décrémentation du temps restant avant le réveil
            printf("\n%s dort pendant encore %d secondes.\n", table_process[i].nom, table_process[i].reveil);
        
            if (table_process[i].reveil == 0){ //On réveille un processus arrivé au bout de son sommeil
                table_process[i].etat = ACTIVABLE;
            }
        }
        if (table_process[i].reveil < 0){ //Processus avec un temps négatif
            printf("Error in processus.c/decrement_reveil : à %d secondes, %s.reveil < 0.\n", nbr_secondes(), mon_nom());
        }
    }
}