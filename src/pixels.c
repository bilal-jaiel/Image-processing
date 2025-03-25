#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pictures.h"
#include "pixels.h"
#include "filename.h"

/*  @requires three integers corresponding respectively to the pixel's position, the channel number, and the byte position;
    @assigns nothing;
    @ensures returns the position of the byte in the picture; */
int pixel(int comp, int nb_chan, int c){
    return comp*nb_chan+c;
}