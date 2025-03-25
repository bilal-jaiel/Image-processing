#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pictures.h"
#include "filename.h"

int main(int argc, char** argv){

    if (argc==1){
        fprintf(stderr,"Error : Usage : %s <file_name> <file_name> ...\n",argv[0]);
        return -1;
    }

    double factor;

    picture p;
    picture p1;
    picture p2;
    picture p3;
    picture p4;

    picture* l;

    for(int i = 1; i<argc; i++){

        p = read_picture(argv[i]);

        if(p.channel == 1){

            p1 = convert_to_color_picture(p);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"convert_color","ppm"));
            clean_picture(&p1);

            p1 = brighten_picture(p,1.5);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"brighten","pgm"));
            clean_picture(&p1);

            p1 = melt_picture(p,byte_number(p)*5);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"melted","pgm"));
            clean_picture(&p1);

            p1 = inverse_picture(p);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"inverse","pgm"));
            clean_picture(&p1);

            p1 = normalize_dynamic_picture(p);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"dynamic","pgm"));
            clean_picture(&p1);

            p1 = set_levels_picture(p,8);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"levels","pgm"));
            clean_picture(&p1);

            p1 = reduce_size_nearest_neighbor_interpolation(p,1.36);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"smaller_nearest","pgm"));
            clean_picture(&p1);

            p1 = reduce_size_bi_linear_interpolation(p,1.36);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"smaller_bilinear","pgm"));
            clean_picture(&p1);

            p1 = grow_size_nearest_neighbor_interpolation(p,1.36);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"larger_nearest","pgm"));

            p2 = grow_size_bi_linear_interpolation(p,1.36);
            write_picture(p2, concat_parts(".",name_from_path(argv[i]),"larger_bilinear","pgm"));
        
            p3 = soustr_picture(p1,p2);
            p4 = normalize_dynamic_picture(p3);
            clean_picture(&p3);
            clean_picture(&p2);
            clean_picture(&p1);
            write_picture(p4, concat_parts(".",name_from_path(argv[i]),"difference","pgm"));
            clean_picture(&p4);

            p1 = read_picture("Lenna_BW.pgm");
            factor = p.width/p1.width;
            p2 = reduce_size_nearest_neighbor_interpolation(p1,factor);
            clean_picture(&p1);
            
            p1 = mult_picture(p,p2);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"product","pgm"));

            p3 = inverse_picture(p1);
            clean_picture(&p1); 
            p1 = mix_picture(p3,p,p2);
            clean_picture(&p2);
            clean_picture(&p3);
            clean_picture(&p);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"mixture","pgm"));
            clean_picture(&p1);

        }
        if(p.channel == 3){

            p1 = convert_to_gray_picture(p);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"convert_gray","pgm"));
            clean_picture(&p1);

            l = split_picture(p);

            write_picture(l[0], concat_parts(".",name_from_path(argv[i]),"red","pgm"));

            write_picture(l[1], concat_parts(".",name_from_path(argv[i]),"green","pgm"));
            
            write_picture(l[2], concat_parts(".",name_from_path(argv[i]),"blue","pgm"));

            p1 = brighten_picture(p,1.5);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"brighten","ppm"));
            clean_picture(&p1);

            p1 = melt_picture(p,byte_number(p)*5);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"melted","ppm"));
            clean_picture(&p1);

            p1 = inverse_picture(p);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"inverse","ppm"));
            clean_picture(&p1);

            p1 = merge_picture(normalize_dynamic_picture(l[0]),normalize_dynamic_picture(l[1]),normalize_dynamic_picture(l[2]));
            p2 = convert_to_color_picture(p1);
            clean_picture(&p1);
            write_picture(p2, concat_parts(".",name_from_path(argv[i]),"dynamic","ppm"));
            clean_picture(&p2);
            clean_picture(&l[0]);
            clean_picture(&l[1]);
            clean_picture(&l[2]);
            free(l);

            p1 = set_levels_picture(p,8);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"levels","ppm"));
            clean_picture(&p1);
            
            p1 = reduce_size_nearest_neighbor_interpolation(p,1.36);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"smaller_nearest","ppm"));
            clean_picture(&p1);

            p1 = reduce_size_bi_linear_interpolation(p,1.36);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"smaller_bilinear","ppm"));
            clean_picture(&p1);

            p1 = grow_size_nearest_neighbor_interpolation(p,1.36);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"larger_nearest","ppm"));

            p2 = grow_size_bi_linear_interpolation(p,1.36);
            write_picture(p2, concat_parts(".",name_from_path(argv[i]),"larger_bilinear","ppm"));

            p3 = soustr_picture(p1,p2);
            p4 = normalize_dynamic_picture(p3);
            clean_picture(&p3);
            clean_picture(&p1);
            clean_picture(&p2);
            write_picture(p4, concat_parts(".",name_from_path(argv[i]),"difference","ppm"));
            clean_picture(&p4);

            p1 = read_picture("Lenna_BW.pgm");
            factor = p.width/p1.width;
            p2 = reduce_size_nearest_neighbor_interpolation(p1,factor);
            clean_picture(&p1);
            p3 = convert_to_color_picture(p2);
            clean_picture(&p2);

            p1 = mult_picture(p,p3);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"product","ppm"));

            p2 = inverse_picture(p1);
            clean_picture(&p1);
            p1 = mix_picture(p2,p,p3);
            clean_picture(&p2);
            clean_picture(&p);
            clean_picture(&p3);
            write_picture(p1, concat_parts(".",name_from_path(argv[i]),"mixture","ppm"));
            clean_picture(&p1);

        }
    }

    return 0;
}
