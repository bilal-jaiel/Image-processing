#ifndef _PIXELS_H
#define _PIXELS_H

#include "pictures.h"

enum Color {RED, GREEN, BLUE};

/*  @requires three integers corresponding respectively to the pixel's position, the channel number, and the byte position;
    @assigns nothing;
    @ensures returns the position of the byte in the picture; */
int pixel(int, int, int);

#endif