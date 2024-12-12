#include "include.h"

//Par défaut : curseur en (0,0)
extern uint32_t position_curseur_col;
extern uint32_t position_curseur_lig;


void pixel(uint32_t x, uint32_t y, uint32_t couleur){

    uint32_t *ptr = (uint32_t *) (0x80000000);
    ptr += (x + y*1280);
    *ptr = couleur;
}


void ecrit_car(uint32_t lig, uint32_t col, char c){

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
                pixel(col * 8 + j, lig * 8 + i, 0x0FFFFFFF);
            }
        }
    }

    //Repositionnement du curseur
    check_curseur(lig, col);
}


void traite_car(char c){

    //Caractère normal
    if (32 <= c && c <= 126 ){
        ecrit_car(position_curseur_lig, position_curseur_col, c);
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
            defilement();
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
        traite_car(s[i]);
    }
}


void efface_ecran(void){
    
    //Parcours de tout l'ecran
    for (int i = 0; i < 720; i++){
        for (int j = 0; j < 1280; j++){
            pixel(j, i, 0x00000000); //Coloriage du pixel en noir
        }
    }

    //Réinitialisation de la position du curseur
    place_curseur(1, 0);
}


void defilement(void){

    //Sinon, le curseur est toujours considéré comme à la ligne 89 et donc n'efface pas celui qui a été remonté ligne 88
    position_curseur_lig -= 1;

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