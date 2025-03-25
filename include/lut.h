#ifndef _LUT_H
#define _LUT_H

#include "pictures.h"

struct lut {
    int n;
    int* value;
};

typedef struct lut lut;

/*  @requires an integer corresponding to the LUT size;
    @assigns nothing;
    @ensures returns a LUT initialized with the value 0; */
lut create_lut(int);

/*  @requires a LUT given by reference;
    @assigns nothing;
    @ensures clean the memory given for the LUT; */
void clean_lut(lut*);

/*  @requires a LUT and a picture p;
    @assigns nothing;
    @ensures returns a picture with the LUT applied to it; */
picture apply_lut(lut,picture);

#endif