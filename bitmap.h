#pragma once
#include <stdint.h>

//defining the data structure for the bitmap
typedef struct  {
    int buffer_size;
    int num_bits;
    uint8_t *buffer;
} bitmap;

//declaring the function to initialize an allocated bitmap
int bitmap_init(bitmap *bmap, int num_bits, uint8_t* buffer);

//declaring the function to set a bit in the bitmap
int bitmap_set(bitmap *bmap, int bit, int value);

//declaring the function to set a bit in the bitmap
int bitmap_reset(bitmap *bmap, int bit);
