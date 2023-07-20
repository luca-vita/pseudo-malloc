#include "buddy allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Defining some auxillary functions that will be useful in the manipulation of the binary tree that results from a buddy allocator
/*Note that:
    the levels are counted starting from 0 with 0 being the root level;
    the indexes are counted starting from 0 with 0 being the root node;
    indexes at the same level are counted from left to right in ascending order;
*/
//Get the level of the buddy at given index
int getLevel(int index){
    return floor(log2(index+1));
}
//Get the index of the parent of the buddy at given index
int getParent(int index){
    return floor((index-1)/2);
}
//Get the index of the buddy of the node at given index
int getBuddy(int index){
    if(index==0)
        return -1; //The root node has no buddy
    return (index%2==0)?index-1:index+1;
}
//Get the index of the first buddy of a given level
int getFirst(int level){
    return (1 << level)-1;
}

//Defining the function to initialize the buddy allocator
void buddyAllocator_init(buddyAllocator *ballocator, int num_levels, int min_bucket_size, char* memory, uint8_t* buffer){

    ballocator->num_levels = num_levels;
    ballocator->min_bucket_size = min_bucket_size;
    ballocator->memory = memory;
    //the initialization of the buddy allocator includes the initialization of its bitmap
    int num_bits = (1 << num_levels-1 ); //era (1 << num_levels) - 1
    bitmap_init(&(ballocator->bmap),  num_bits, buffer);

    //setting the root node as available (1 available, 0 not available)
    bitmap_set_bit(&(ballocator->bmap), 0, 1);

    printf("BUDDY ALLOCATOR INITIALIZATION FINISHED\n");
    printf("\tIt has %d levels\n", ballocator->num_levels);
    printf("\tThe minimum allocatable size is %d bytes\n\n", ballocator->min_bucket_size);    
    printf("\tTherefore it can manage %d bytes of memory\n", (1 << (num_levels - 1)) * min_bucket_size);
}

//Defining the function to get the level corresponding to the smallest partition that can satisfy the requested size
int buddyAllocator_getFittestLevel(buddyAllocator *ballocator, int size){
    int steps = 0;
    int current_size = ballocator->min_bucket_size;
    while(current_size < size){
        current_size *= 2;
        steps++;
    }
    return ballocator->num_levels - steps - 1;
}

//Defining the function to get the index of the first available buddy of a given level
int buddyAllocator_getFirstAvailable(buddyAllocator *ballocator, int level){
    int first = getFirst(level);
    int last = getFirst(level+1)-1;
    for(int i=first; i<=last; i++){
        if(bitmap_get(&(ballocator->bmap), i) == 1)
            return i;
    }
    return -1;
}

//Defining the function to get the memory address of the buddy at the given index (the buddies will be managed using their indexes
//and only when needed the actual memory address will be calculated and used)
char* buddyAllocator_getBuddyAddress(buddyAllocator *ballocator, int index){
    int level = getLevel(index);
    int offset = (index - getFirst(level)) * ballocator->min_bucket_size * (1 << (ballocator->num_levels - level - 1));
}