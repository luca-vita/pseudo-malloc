#include "buddy_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

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
//Get the index of the left child of the node at given index
int getLeft(int index){
    return 2*index+1;
}
//Get the index of the right child of the node at given index
int getRight(int index){
    return 2*index+2;
}

//Defining the function to initialize the buddy allocator
void buddyAllocator_init(buddyAllocator *ballocator, int num_levels, int min_bucket_size, char *memory, uint8_t *buffer){

    ballocator->num_levels = num_levels;
    ballocator->min_bucket_size = min_bucket_size;
    ballocator->memory = memory;
    //the initialization of the buddy allocator includes the initialization of its bitmap
    int num_bits = (1 << num_levels) - 1 ;  //one bit for each node of the binary tree
    bitmap_init(ballocator->bmap,  num_bits, buffer);

    //setting the root node as available (1 available, 0 not available)
    bitmap_set(ballocator->bmap, 0, 1);

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
    if(level < 0){
        return -1; //the requested memory exceeds the maximum allocatable size
    }
    assert(level < ballocator->num_levels);
    int first = getFirst(level);
    int last = getFirst(level+1)-1;
    for(int i=first; i<=last; i++){
        if(bitmap_get(ballocator->bmap, i) == 1)
            return i;
    }
    return -1;
}

//Defining the function to choose the index of the buddy to allocate for a given size request and to do side effect on the bitmap 
//to reflect the splitting operations that might be needed
int buddyAllocator_chooseBuddy(buddyAllocator *ballocator, int level){
    if(level < 0){
        printf("Could not find a buddy to allocate for this request\n");
        return -1;
    }
    int chosen = buddyAllocator_getFirstAvailable(ballocator, level);
    if(chosen == -1){
        int greater_buddy = buddyAllocator_chooseBuddy(ballocator, level-1);
        if(greater_buddy == -1)
            return -1;
        else{
            chosen = getLeft(greater_buddy);
            int discarded = getRight(greater_buddy);
            bitmap_set(ballocator->bmap, discarded, 1);
        }
    }
    //setting the chosen buddy as not available
    bitmap_set(ballocator->bmap, chosen, 0);
    //setting the chosen buddy's ancestors as not available
    return chosen;
}

//Defining the function that does side effect on the bitmap to reflect the merging operations that might follow a deallocation
void buddyAllocator_restoreBuddies(buddyAllocator *ballocator, int index){
    int buddy = getBuddy(index);
    if(buddy == -1)
        bitmap_set(ballocator->bmap, index, 1);
    if(bitmap_get(ballocator->bmap, buddy) == 1){
        bitmap_set(ballocator->bmap, index, 0);
        bitmap_set(ballocator->bmap, buddy, 0);
        buddyAllocator_restoreBuddies(ballocator, getParent(index));
    }
    else{
        bitmap_set(ballocator->bmap, index, 1);
    }
}

//Defining the function to get the memory address of the buddy at the given index (the buddies will be managed using their indexes
//and only when needed the actual memory address will be calculated and used)
char* buddyAllocator_getBuddyAddress(buddyAllocator *ballocator, int index){
    int level = getLevel(index);
    int offset = (index - getFirst(level)) * ballocator->min_bucket_size * (1 << (ballocator->num_levels - level - 1));
    return ballocator->memory + offset;
}

//Defining the function to allocate a buddy address of the given size. We also need a way to keep track of the allocated index
//in order to restore it when the deallocation of this address will be requested. We can do this by storing the index in the
//first 4 bytes (an int) of the allocated memory. We need to allocate more than requested.
char* buddyAllocator_malloc(buddyAllocator *ballocator, int size){
    int level = buddyAllocator_getFittestLevel(ballocator, size+4);
    int index = buddyAllocator_chooseBuddy(ballocator, level);
    if(index == -1){
        printf("The available memory is not sufficient\n");
        return NULL;
    }
    //setting the index in the first 4 bytes of the allocated memory
    char* address = buddyAllocator_getBuddyAddress(ballocator, index);
    *((int*)address) = index; //the cast to an integer pointer is needed to make sure that the index is only written in the first 4 bytes
    return address + 4;
}

//Defining the function to deallocate a buddy address
void buddyAllocator_free(buddyAllocator *ballocator, char* address){
    int index = *((int*)(address - 4));
    if(index > ballocator->bmap->num_bits){
        printf("The index is not compatible with this buddy allocator. It must be freed with the same allocator it was allocated with\n");
        return;
    }
    assert(index >= 0);
    buddyAllocator_restoreBuddies(ballocator, index);
}