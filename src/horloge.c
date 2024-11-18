#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "font.h"
#include "string.h"
#include "affichage.h"

#define IT_FREQ 20
#define TIMER_FREQ 10000000
#define CLINT_TIMER 0x0200bff8
#define CLINT_TIMER_CMP 0x02004000

int h, m, s = 0;

extern void mon_traitant(void); //Inclusion de la fonction ASM dans le fichier C

void write_timer(const char *s){
    int len = strlen(s);
    for (int i=0; i<len; i++){
        ecrit_car(0, 1280-len+i, s[i]);
    }
}

void trap_handler(uint64_t mcause, uint64_t mie, uint64_t mip){
    //Vérification de la nature de l'interruption
    uint64_t x;
    __asm__("csrr %0, mcause": "=r"(x));
    x = x & 0x7FFFFFFFFFFFFFFF; //Lecture de mcause en ignorant le bit 63
    
    if (x ==7){ //Identification d'une exception timer

        //Acquitter auprès du composant que l'exception a bien été traitée
        uint64_t t;
        __asm__("csrr %0, CLINT_TIMER": "=r"(t));  //Lecture de la valeur du timer
        __asm__("csrw CLINT_TIMER_CMP, %0" :: "r"(t + TIMER_FREQ/IT_FREQ)); //Réenclenchement du timer à t + nombre de tics restant

        //Actualisation de l'horloge
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
        char timer[9];
        sprintf(timer, "[%02d:%02d:%02d]", h, m, s);
        write_timer(timer);  //Impression de la nouvelle heure dans le format "[HH:MM:SS]"
    }
}

void init_traitant_timer(void (*traitant)(void)){
    __asm__("csrw mtvec, %0" :: "r"(traitant)); //Renseigne l'adresse de l'interruption dans le registre mtvec
}

void enable_timer(){
    __asm__("csrw mie, %0" :: "r"(0x80)); // Active les interruptions timer (bit 7)
    * (uint64_t) CLINT_TIMER = (uint64_t*) CLINT_TIMER_CMP + TIMER_FREQ/IT_FREQ;
}