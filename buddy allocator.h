#include "bitmap.h"

//Defining the data structure for the buddy allocator
typedef struct {
    int num_levels;
    int min_bucket_size;
    bitmap* bmap;
    char* memory;
} buddyAllocator;

//declaring the function to initialize an allocated buddy allocator
void buddyAllocator_init(buddyAllocator *ballocator, int num_levels, int min_bucket_size, char* memory, uint8_t* buffer);

//declaring the function to get the memory address of the buddy at the given index (the buddies will be managed using their indexes
//and only when needed the actual memory address will be calculated and used)
char* buddyAllocator_getBuddyAddress(buddyAllocator *ballocator, int index);

//declaring the function to get the level corresponding to the smallest partition that can satisfy the requested size
int buddyAllocator_getFittestLevel(buddyAllocator *ballocator, int size);

//declaring the function to get the index of the first available buddy of a given level
int buddyAllocator_getFirstAvailable(buddyAllocator *ballocator, int level);

//declaring the function to choose the index of the buddy to allocate for a given size request
int buddyAllocator_chooseBuddy(buddyAllocator *ballocator, int level);

//declaring the function that does side effect on the bitmap to reflect the merging operations that might follow the deallocation
//of the buddy at the given index
void buddyAllocator_restoreBuddies(buddyAllocator *ballocator, int index);

//declaring the function to allocate a buddy address of the given size
char* buddyAllocator_malloc(buddyAllocator *ballocator, int size);

//declaring the function to deallocate a buddy address
void buddyAllocator_free(buddyAllocator *ballocator, char* address);