#include <stdio.h>
#include <stdlib.h>
#include "filename.h"
#include <string.h>
#include "pictures.h"
#include "pixels.h"



int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <fichier_image_gray> <fichier_image_color> <mask> \n", argv[0]);
        return 1;
    }


    //Création des vars associés aux images pour ouverture
    char *filename_gray = argv[1];
    char *filename_color = argv[2];
    char *filename_mask = argv[3];

    //Ouverture
    picture img_gray = read_picture(filename_gray);
    picture img_color = read_picture(filename_color);
    picture img_mask = read_picture(filename_mask);

    if (is_empty_picture(img_gray) || is_empty_picture(img_color)) {
        fprintf(stderr, "Erreur : Image lue vide.\n");
        return 2;
    }

    //Divers Tests demandés
    write_picture(convert_to_color_picture(copy_picture(img_gray)), "Lenna_gray_convert_color.ppm");
    write_picture(convert_to_gray_picture(copy_picture(img_color)), "Lenna_color_convert_gray.pgm");
    
    picture * split = split_picture(copy_picture(img_color));
    write_picture(convert_to_gray_picture(copy_picture(split[0])), "Lenna_color_red.pgm");
    write_picture(convert_to_gray_picture(copy_picture(split[1])), "Lenna_color_green.pgm");
    write_picture(convert_to_gray_picture(copy_picture(split[2])), "Lenna_color_blue.pgm");
    
    write_picture(brighten_picture(copy_picture(img_color), 1.5), "Lenna_color_brighten.ppm");
    write_picture(brighten_picture(copy_picture(img_gray), 1.5), "Lenna_gray_brighten.pgm");
    
    write_picture(melt_picture(copy_picture(img_color), img_color.width * img_color.height * img_color.channels * 5), "Lenna_color_melted.ppm");
    write_picture(melt_picture(copy_picture(img_gray), img_gray.width * img_gray.height * img_gray.channels * 5), "Lenna_gray_melted.pgm");
    
    picture img_color_inverse = inverse_picture(copy_picture(img_color));
    picture img_gray_inverse = inverse_picture(copy_picture(img_gray));

    write_picture(img_color_inverse, "Lenna_color_inverse.ppm");
    write_picture(img_gray_inverse, "Lenna_gray_inverse.pgm");   
    
    write_picture(normalize_dynamic_picture(copy_picture(img_gray)), "Lenna_gray_dynamic.pgm"); 
    
    write_picture(merge_picture(normalize_dynamic_picture(copy_picture(split[0])), normalize_dynamic_picture(copy_picture(split[1])), normalize_dynamic_picture(copy_picture(split[2]))), "Lenna_color_dynamic.ppm"); 
    
    write_picture(set_levels_picture(copy_picture(img_gray), 8), "Lenna_gray_levels.pgm");
    write_picture(set_levels_picture(copy_picture(img_color), 8), "Lenna_color_levels.ppm");  

    write_picture(resample_picture_nearest(copy_picture(img_gray), img_gray.width / 1.36, img_gray.height / 1.36), "Lenna_gray_smaller_nearest.pgm");
    write_picture(resample_picture_nearest(copy_picture(img_color), img_color.width / 1.36, img_color.height / 1.36), "Lenna_color_smaller_nearest.ppm");  
    write_picture(resample_picture_bilinear(copy_picture(img_gray), img_gray.width / 1.36, img_gray.height / 1.36), "Lenna_gray_smaller_bilinear.pgm");
    write_picture(resample_picture_bilinear(copy_picture(img_color), img_color.width / 1.36, img_color.height / 1.36), "Lenna_color_smaller_bilinear.ppm"); 

    picture lenna_large_nearest_color = resample_picture_nearest(copy_picture(img_color), 1.36 * img_color.width, 1.36 * img_color.height);
    picture lenna_large_nearest_gray = resample_picture_nearest(copy_picture(img_gray), 1.36 * img_gray.width, 1.36 * img_gray.height);
    picture lenna_large_bilinear_color = resample_picture_bilinear(copy_picture(img_color), 1.36 * img_color.width, 1.36 * img_color.height);
    picture lenna_large_bilinear_gray = resample_picture_bilinear(copy_picture(img_gray), 1.36 * img_gray.width, 1.36 * img_gray.height);    
    write_picture(lenna_large_nearest_gray, "Lenna_gray_larger_nearest.pgm");
    write_picture(lenna_large_nearest_color, "Lenna_color_larger_nearest.ppm");  
    write_picture(lenna_large_bilinear_gray, "Lenna_gray_larger_bilinear.pgm");
    write_picture(lenna_large_bilinear_color, "Lenna_color_larger_bilinear.ppm"); 

    write_picture(normalize_dynamic_picture(diff_pictures(lenna_large_nearest_color, lenna_large_bilinear_color)), "Lenna_color_difference.ppm");
    write_picture(normalize_dynamic_picture(diff_pictures(lenna_large_nearest_gray, lenna_large_bilinear_gray)), "Lenna_gray_difference.pgm");

    picture img_mask_color = convert_to_color_picture(copy_picture(img_mask));

    write_picture(mult_picture(copy_picture(img_gray), copy_picture(img_mask)), "Lenna_gray_product.pgm");
    write_picture(mult_picture(copy_picture(img_color), copy_picture(img_mask_color)), "Lenna_color_product.ppm");

    write_picture(mix_picture(copy_picture(img_gray_inverse), copy_picture(img_gray), copy_picture(img_mask)), "Lenna_gray_mixture.pgm");
    write_picture(mix_picture(copy_picture(img_color_inverse), copy_picture(img_color), copy_picture(img_mask_color)), "Lenna_color_mixture.ppm");

    clean_picture(&lenna_large_nearest_color);
    clean_picture(&lenna_large_nearest_gray);
    clean_picture(&lenna_large_bilinear_color);
    clean_picture(&lenna_large_bilinear_gray);
    clean_picture(&split[0]);
    clean_picture(&split[1]);
    clean_picture(&split[2]);
    clean_picture(split);
    clean_picture(&img_gray);
    clean_picture(&img_color);
    clean_picture(&img_mask);
    clean_picture(&img_mask_color);
    clean_picture(&img_gray_inverse);
    clean_picture(&img_color_inverse);    

    return 0;
}