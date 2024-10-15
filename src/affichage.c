#include <inttypes.h>
#include <stdlib.h>
#include "font.h"
#include "string.h"

#define NB_OCTETS 3681280

uint32_t position_curseur_col = 0;
uint32_t position_curseur_lig = 1;

void pixel(uint32_t x, uint32_t y, uint32_t couleur){
    uint32_t *ptr = (uint32_t *) (0x80000000);
    ptr += (x + y * 1280);
    *ptr = couleur;
}

void place_curseur(uint32_t lig, uint32_t col){

    //Effacement du curseur précédent
    for (int i = 0; i < 8; i++){
        pixel(position_curseur_lig*8+i, position_curseur_col*8+7, 0x00000000);
    }

    //Placement du nouveau curseur à lig et col
    for (int i = 0; i < 8; i++){
        pixel(lig*8+i+8, col*8+7, 0x0FFFFFFF);
    }

    //Mise à jour de ses nouvelles coordonnées
    position_curseur_lig = lig; 
    position_curseur_col = col;
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t couleur){

    //Dessin du caractère 
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){

            if (((font8x8_basic[(int) c][i] >> j) & 1) == 1){
                pixel(col * 8 + j, lig * 8 + i, couleur);
            }
        }
    }

    //Repositionnement du curseur
    place_curseur(lig,col);
}

void efface_ecran(void){
    
    //Parcours de tout l'ecran
    for (int i = 0; i < 720; i++){
        for (int j = 0; j < 1280; j++){
            pixel(j+8, i, 0x00000000); //Mise à noir du pixel  
        }
    }

    //Réinitialisation de la position du curseur
    place_curseur(1, 0);
}

void defilement(void){

    uint32_t* dest = (uint32_t*) (0x80000000 + 1280 * 4);
    uint32_t* src = (uint32_t*) (0x80000000 + 719 * 1280 * 4);
    memmove(dest, src, NB_OCTETS);

    //Mise à noir de la dernière ligne
    uint32_t* ptr = (uint32_t*) (0x80000000 + 719 * 1280 * 4);
    for (int i = 0; i < 1280; i ++){
        *ptr = 0x00000000;
        ptr += 1;
    }
}

/*
void traite_car(char c){}
**/

