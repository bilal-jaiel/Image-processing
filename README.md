# Image Processing
PPM/PBM image processing in C
Development of a C program to manipulate images in PPM and PBM formats. Performance optimization and efficient memory management (pointers, dynamic allocation).

## Author: Bilâl Jaiel  
**Date:** January 4, 2025  

## Table of Contents
1. [Introduction](#introduction)
2. [Project Structure](#project-structure)
3. [Technical Choices and Solutions](#technical-choices-and-solutions)
   - [General Remarks](#general-remarks)
   - [LUT Implementation](#lut-implementation)
   - [Image Processing Functions](#image-processing-functions)
   - [Main Program](#main-program)
4. [Conclusion](#conclusion)

## Introduction
The goal of this project is to implement various image processing techniques on grayscale and color images.

## Project Structure
The project consists of the following files, which include both predefined functions and additional ones created for specific tasks:

```
main.c
filename.[h/c]
lut.[h/c]
Makefile
pixels.[h/c]
pictures.[h/c]
doc/rapport.pdf
```

### Notable Functions:
#### pixels.[h/c]:
- **int pixel(int comp, int nb_chan, int c):** Computes the byte position of a pixel based on its parameters.

#### pictures.[h/c]:
- **`FILE* canopen(char* filename):`** Checks if a file can be opened for reading.
- **`FILE* canwrite(char* filename):`** Checks if a file can be opened for writing.
- **`int is_gray_picture(picture p):`** Returns 1 if the image is grayscale, otherwise 0.
- **`int is_color_picture(picture p):`** Returns 1 if the image is in color, otherwise 0.
- **`int rand_up_to(int max):`** Returns a random number between 0 and max.
- **`int byte_number(picture p):`** Returns the total number of bytes in an image.
- **`picture grow_size_nearest_neighbor_interpolation(picture p, double factor):`** Enlarges an image using the nearest neighbor interpolation method.
- **`picture reduce_size_nearest_neighbor_interpolation(picture p, double factor):`** Reduces an image using the nearest neighbor interpolation method.
- **`picture grow_size_bi_linear_interpolation(picture p, double factor):`** Enlarges an image using bilinear interpolation.
- **`picture reduce_size_bi_linear_interpolation(picture p, double factor):`** Reduces an image using bilinear interpolation.

## Technical Choices and Solutions

### General Remarks
In an image, each pixel is represented by one or more bytes, depending on the format. In visual terms, this structure is represented in the form of an array. However, in my code, I treat images as a single line of bytes via a list, as I find this more comfortable. In this way, pixel access operations are applied to a single line.

### LUT Implementation
- **`create_lut`:** Uses `calloc` to initialize all LUT values to zero, preventing potential errors.
- **`apply_lut`:** Uses a scaling operation to match the LUT size with the number of bytes in the image.

### Image Processing Functions
- **`read_picture`:** Handles commented lines and properly positions the file cursor for correct image byte reading.
- **`canopen` / `canwrite`:** Separate functions to check file readability and writability, reducing redundancy.
- **`melt_picture`:** Determines the darker pixel by summing RGB components and comparing values.

## Main Program
The main program (`main.c`) integrates all the functionalities and allows processing images with the implemented transformations.

## Conclusion
This project successfully implements various image processing techniques, including grayscale and color image manipulations, interpolation methods, and LUT transformations. The modular structure makes it easy to extend and adapt for future improvements.
