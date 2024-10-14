#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include <inttypes.h>
#include "font.h"
#include "string.h"


void pixel(uint32_t x, uint32_t y, uint32_t couleur){
    uint32_t* ptr = (uint32_t*) (0x80000000 + (x + y * 1280)*4);
    *ptr = couleur;
}

uint8_t chercher(char c){
	uint8_t car[8];
	int i;

    //Récupère les 8 octets du caractère sous forme de tableau 1D
	for (i=0; i<8; i++){
		car[i]=font8x8_basic[c][i];
	}
	return car;
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t couleur){
    uint8_t car[8] = chercher(c);
    uint32_t* ptr = (uint32_t*) (0x80000000 + (lig + col * 1280)*4);
    int i, j;

    for (i = 0; i < 8; i++){
        int pixel;
        for (j = 0; j < 8; j++){
            pixel=(car[i] >> j) & 0xFF; //Utilisation d'un masque pour récupérer le 1 ou le 0 à la position (i, j)

            if (pixel == 1){ *ptr = couleur;}
            ptr += 1;
        }
    }
}

void efface_ecran(void){
    uint32_t* ptr = (uint32_t*) (0x80000000);
    
    //Parcours de tout l'ecran
    for (int i = 0; i < 720; i++){
        for (int j = 0; j < 1280; j++){
            *ptr = 0x00000000;          //Mie à noir du pixel
            ptr += 1;  
        }
    }

    ptr = (uint32_t*) (0x80000000 + 1280 * 4); //Réinitialisation du placement du curseur
}

void place_curseur(uint32_t lig, uint32_t col){

    //Placement du curseur à lig et col
    uint32_t* ptr = (uint32_t*) (0x80000000 + (lig + col * 1280)*4);
}

void defilement(void){

    int nb_bytes = 3 681 280; //Nombre d'octets à déplacer (719 lignes x 1280 colonnes x 4 octets/pixel)
    uint32_t* dest = (uint32_t*) (0x80000000 + 1280 * 4);
    uint32_t* src = (uint32_t*) (0x80000000 + 719 * 1280 * 4);
    memmove(dest, src, nb_bytes);

    //Mise à noir de la dernière ligne
    uint32_t* ptr = (uint32_t*) (0x80000000 + 719 * 1280 * 4);
    for (int i = 0; i < 1280; i ++){
        ptr* = 0x00000000;
        ptr += 1;
    }
}

void traite_car(char c){
    
}