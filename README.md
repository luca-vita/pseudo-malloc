# Pseudo Malloc
This is a self-implemented substitute for the malloc function that uses a Buddy Allocator for requests of up to 1/4 of the page size and the mmap function for larger requests. The buddy allocator is implemented using a bitmap
