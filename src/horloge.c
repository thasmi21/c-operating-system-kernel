#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "font.h"
#include "string.h"
#include "affichage.h"
#include "processus.h"

#define IT_FREQ 20
#define TIMER_FREQ 10000000
#define CLINT_TIMER 0x0200bff8
#define CLINT_TIMER_CMP 0x02004000

int interruptions, h, m, s = 0;

extern uint32_t position_curseur_col;
extern uint32_t position_curseur_lig;
extern void mon_traitant(void); //Inclusion de la fonction ASM dans le fichier C

void write_timer(char *s){
    uint32_t a = position_curseur_lig;
    uint32_t b = position_curseur_col;
    for (int i = 0; i < 10; i++){
        ecrit_car(0, 150+i, s[i]);
    }
    place_curseur(a+1, b);
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
            char timer[11];
            sprintf(timer, "[%02d:%02d:%02d]", h, m, s);
            write_timer(timer);  //Impression de la nouvelle heure dans le format "[HH:MM:SS]"
            decrement_reveil();
        }
    }

    ordonnance();  
    
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
    return h/3600 + m/60 + s;
}