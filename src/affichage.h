#ifndef _AFFICHAGE_H_
#define _AFFICHAGE_H_

//Affichage d'un pixel aux coordonnées (x, y) dans une couleur spécifiée 
void pixel(uint32_t x, uint32_t y, uint32_t couleur);

//Ecrit le caractère c dans la couleur spécifiée à la ligne et colonne rentrée en argument
void ecrit_car(uint32_t lig, uint32_t col, char c);

//Effacage de tout l'écran à partir de la ligne 1 et remise du curseur en position initiale
void efface_ecran(void);

//Placement du curseur à lig et col
void place_curseur(uint32_t lig, uint32_t col);

//Permet de faire défiler le texte vers le haut en supprimant la première ligne et en placant le curseur au début de la dernière
void defilement(void);

//Permet de tratier un caractère correctement suivant si c'est un caractère de texte ou spécial
void traite_car(char c);

//Affiche à l'écran du terminal la châine de caractères *s de longueur len. Fonction finale du fichier
void console_putbytes(const char *s, int len);

#endif