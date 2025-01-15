#ifndef PICTURES_H
#define PICTURES_H
#define MAX_BYTE 255;
#include <stdlib.h>

typedef unsigned char byte;

typedef enum channel {
    non_ini = 0,
    gris = 1,
    couleur = 3,
} channel;

typedef struct picture {
    int width;
    int height;
    channel channels;
    byte* data;
} picture;

/*
 @requires filename != NULL
 @assigns Une picture
 @ensures Retourne l'image valide ou une 0x0 non_ini en cas d'erreur
*/
picture read_picture(char *filename);

/*
 @requires p.data != NULL, filename != NULL
 @assigns Écrit dans le fichier
 @ensures Retourne 0 si succès, code d’erreur sinon
*/
int write_picture(picture p, char * filename);

/*
 @requires (width, height) >= 0
 @assigns Une picture
 @ensures Retourne une picture initialisée
*/
picture create_picture(unsigned int, unsigned int, channel);

/*
 @requires p != NULL
 @assigns Libère p->data
 @ensures p devient 0x0 non_ini
*/
void clean_picture(picture* p);

/*
 @requires p.data != NULL
 @assigns Une picture
 @ensures Retourne une copie de p
*/
picture copy_picture(picture p);

/*
 @assigns Rien
 @ensures Retourne 1 si vide, sinon 0
*/
int is_empty_picture(picture p);

/*
 @assigns Rien
 @ensures Retourne 1 si gris, 0 sinon
*/
int is_gray_picture(picture p);

/*
 @assigns Rien
 @ensures Retourne 1 si couleur, 0 sinon
*/
int is_color_picture(picture p);

/*
 @assigns Rien
 @ensures Affiche (width, height, channels)
*/
void info_picture(picture p);

/*
 @assigns Une picture
 @ensures Convertit p en couleur (ou en copie si déjà couleur)
*/
picture convert_to_color_picture(picture p);

/*
 @assigns Une picture
 @ensures Convertit p en gris (ou en copie si déjà gris)
*/
picture convert_to_gray_picture(picture p);

/*
 @assigns 1 ou 3 pictures
 @ensures Retourne la séparation R/G/B si couleur, sinon un seul élément
*/
picture * split_picture(picture p);

/*
 @assigns Une picture
 @ensures Fusion R,G,B en une image couleur ou 0x0 si incompatible
*/
picture merge_picture(picture red, picture green, picture blue);

/*
 @assigns Rien
 @ensures Compare les dimensions/canaux de p1 et p2
*/
int piccmp(picture p1, picture p2);

/*
 @assigns Une LUT, une picture
 @ensures Inverse l'image
*/
picture inverse_picture(picture p);

/*
 @assigns Une LUT, une picture
 @ensures Etale les pixels entre 0 et 255
*/
picture normalize_dynamic_picture(picture p);

/*
 @assigns Une LUT, une picture
 @ensures Discrétise l'image
*/
picture set_levels_picture(picture p, byte nb_levels);

/*
 @assigns Une picture
 @ensures Éclaircit l'image
*/
picture brighten_picture(picture p, double factor);

/*
 @assigns p.data
 @ensures "Fusion" aléatoire de pixels (fonte)
*/
picture melt_picture(picture p, int number);

/*
 @assigns Une picture
 @ensures Retourne la diff absolue pixel à pixel
*/
picture diff_pictures(picture p1, picture p2);

/*
 @assigns Une picture
 @ensures Produit pixel à pixel p1*p2, puis normalise
*/
picture mult_picture(picture p1, picture p2);

/*
 @assigns Une picture
 @ensures Mix p1/p2 pondéré par p3/255
*/
picture mix_picture(picture p1, picture p2, picture p3);

/*
 @assigns Une picture
 @ensures Redimensionne p
*/
picture resample_picture_nearest(picture p, unsigned int width, unsigned int height);

/*
 @assigns Une picture
 @ensures Redimensionne p
*/
picture resample_picture_bilinear(picture p, unsigned int width, unsigned int height);

#endif

