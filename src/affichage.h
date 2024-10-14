#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

//Affichage d'un pixel aux coordonnées (x, y) dans une couleur spécifiée 
void pixel(uint32_t x, uint32_t y, uint32_t couleur);

//Va chercher les 64 pixels du caractère c dans la matrice de front.c
uint8_t chercher(char c);

//Ecrit le caractère c dans la couleur spécifiée à la ligne et colonne rentrée en argument
void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t couleur);

//Effacage de tout l'écran à partir de la ligne 1 et remise du curseur en position initiale
void efface_ecran(void);

//Placement du curseur à lig et col
void place_curseur(uint32_t lig, uint32_t col);

#endif