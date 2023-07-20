#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"

//defining the function to initialize an allocated bitmap
void bitmap_init(bitmap *bmap, int num_bits, uint8_t* buffer){
    bmap->num_bits = num_bits;
    printf("prova ");
    fflush(stdout);
    bmap->buffer = buffer;
    bmap->buffer_size = (num_bits + 7) / 8; //this way it's rounded to the ceiling of the actual division (9 bits require 2 bytes)    
}

//defining the function to set a bit in the bitmap
void bitmap_set(bitmap *bmap, int bit, int value){
    int byte = bit / 8;
    int offset = bit % 8;
    if(value){
        bmap->buffer[byte] |= (1 << offset);
    }
    else{
        bmap->buffer[byte] &= ~(1 << offset);
    }
}

//defining the function to get a bit from the bitmap
int bitmap_get(bitmap *bmap, int bit){
    int byte = bit / 8;
    int offset = bit % 8;
    return (bmap->buffer[byte] & (1 << offset));
}