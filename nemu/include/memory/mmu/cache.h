#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"

#ifdef CACHE_ENABLED

typedef struct{
    bool valid_bit;
    uint32_t label;
    uint8_t data[64];
} CacheLine;

// init the cache
void init_cache();

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* my_cache);

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* my_cache);

extern CacheLine my_cache[];

#endif

#endif
