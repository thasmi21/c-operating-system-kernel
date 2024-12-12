#include "include.h"

struct processus table_process[SIZE_TABLE];
struct processus* elu;

int pid = 0;
int nb_proc_actifs = 0;

const char* get_etat(enum etat e){
    switch (e) {
        case ELU:       return "ELU";
        case ACTIVABLE: return "ACTIVABLE";
        case ENDORMI:   return "ENDORMI";
        case MORT:      return "MORT";
        default:        return "INCONNU";
    }
}

void idle(void){
    
    for (;;) {
        enable_it();
        hlt();
        disable_it();
    }
}


void proc1(void){
    
    for (int i=0; i<1; i++) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(1);
    }
}


void proc2(void) {
    
    for (int i=0; i<2; i++) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(2);
    }
}


void proc3(void) {
    
    for (int i=0; i<3; i++) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        dors(3);
    }
}


void init_proc(void){
    strcpy(table_process[0].nom, "idle");
    table_process[0].pid = 0;
    table_process[0].etat = ELU;
    table_process[0].tab[0] = (uint64_t)&idle;
    table_process[0].tab[1] = (uint64_t)&table_process[0].pile[SIZE_PILE];
    table_process[0].reveil = 0;

    pid = 0;
    nb_proc_actifs = 1;
    elu = &table_process[0];
}


int cree_processus(char *nom, void (*code)(void)){

    if (pid > SIZE_TABLE){
        if (nb_proc_actifs == SIZE_TABLE){
            printf("Error in processus.c/cree_processus : trop de processus déjà actifs.\n");
            return -1;
        }
        else {
            int i=1;
            while (table_process[i].etat != MORT && i < nb_proc_actifs){
                i += 1;
            }
            if (i == nb_proc_actifs){
                printf("Error in processus.c/cree_processus : pas de processus mort mais nb_proc_actifs == SIZE_TABLE.\n");
                return -1;
            }
            else {
                pid = i;
            }
        }
    }

    if (sizeof(*nom) > NOM_MAX_SZ){
        printf("Error in processus.c/cree_processus : Nom de processus trop long (taille maximale = 100 caractères).\n");
        return -1;
    }

    pid += 1;

    table_process[pid].pid = pid;
    strcpy(table_process[pid].nom, nom);
    table_process[pid].etat = ACTIVABLE;
    table_process[pid].tab[0] = (uint64_t)&proc_launcher;
    table_process[pid].tab[4] = (uint64_t)code;
    table_process[pid].tab[1] = (uint64_t)&table_process[pid].pile[SIZE_PILE];

    nb_proc_actifs += 1;
    
    return pid;
}


int32_t mon_pid(void){
    return elu->pid;
}

char *mon_nom(void){
    return elu->nom;
}


void proc_launcher(void (*proc)(void)){
    proc();
    fin_processus();
}


void fin_processus(void){
    if (mon_pid() == 0){
        printf("Error in processus.c/fin_processus : fin_processus() appelé pour le processus IDLE.\n");
    }
    else {
        table_process[mon_pid()].etat = MORT;
        printf("%s, %s : \n", table_process[mon_pid()].nom, get_etat(table_process[mon_pid()].etat));
        nb_proc_actifs -= 1;
        ordonnance();
    }
}


void affiche_etats(void){
    uint32_t a = position_curseur_lig;
    place_curseur(0,0);
    printf("Etat des processus : ");
    for (int i = 0; i <= pid; i++){
        printf("%s: %s ", table_process[i].nom, get_etat(table_process[i].etat));
    }

    printf("                 ");

    check_curseur(a-1, 159);
}