#include "include.h"

//Par défaut : curseur en (0,0)
uint32_t position_curseur_col = 0;
uint32_t position_curseur_lig = 1;


void place_curseur(uint32_t lig, uint32_t col){

    //Effacement du curseur précédent
    for (int i = 0; i < 8; i++){
        pixel(position_curseur_col*8 +i, position_curseur_lig*8+7, 0x00000000);
    }

    //Placement du nouveau curseur à lig et col
    for (int i = 0; i < 8; i++){
        pixel(col*8+i, lig*8+7, 0x0FFFFFFF);
    }

    //Mise à jour de ses nouvelles coordonnées
    position_curseur_lig = lig; 
    position_curseur_col = col;
}


void check_curseur(uint32_t lig, uint32_t col){
    if (col==159){
        if (lig == 89){
            defilement();
        }
        else {
            place_curseur(lig+1, 0);
        }
    }
    else {
        place_curseur(lig,col+1);
    }
}