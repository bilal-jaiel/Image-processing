#ifndef _PICTURES_H
#define _PICTURES_H

typedef unsigned char byte;

extern const int MAX_BYTE;

struct picture {
    int width;
    int height;
    int channel;
    byte* tbl;
};

typedef struct picture picture;

/*  @requires filename the name of an existing .pgm or .ppm file;
    @assigns nothing;
    @ensures returns a picture filled with the filename data. On failure it returns an error message; */
picture read_picture(char* filename);

/*  @requires filename the name of a .pgm or .ppm file. p a valid picture structure;
    @assigns the content of filename;
    @ensures write the content of p in filename. On success returns null. On failure returns -1; */
int write_picture(picture p, char * filename);

/*  @requires filename the name of a file;
    @assigns nothing;
    @ensures checks if filename can be open reading. On success returns a file descriptor for reading. On failure returns NULL; */
FILE* canopen(char* filename);

/*  @requires filename the name of a file;
    @assigns nothing;
    @ensures checks if filename can be open writing. On success returns a file descriptor for writing. On failure returns NULL; */
FILE* canwrite(char* filename);

/*  @requires three unsigned integers representing respectively the width, height and number channels of a picture and the maximum of a byte;
    @assigns nothing;
    @ensures returns a picture initialized with the parameters provided to the function; */
picture create_picture(int width, int height, int channels, byte max);

/*  @requires p a pointer to a picture;
    @assigns p;
    @ensures removes data from p; */
void clean_picture(picture* p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns a copy of the picture p; */
picture copy_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns 1 if p is empty 0 otherwise; */
int is_empty_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns 1 if p is a gray picture 0 otherwise; */
int is_gray_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures returns 1 if p is a color picture 0 otherwise; */
int is_color_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures print the string: “(<width> x <height> x <channels>)” where <width>, <height> and <channels> are to be replaced by their respective values in p.; */
void info_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures on sucess it returns p converted into colored format. On failure it returns an empty picture; */
picture convert_to_color_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures on sucess it returns p converted into gray format. On failure it returns an empty picture; */
picture convert_to_gray_picture(picture p);

/*  @requires p a picture;
    @assigns nothing;
    @ensures on sucess it returns a picture table which contains the R V B channels of p. On failure it returns NULL; */
picture* split_picture(picture p);

/*  @requires three gray pictures representing the R V B components of a colored picture;
    @assigns nothing;
    @ensures on sucess it returns the composed colored picture. On failure it returns an empty picture; */
picture merge_picture(picture red, picture green, picture blue);

/*  @requires a picture p and a double factor which is the factor to be applied to the pixel values of the image;
    @assigns nothing;
    @ensures returns the brightened image; */
picture brighten_picture(picture p, double factor);

/*  @requires a picture p and an integer representing the number of pixels chosen randomly;
    @assigns nothing;
    @ensures returns the image containing the melted pixels; */
picture melt_picture(picture p, int number);

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the inversed image; */
picture inverse_picture(picture p);

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the normalized picture; */
picture normalize_dynamic_picture(picture p);

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the levels adjusted picture; */
picture set_levels_picture(picture p, byte nb_levels);

/*  @requires two pictures;
    @assigns nothing;
    @ensures returns the image where each pixel contains the normalized differences between the two pictures; */
picture soustr_picture(picture p1, picture p2);

/*  @requires two pictures;
    @assigns nothing;
    @ensures returns an image where each pixel is the result of the multiplication of the corresponding pixels from the two images; */
picture mult_picture(picture p1, picture p2);

/*  @requires three pictures;
    @assigns nothing;
    @ensures returns an image where each pixel is a blend of the first two images based on the corresponding pixel values of the third image; */
picture mix_picture(picture p1, picture p2,picture p3);

/*  @requires an integer;
    @assigns nothing;
    @ensures returns a random integer between 0 and the integer given; */
int rand_up_to(int);

/*  @requires a picture p;
    @assigns nothing;
    @ensures returns the number of bytes in the picture p; */
int byte_number(picture);

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-up picture p with the given factor, using nearest neighbor interpolation; */
picture grow_size_nearest_neighbor_interpolation(picture p, double factor);

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-down picture p with the given factor, using nearest neighbor interpolation; */
picture reduce_size_nearest_neighbor_interpolation(picture p, double factor);

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-up picture p with the given factor, using bi-linear interpolation; */
picture grow_size_bi_linear_interpolation(picture p, double factor);

/*  @requires a picture p and a double factor;
    @assigns nothing;
    @ensures returns the scaled-down picture p with the given factor, using bi-linear interpolation; */
picture reduce_size_bi_linear_interpolation(picture p, double factor);

#endif