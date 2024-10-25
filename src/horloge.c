#include <inttypes.h>
#include <stdlib.h>
#include "font.h"
#include "string.h"
#include "affichage.h"

void write_timer(const char *s){
    int len = strlen(s);
    for (int i=0; i<len; i++){
        ecrit_car(0, 1280-len+i, s[i]);
    }
}

trap_handler(){
    __asm__("csrw mcause, %d":: "r"(int 7));
}
