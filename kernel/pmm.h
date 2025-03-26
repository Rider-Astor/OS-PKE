#ifndef _PMM_H_
#define _PMM_H_
#include "util/types.h"

// Initialize phisical memeory manager
void pmm_init();
// Allocate a free phisical page
void* alloc_page();
// Free an allocated page
void free_page(void* pa, int init);
// for 
void add_block_used(uint64 pa);
void remove_block_used(uint64 pa);
#endif