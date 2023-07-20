#include "my allocator.h"
#include <stdlib.h>
#include <sys/mman.h>


//here we define some global variables and structure that will be shared among the following functions.
//They must be allocated statically because they will be used in the allocator itself
buddyAllocator buddy_allocator;
char memory[MAX_BUDDY_ALLOCABLE_SIZE];
uint8_t buffer[(1 << (BUDDY_LEVELS) -1)];

//Defining the function to initialize the allocator (it's just a wrapper for the other initializations needed)
void init_allocator() {
    init_buddy_allocator(&buddy_allocator, BUDDY_LEVELS, MIN_BUDDY_ALLOCABLE_SIZE, memory, buffer);
}

//Defining the function to allocate memory
void* my_malloc(int size) {
    assert(size > 0); //invalid request

    //if the requested size is no greater than 1/4 of the page size, we can use the buddy allocator
    if(size <= PAGE_SIZE/4) {
        return buddyAllocator_malloc(&buddy_allocator, size);
    }
    //otherwise we use the mmap function
    else {
        void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
        if(ptr == MAP_FAILED) {
            perror("Error in allocating memory with mmap\n");
            return NULL;
        }
        return ptr;
    }
}

//Defining the function to free memory
void my_free(void* ptr, int size) {
    assert(ptr > 0 && size > 0); //invalid request

    //if the requested size is no greater than 1/4 of the page size, we can use the buddy allocator
    if(size <= PAGE_SIZE/4) {
        buddyAllocator_free(&buddy_allocator, ptr);
    }
    //otherwise we use the munmap function
    else {
        if(munmap(ptr, size) == -1) {
            perror("Error in freeing memory with munmap\n");
        }
    }
}