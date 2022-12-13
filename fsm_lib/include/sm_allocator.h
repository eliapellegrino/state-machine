#ifndef _SM_ALLOCATOR_H
#define _SM_ALLOCATOR_H

#include <stddef.h>



void* SMALLOC_Alloc(size_t size);
void SMALLOC_Free(void* ptr);
void* SMALLOC_Realloc(void *ptr, size_t new_size);
void* SMALLOC_Calloc(size_t num, size_t size);

#endif // _SM_ALLOCATOR_H
