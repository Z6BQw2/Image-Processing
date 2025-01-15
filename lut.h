#ifndef __LUT__
#define __LUT__

#include "pictures.h"

typedef struct lut_base* lut;

/*
 @requires
 @assigns Une LUT
 @ensures Retournes une LUT de taille demandée
*/
lut create_lut(int size);

/*
 @requires Rien
 @assigns Libère la LUT
*/
void nettoyage_lut(lut* lt);

/*
 @requires Rien
 @assigns Rien
 @ensures Applique la LUT à la picture
*/
picture apply_lut(picture p, lut l);

/*
 @requires Rien
 @assigns Rien
 @ensures Assigne la valeur à la LUT à l'indice donné
*/
void set_lut_value(lut lt, int index, int value);

#endif