#include "pictures.h"
#include <stdio.h>
#include "lut.h"
#include <time.h>
#include <stdlib.h>
#include "filename.h"
#include <string.h>
#include "pixels.h"

/*
 @requires filename != NULL
 @assigns Une picture
 @ensures Retourne l'image valide ou une 0x0 non_ini en cas d'erreur
*/
picture read_picture(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return create_picture(0, 0, non_ini);
    }

    char magic_num[3];
    channel channels;
    fgets(magic_num, 3, file);
    if (!strcmp(magic_num, "P5")) channels = gris;
    else if (!strcmp(magic_num, "P6")) channels = couleur;
    else {
        fprintf(stderr, "Erreur : Type d'image invalide %s\n", magic_num);
        fclose(file);
        return create_picture(0, 0, non_ini);
    }

    char buff[255];
    fgets(buff, 2, file); // skip la première ligne

    int width;
    int height;

    while (fgets(buff, 255, file)) {
        if (buff[0] != '#') { //Si c'est pas des commentaires
            sscanf(buff, "%d %d", &width, &height); //Récupère les infos (Dim)
            break;
        }
    }

    int max_value;

    while (fgets(buff, 255, file)) { //Si c'est pas des commentaires
        if (buff[0] != '#') { //Récupère les infos (Max_value)
            sscanf(buff, "%d", &max_value);
            break;
        }
    }

    if (max_value < 1 || max_value > 255 || width < 1 || height < 1){
        fprintf(stderr, "Erreur : format invalide %d * %d, max %d\n", width, height, max_value);
        fclose(file);
        return create_picture(0, 0, non_ini);
    }

    picture img = create_picture(width, height, channels);

    if (max_value != 255) {
        for (int i = 0; i < img.width * img.height * img.channels; i++) {
            img.data[i] = (img.data[i] * 255) / max_value; //Insère les pixels
        }
    }


    if (fread(img.data, sizeof(byte), width * height * channels, file) != width * height * channels) {
        fprintf(stderr, "Erreur : Données d'image incomplètes.\n");
        clean_picture(&img);
        fclose(file);
        return create_picture(0, 0, non_ini);
    }

    fclose(file);

    return img;
}

/*
 @requires p.data != NULL, filename != NULL
 @assigns Écrit dans le fichier
 @ensures Retourne 0 si succès, code d’erreur sinon
*/
int write_picture(picture p, char * filename){
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }    

    char magic_num[4];

    //Test quel type d'image
    if (p.channels == gris) strcpy(magic_num, "P5\n");
    else if (p.channels == couleur) strcpy(magic_num, "P6\n");
    else {
        fprintf(stderr, "Erreur : Type d'image invalide %d\n", p.channels);
        fclose(file);
        return 2;
    }

    int max_value = 255;

    //\n pour ne pas faire plusieurs fprintf
    fprintf(file, "%s%d %d\n%d\n", magic_num, p.width, p.height, max_value);

    if (fwrite(p.data, sizeof(byte), p.width * p.height * p.channels, file) != p.width * p.height * p.channels) {
        fprintf(stderr, "Erreur : Écriture des pixels échouée.\n");
        fclose(file);
        return 3;
    }

    fclose(file);
    return 0;
}

/*
 @requires (width, height) >= 0
 @assigns Une picture
 @ensures Retourne une picture initialisée
*/
picture create_picture(unsigned int width, unsigned int height, channel channels){
    picture res;
    res.width = width;
    res.height = height;
    res.channels = channels;
    res.data = malloc(width * height * channels * sizeof(byte));
    return res;
}

/*
 @requires p != NULL
 @assigns Libère p->data
 @ensures p devient 0x0 non_ini
*/
void clean_picture(picture * p){
    p->width = 0;
    p->height = 0;
    p->channels = non_ini;
    free(p->data);
    p->data = NULL;
}

/*
 @requires p.data != NULL
 @assigns Une picture
 @ensures Retourne une copie de p
*/
picture copy_picture(picture p) {
    picture res = create_picture(p.width, p.height, p.channels);
    for (int i = 0; i < p.width * p.height * p.channels; i++)
        res.data[i] = p.data[i];
    return res;
}

/*
 @assigns Rien
 @ensures Retourne 1 si vide, sinon 0
*/
int is_empty_picture(picture p){
    return !(p.width && p.height && p.channels && p.data);
}

/*
 @assigns Rien
 @ensures Retourne 1 si gris, 0 sinon
*/
int is_gray_picture(picture p){
    return (p.channels == gris);
}

/*
 @assigns Rien
 @ensures Retourne 1 si couleur, 0 sinon
*/
int is_color_picture(picture p){
    return (p.channels == couleur);
}

/*
 @assigns Rien
 @ensures Affiche (width, height, channels)
*/
void info_picture(picture p){
    printf("(%d x %d x %d)\n", p.width, p.height, p.channels);
}

/*
 @assigns Une picture
 @ensures Convertit p en couleur (ou en copie si déjà couleur)
*/
picture convert_to_color_picture(picture p){
    if (p.channels == couleur) return copy_picture(p);
    picture res = create_picture(p.width, p.height, couleur);
    for (int i = 0; i < (int)(p.width * p.height); i++) {
        res.data[3*i] = p.data[i];
        res.data[3*i+1] = p.data[i];
        res.data[3*i+2] = p.data[i];
    }
    return res;
}

/*
 @assigns Une picture
 @ensures Convertit p en gris (ou en copie si déjà gris)
*/
picture convert_to_gray_picture(picture p) {
    if (p.channels == gris) return copy_picture(p);
    picture res = create_picture(p.width, p.height, gris);
    for (int i = 0; i < p.width * p.height; i++) res.data[i] = 0.299 * p.data[3*i] + 0.587 * p.data[3*i + 1] + 0.114 * p.data[3*i + 2];
    return res;
}

/*
 @assigns 1 ou 3 pictures
 @ensures Retourne la séparation R/G/B si couleur, sinon un seul élément
*/
picture * split_picture(picture p){
    if (p.channels == gris) {
        picture *tab = malloc(sizeof(picture));
        tab[0] = copy_picture(p);
        return tab;
    }
    picture *res = malloc(3*sizeof(picture));
    res[0] = create_picture(p.width, p.height, gris); // Crée un élément par composante
    res[1] = create_picture(p.width, p.height, gris);
    res[2] = create_picture(p.width, p.height, gris);
    for (int i = 0; i < p.width * p.height * p.channels; i++) {
        if (i % 3 == 0) res[0].data[i/3] = p.data[i];
        else if (i % 3 == 1) res[1].data[i/3] = p.data[i];
        else res[2].data[i/3] = p.data[i];
    }
    return res;
}

/*
 @assigns Rien
 @ensures Compare les dimensions/canaux de p1 et p2
*/
int piccmp(picture p1, picture p2){
    return (p1.width != p2.width || p1.height != p2.height || p1.channels != p2.channels || p1.data == NULL || p2.data == NULL);
}

/*
 @assigns Une picture
 @ensures Fusion R,G,B en une image couleur ou 0x0 si incompatible
*/
picture merge_picture(picture red, picture green, picture blue){
    if (piccmp(red, green) || piccmp(red, blue)) 
        return create_picture(0, 0, non_ini);
    picture res = create_picture(red.width, red.height, couleur);
    for (int i = 0; i < red.width * red.height * 3; i++) res.data[i] = (i % 3 == 0) ? red.data[i / 3]: (!i % 3 + 1 == 0) ? green.data[i / 3] : blue.data[i / 3]; //Test si on est rouge, bleu ou vert
    return res;
}

/*
 @assigns Une LUT, une picture
 @ensures Inverse l'image
*/
picture inverse_picture(picture p) {
    lut lt = create_lut(256);
    for (int i = 0; i < 256; i++) set_lut_value(lt, i, 255 - i);
    picture res = apply_lut(p, lt);
    nettoyage_lut(&lt);
    return res;
}

/*
 @assigns Une LUT, une picture
 @ensures Etale les pixels entre 0 et 255
*/
picture normalize_dynamic_picture(picture p){
    int min = p.data[0], max = p.data[0];
    for (int i = 1; i < p.width * p.height * p.channels; i++) {
        if (p.data[i] < min) min = p.data[i];
        else if (p.data[i] > max) max = p.data[i];
    }

    lut lt = create_lut(256);
    if (max == min) for (int i = 0; i < 256; i++) set_lut_value(lt, i, 255);
    else for (int i = 0; i < 256; i++) set_lut_value(lt, i, (i - min) * 255 / (max - min));

    picture res = apply_lut(p, lt);
    nettoyage_lut(&lt);
    return res;
}

/*
 @assigns Une picture
 @ensures Éclaircit l'image
*/
picture brighten_picture(picture p, double factor) {
    picture res = copy_picture(p);
    for (int i = 0; i < res.width * res.height * res.channels; i++) { // Détermine l'écart avec le blanc pour uniformiser l'éclairage
        int gap_to_white = 255 - res.data[i];
        int new_value = 255 - gap_to_white / factor;
        if (new_value > 255) res.data[i] = 255;
        else if (new_value < 0) res.data[i] = 0;
        else res.data[i] = new_value;
    }
    return res;
}

/*
 @assigns p.data
 @ensures "Fusion" aléatoire de pixels (fonte)
*/
picture melt_picture(picture p, int number) {
    if (p.channels == 1) {
        for (int i = 0; i < number; i++) {
            int rand_idx = rand() % (p.width * p.height - p.width) + p.width; // skip la 1ère ligne

            if (p.data[rand_idx - p.width] < p.data[rand_idx]) p.data[rand_idx] = p.data[rand_idx - p.width];
        }
    } else {
        for (int i = 0; i < number; i++) {
            int rand_idx = (rand() % (p.width * p.height - p.width)) * 3 + p.width * 3; // skip la 1ère ligne

            if (0.299 * p.data[rand_idx - p.width * 3] + 0.587 * p.data[rand_idx + 1 - p.width * 3] + 0.114 * p.data[rand_idx + 2 - p.width * 3] < 0.299 * p.data[rand_idx] + 0.587 * p.data[rand_idx + 1] + 0.114 * p.data[rand_idx + 2]) { // Compare avec le pixel du dessus
                p.data[rand_idx] = p.data[rand_idx - p.width * 3];
                p.data[rand_idx + 1] = p.data[rand_idx + 1 - p.width * 3];
                p.data[rand_idx + 2] = p.data[rand_idx + 2 - p.width * 3]; 
            }
        }
    }

    return p;
}

/*
 @assigns Une picture
 @ensures Retourne la diff absolue pixel à pixel
*/
picture diff_pictures(picture p1, picture p2){
    if (piccmp(p1, p2)) {
        fprintf(stderr, "Erreur : images incompatibles\n");
        return create_picture(0, 0, non_ini);
    }
    picture res = create_picture(p1.width, p1.height, p1.channels);
    for (int i = 0; i < res.width * res.height * res.channels; i++)
        res.data[i] = (p1.data[i] > p2.data[i]) ? p1.data[i] - p2.data[i] : p2.data[i] - p1.data[i]; // Valeur absolu
    return res;
}

/*
 @assigns Une picture
 @ensures Produit pixel à pixel p1*p2, puis normalise
*/
picture mult_picture(picture p1, picture p2){
    if (piccmp(p1, p2)) {
        fprintf(stderr, "Erreur : images incompatibles\n");
        return create_picture(0, 0, non_ini);
    }
    picture res = create_picture(p1.width, p1.height, p1.channels);
    for (int i = 0; i < res.width * res.height * res.channels; i++) //Applique la multiplication pixel par pixel
        res.data[i] = p1.data[i] * p2.data[i];
    return normalize_dynamic_picture(res);
}

/*
 @assigns Une picture
 @ensures Mix p1/p2 pondéré par p3/255
*/
picture mix_picture(picture p1, picture p2, picture p3){
    if (piccmp(p1, p2) || piccmp(p1, p3)) {
        fprintf(stderr, "Erreur : images incompatibles\n");
        return create_picture(0, 0, non_ini);
    }
    picture res = create_picture(p1.width, p1.height, p1.channels);
    for (int i = 0; i < res.width * res.height * res.channels; i++) {
        int alpha = p3.data[i] / 255; 
        res.data[i] = (1 - alpha) * p1.data[i] + alpha * p2.data[i];
    }
    return res;
}

/*
 @assigns Une picture
 @ensures Redimensionne p
*/
picture resample_picture_nearest(picture p, unsigned int width, unsigned int height) {
    picture res = create_picture(width, height, p.channels);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int c = 0; c < p.channels; c++) { // Normalise la grille pour passer entre 2 dimensions proportionnellement 
                int ni = i * p.height / height;
                int nj = j * p.width / width;
                res.data[(i*width + j)*p.channels + c] 
                    = p.data[(ni*p.width + nj)*p.channels + c];
            }
        }
    }
    return res;
}

/*
 @assigns Une picture
 @ensures Redimensionne p
*/
picture resample_picture_bilinear(picture p, unsigned int width, unsigned int height){
    picture res = create_picture(width, height, p.channels);

    double x_scale_factor = (double)p.width / width;
    double y_scale_factor = (double)p.height / height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double src_x = x * x_scale_factor;
            double src_y = y * y_scale_factor;

            int x1 = (int)src_x;
            int y1 = (int)src_y;
            int x2 = (x1 + 1 < p.width - 1) ? x1 + 1 : p.width - 1;
            int y2 = (y1 + 1 < p.height - 1) ? y1 + 1 : p.height - 1;

            double alpha = src_x - x1;
            double beta = src_y - y1;

            for (int c = 0; c < p.channels; c++) { // Passe par les cannaux pour le cas en couleur 
                res.data[(y * width + x) * p.channels + c] = (
                    (1 - alpha) * (1 - beta) * p.data[(y1 * p.width + x1) * p.channels + c]
                    + alpha * (1 - beta) * p.data[(y1 * p.width + x2) * p.channels + c]
                    + (1 - alpha) * beta * p.data[(y2 * p.width + x1) * p.channels + c]
                    + alpha * beta * p.data[(y2 * p.width + x2) * p.channels + c]
                );
            }
        }
    }

    return res;    
}
/*
 @assigns Une LUT, une picture
 @ensures Discrétise l'image
*/
picture set_levels_picture(picture p, byte nb_levels) {
    if (nb_levels < 2) {
        fprintf(stderr, "Erreur : nb_levels hors intervalle\n");
        return create_picture(0, 0, non_ini);
    }
    lut lt = create_lut(256);
    int step = 255 / (nb_levels - 1);
    for (int i = 0; i < 256; i++) { //Définie la LUT
        int level = (i / step) * step;
        set_lut_value(lt, i, level);
    }
    picture res = apply_lut(copy_picture(p), lt);
    nettoyage_lut(&lt);
    return res;
}


/*
int main() {
    // Exemples d'utilisation
    picture img = create_picture(100, 100, couleur);
    info_picture(img);
    printf("%d\n", is_empty_picture(img));
    clean_picture(&img);
    info_picture(img);
    picture res = copy_picture(img);
    info_picture(res);
    printf("%d\n", is_empty_picture(res));
    return 0;
}
*/