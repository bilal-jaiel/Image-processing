#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lut.h"
#include "pictures.h"
#include "filename.h"

/*  @requires an integer corresponding to the LUT size;
    @assigns nothing;
    @ensures returns a LUT initialized with the value 0; */
lut create_lut(int n){
    lut l;
    l.n = n;
    l.value = calloc(n,sizeof(int));
    return l;
}

/*  @requires a LUT given by reference;
    @assigns nothing;
    @ensures clean the memory given for the LUT; */
void clean_lut(lut* l){
    (*l).n = 0;
    free(l->value);
    l->value = NULL;
}

/*  @requires a LUT and a picture p;
    @assigns nothing;
    @ensures returns a picture with the LUT applied to it; */
picture apply_lut(lut l,picture p){
    picture p_lut = copy_picture(p);
    for(int i=0; i<p_lut.height*p_lut.width*p_lut.channel; i++){
        // Calcule de la valeur du byte donnée par la LUT à l'indice de la valeur.
        p_lut.tbl[i]=l.value[(p_lut.tbl[i]*l.n)/255];
    }
    return p_lut;
}