#include <inttypes.h>
#include <stdlib.h>
#include "font.h"
#include "string.h"

#define NB_OCTETS 40960

//Par défaut : curseur en (1,0), fond en noir et texte en blanc
uint32_t position_curseur_col = 0;
uint32_t position_curseur_lig = 1;
// uint32_t fond = 0x00000000;
// uint32_t couleur = 0x0FFFFFFF;

void pixel(uint32_t x, uint32_t y, uint32_t color){

    //Ecrire avec la couleur spécifiée si c'est le cas
    // if (color != NULL){
    //     couleur = color;
    // }

    uint32_t *ptr = (uint32_t *) (0x80000000);
    ptr += (x + y*1280);
    *ptr = color;
}

void place_curseur(uint32_t lig, uint32_t col){

    //Effacement du curseur précédent
    for (int i = 0; i < 8; i++){
        pixel(position_curseur_col*8 +i, position_curseur_lig*8+7, 0x0000000);
    }

    //Placement du nouveau curseur à lig et col
    for (int i = 0; i < 8; i++){
        pixel(col*8+i, lig*8+7, 0x0FFFFFFF);
    }

    //Mise à jour de ses nouvelles coordonnées
    position_curseur_lig = lig; 
    position_curseur_col = col;
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t color, uint32_t back){

    //Coloriage du fond si spécifié
    // if (back != NULL){
    //     for (int i = 0; i < 720; i++){
    //         for (int j = 0; j < 1280; j++){
    //             pixel(j, i, back);
    //         }
    //     }
    //     fond = back;
    // }

    //Effacement de pixels potentiels à l'emplacement du caractère
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            pixel(col * 8 + j, lig * 8 + i, 0x00000000);
        }
    }

    //Dessin du caractère 
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){

            if (((font8x8_basic[(int) c][i] >> j) & 1) == 1){
                pixel(col * 8 + j, lig * 8 + i, color);
            }
        }
    }
    //Coloriage de la 8e ligne du caractère avec la couleur du fond
    // for (int j = 0; j < 8; j++){
    //     pixel(col * 8 + j, lig * 8 + 8, 0x0FFFFFFF);
    // }

    //Repositionnement du curseur
    place_curseur(lig,col+1);
}

void efface_ecran(void){
    
    //Parcours de tout l'ecran
    for (int i = 0; i < 720; i++){
        for (int j = 0; j < 1280; j++){
            pixel(j, i, 0x00000000); //Coloriage du pixel avec la couleur du fond 
        }
    }

    //Réinitialisation de la position du curseur
    place_curseur(1, 0);
}

void defilement(void){

    //Déplacement des lignes 2 à 89 aux positions 1 à 88
    for (int i = 1; i < 89; i++){
        uint32_t* dest = (uint32_t*) (0x80000000 + i*8*4*1280);
        uint32_t* src = (uint32_t*) (0x80000000 + (i+1)*4*8*1280);
        memmove(dest, src, NB_OCTETS);
    }

    //Effacement de la dernière ligne
    for (int j = 0; j < 8; j++){
        for (int i = 0; i < 1280; i++){
            pixel(i, 712 + j, 0x00000000);
        }
    }

    //Replacement du curseur
    place_curseur(89, 0);
}

void traite_car(char c, uint32_t color){

    //Caractère normal
    if (32 <= c && c <= 126 ){
        ecrit_car(position_curseur_lig, position_curseur_col, c, color, 0x00000000);
    }

    if (c == '\b'){
        if (position_curseur_col > 0){
            place_curseur(position_curseur_lig, position_curseur_col - 1);
        }
        else {place_curseur(position_curseur_lig - 1, 159);}
    }

    if (c == '\t'){
        if (position_curseur_col < 153){
            place_curseur(position_curseur_lig, position_curseur_col + 7);
        }
        else {place_curseur(position_curseur_lig + 1, 0);}
    }

    if (c == '\n'){
        if (position_curseur_lig == 89){
            defilement()
        }
        else {place_curseur(position_curseur_lig + 1, 0);}
    }
    
    if (c == '\f'){
        efface_ecran();
    }

    if (c == '\r'){
        place_curseur(position_curseur_lig, 0);
    }
}

void console_putbytes(const char *s, int len){
    for (int i = 0; i < len; i++){
        traite_car(s[i], 0x0FFFFFFF);
    }
}

