#include "my_allocator.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

    init_allocator();
    printf("Allocator initialized\n");
    
    //here we test the buddy allocator trying to allocate and free different sizes of memory
    char* ptr1 = (char*) my_malloc(4); //(int) 4 bytes << 1/1024 of the page size (4096 bytes)
    if(ptr1!=NULL){
        printf("Allocated 4 bytes at address %p\n", ptr1);
    }
    char* ptr2 = (char*) my_malloc(150);
    if(ptr2!=NULL){
        printf("Allocated 150 bytes at address %p\n", ptr2);
    }
    char* ptr3 = (char*) my_malloc(500);
    if(ptr3!=NULL){
        printf("Allocated 500 bytes at address %p\n", ptr3);
    }
    char* ptr4 = (char*) my_malloc(1020); //4 bytes will be added for the index
    if(ptr4!=NULL){
        printf("Allocated 1024 bytes at address %p\n", ptr4);
    }
    my_free(ptr1, 4);
    my_free(ptr2, 150);
    my_free(ptr3, 500);
    my_free(ptr4, 1024);
    printf("All memory as been freed\n");

    //here we test the mmap function trying to allocate and free different sizes of memory
    char* ptr5 = (char*) my_malloc(1024);
    if(ptr5!=NULL){
        printf("Allocated 1024 bytes at address %p\n", ptr5);
    }
    char* ptr6 = (char*) my_malloc(4096);
    if(ptr6!=NULL){
        printf("Allocated 4096 bytes at address %p\n", ptr6);
    }
    my_free(ptr5, 1024);
    my_free(ptr6, 4096);
    printf("All memory as been freed\n");

    //here we try to empty all the memory available to the buddy allocator and then request more
    char* ptr[1025];
    for(int i = 0; i<1025; i++){
        ptr[i] = (char*) my_malloc(1024);
    }
    for(int i= 0; i<1025; i++){
        my_free(ptr[i], 1024);
    }
    //here we check if the allocator can handle invalid requests well
    char* ptr7 = (char*) my_malloc(0);
    if(ptr7!=NULL){
        printf("Allocated 0 bytes at address %p\n", ptr7);
    }
    char* ptr8 = (char*) my_malloc(-1);
    if(ptr8!=NULL){
        printf("Allocated -1 bytes at address %p\n", ptr8);
    }
    my_free(ptr7, 0);
    my_free(ptr8, -1);
    printf("All memory as been freed\n");
}