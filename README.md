# Pseudo Malloc
This is a self-implemented substitute for the malloc function that uses a Buddy Allocator for requests of up to 1/4 of the page size and the mmap function for larger requests. The buddy allocator is implemented using a bitmap that keeps track of the status of the buddies (chunks of memory) in the following way:

  the value 1 indicates that the buddy is ready to be allocated;

  the value 0 indicates that the buddy is not ready to be allocated either because it's already been allocated or becaure it's been split in two or because it's parent hasn't been split yet
