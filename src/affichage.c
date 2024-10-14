
#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include <inttypes.h>
#include "font.h"


void pixel(uint32_t x, uint32_t y, uint32_t couleur){
    uint32_t* ptr = (uint32_t*) (0x80000000 + (x + y * 1280)*4);
    *ptr = couleur;
}

uint8_t chercher(char c){
	uint8_t car[8];
	int i;
	for (i=0; i<8; i++){
		car[i]=font8x8_basic[c][i];
	}
	return car;
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t couleur){
    uint8_t car[8] = chercher(c);
    uint32_t* ptr = (uint8_t*) (0x80000000 + (lig + col * 1280)*4);
    int i, j;
    for (i=0; i<8; i++){
        int positions[8];
        for (j=0; i<8; j++){
            positions[j]=(car[j]>>j) & 0xFF;
            if (positions[j]==1){ *ptr = couleur;}
            ptr+=1;
        }
    } 
}
