#include "lut.h"
#include <stdlib.h>
#include "pictures.h"

typedef struct lut_base {
    int size;
    int *tab;
} *lut;

lut create_lut(int size){
    lut res = malloc(sizeof(struct lut_base));
    res->size = size;
    res->tab = calloc(sizeof(int), size);
    return res;
}

void nettoyage_lut(lut* lt){
    free((*lt)->tab);
    free(*lt);
}

picture apply_lut(picture p, lut l){
    for (int i=0; i < p.width * p.height * p.channels; i++){
        p.data[i] = l->tab[p.data[i]];
    }
    return p;
}


// Car on ne peut pas lt->tab sur un type abstrait
void set_lut_value(lut lt, int index, int value){
    if (index >= 0 && index < lt->size) {
        lt->tab[index] = value;
    }
}