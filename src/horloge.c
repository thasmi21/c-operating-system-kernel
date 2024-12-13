#include "include.h"

int interruptions, h, m, s = 0;

extern uint32_t position_curseur_col;
extern uint32_t position_curseur_lig;
extern void mon_traitant(void); //Inclusion de la fonction ASM dans le fichier C

void write_timer(char *s){
    //On sauvegarde les coordonnées du curseur pour reprendre l'écriture principale après l'actualisation de l'horloge
    uint32_t a = position_curseur_lig; 
    uint32_t b = position_curseur_col;

    //Ecriture de l'horloge à nbr_secondes + 1
    for (int i = 0; i < 10; i++){
        ecrit_car(0, 150+i, s[i]);
    }

    //Replacement du curseur
    check_curseur(a, b);
}

void trap_handler(uint64_t mcause, uint64_t mie, uint64_t mip){
    
    //Vérification de la nature de l'interruption
    if ((mcause & 0x7FFFFFFFFFFFFFFF) ==7){ //Identification d'une exception timer

        //Acquitter auprès du composant que l'exception a bien été traitée
        interruptions += 1; //Incrémentation du nombre d'interruptions levées depuis le lancement du système
        *(uint64_t *) CLINT_TIMER_CMP = *(uint64_t *) CLINT_TIMER + TIMER_FREQ/IT_FREQ; //Réenclenchement du timer à t + nombre de tics restant

        //Actualisation de l'horloge toutes les 20 interruptions
        if (interruptions % IT_FREQ == 0){
            if (s==59){ 
                if (m==59){ //Si nouvelle heure
                    s=0;
                    m=0;
                    h+=1;
                }
                else { //Si nouvelle minute
                    s=0;
                    m+=1;
                }
            }
            else {
                s+=1;
            }
            //Construction d'un char de la forme de l'horloge, actualié à la nouvelle heure
            char timer[11];
            sprintf(timer, "[%02d:%02d:%02d]", h, m, s);

            write_timer(timer);  //Impression de la nouvelle heure dans le format "[HH:MM:SS]"
            decrement_reveil();  //Décrémenation du temps restant de chaque processus endormi avant leur réveil
        }
    }

    ordonnance();  //Appel à la fonction d'ordonnancement pour changer de processus à chaque interruption
    
}


void init_traitant_timer(void (*traitant)(void)){
    place_curseur(0, 150);
    printf("[00:00:00]");
    __asm__("csrw mtvec, %0" :: "r"(traitant)); //Renseigne l'adresse de l'interruption dans le registre mtvec
}


void enable_timer(){

    //Autorise les interruptions timer en mettant le 7e bit de mie à 1
    uint64_t mie_value;
    __asm__("csrr %0, mie": "=r"(mie_value));
    mie_value |= 0x80; //Masque pour ne changer que le 7e bit de mie
    __asm__("csrw mie, %0" :: "r"(mie_value)); 

    *(uint64_t*) CLINT_TIMER_CMP = *(uint64_t*) CLINT_TIMER + TIMER_FREQ/IT_FREQ;
}


uint32_t nbr_secondes(void){
    return h/3600 + m/60 + s; //Calcul du nombre total de secondes éoulées depuis le lancement du système
}