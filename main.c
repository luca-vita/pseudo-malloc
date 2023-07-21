#include "my_allocator.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    
    init_allocator();
    
    printf("Allocator initialized\n");

    //here we test the buddy allocator trying to allocate and free different sizes of memory
    printf("TESTING THE BUDDY ALLOCATOR WITH DIFFERENT SIZES\n");
    printf("Trying to allocate 4 bytes\n");
    char* ptr1 = (char*) my_malloc(4); //(int) 4 bytes << 1/1024 of the page size (4096 bytes)
    if(ptr1!=NULL){
        printf("Allocated 4+4 bytes at address %p\n", ptr1);
    }
    
    printf("Trying to allocate 150+4 vytes\n");
    char* ptr2 = (char*) my_malloc(150);
    if(ptr2!=NULL){
        printf("Allocated 150 bytes at address %p\n", ptr2);
    }

    printf("Trying to allocate 500+4 bytes\n");
    char* ptr3 = (char*) my_malloc(500);
    if(ptr3!=NULL){
        printf("Allocated 500 bytes at address %p\n", ptr3);
    }
    printf("Trying to allocate 1020+4 bytes");
    char* ptr4 = (char*) my_malloc(1020); //4 bytes will be added for the index
    if(ptr4!=NULL){
        printf("Allocated 1020 bytes at address %p\n", ptr4);
    }
    my_free(ptr1, 4);
    my_free(ptr2, 150);
    my_free(ptr3, 500);
    my_free(ptr4, 1020);
    printf("All memory as been freed\n\n");

    //here we test the mmap function trying to allocate and free different sizes of memory
    printf("TESTING THE MMAP ALLOCATOR\n");
    printf("Trying to allocate 1024+4 bytes\n");
    char* ptr5 = (char*) my_malloc(1024);
    if(ptr5!=NULL){
        printf("Allocated 1024 bytes at address %p\n", ptr5);
    }
    
    printf("Trying to allocate 4096+4 bytes\n");
    char* ptr6 = (char*) my_malloc(4096);
    if(ptr6!=NULL){
        printf("Allocated 4096 bytes at address %p\n", ptr6);
    }
    my_free(ptr5, 1024);
    my_free(ptr6, 4096);
    printf("All memory as been freed\n\n");


    printf("TESTING THE BUDDY ALLOCATOR BEHAVIOR WHEN TRYING MEMORY IS SATURATED\n");
    char* ptrs[20];
	for (int i = 0; i < 20; ++i)
	{
		ptrs[i] = (char*) my_malloc(1020);
	}
    //here we check if the allocator can handle invalid requests well
    printf("\nTESTING INVALID REQUESTS\n");
    printf("Trying to allocate 0+4 bytes\n");
    char* ptr7 = (char*) my_malloc(0);
    if(ptr7!=NULL){
        printf("Allocated 0 bytes at address %p\n", ptr7);
    }
    
    printf("Trying to allocate -1+4 bytes\n");
    char* ptr8 = (char*) my_malloc(-1);
    if(ptr8!=NULL){
        printf("Allocated -1 bytes at address %p\n", ptr8);
    }
    my_free(ptr7, 0);
    my_free(ptr8, -1);
    printf("All memory as been freed\n");
}