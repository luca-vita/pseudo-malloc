#pragma once
#include "buddy allocator.h"

//here we define the parameters that will be used to build the buddy allocator
#define PAGE_SIZE 4096                  //as the most common page size in moder systems
#define MAX_BUDDY_ALLOCABLE_SIZE 1024*1024 //(1MB) as in the example provided in the course
#define BUDDY_LEVELS 9                  //as in the example provided in the course
#define MIN_BUDDY_ALLOCABLE_SIZE (MAX_BUDDY_ALLOCABLE_SIZE >> BUDDY_LEVELS) 

//declaring the function to initialize the allocator (it's just a wrapper for the other initializations needed)
void init_allocator();

//declaring the function to allocate memory
void* my_malloc(int size);

//declaring the function to free memory
void my_free(void* ptr, int size);