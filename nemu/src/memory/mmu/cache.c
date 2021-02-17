#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef CACHE_ENABLED
CacheLine my_cache[1024];
//uint32_t hit = 0;
//uint32_t n_hit = 0;
// init the cache
void init_cache()
{
	// implement me in PA 3-1
	for (int i = 0; i < 1024;  i++){
	    my_cache[i].valid_bit = 0;
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* my_cache)
{
	// implement me in PA 3-1
	
	uint32_t label = (paddr >> 13) & 0x7ffff;
	uint32_t group_num = (paddr >> 6) & 0x7f;
	uint32_t offset = paddr & 0x3f;
	
	memcpy(hw_mem+paddr, &data, len);
	for (int i = 0; i < 8; i++){
	    uint32_t block_num = group_num * 8 + i;
	    if (my_cache[block_num].valid_bit == 1 && my_cache[block_num].label == label){
	       if (offset + len <= 64)
	            memcpy(my_cache[block_num].data + offset, &data, len);
	       else
	       {
	           uint32_t rest = 64 - offset;
	           //cache_write(paddr, rest, data, my_cache);
	           memcpy(my_cache[block_num].data + offset, &data, rest);
	           cache_write(paddr + rest, len - rest, data >> (8 * rest), my_cache);
	       }
	       break;
	    }
	}
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* my_cache)
{
	// implement me in PA 3-1
	uint32_t label = (paddr >> 13) & 0x7ffff;
	uint32_t group_num = (paddr >> 6) & 0x7f;
	uint32_t offset = paddr & 0x3f;
	uint32_t ret;
	
	bool get = false;
	for (int i = 0; i < 8; i++){
	    uint32_t block_num = group_num * 8 + i;
	    if (my_cache[block_num].valid_bit == 1 && my_cache[block_num].label == label){
	       get = true;
	       if (offset + len <= 64)
	           memcpy(&ret, my_cache[block_num].data + offset, len);
	       else{
	           uint32_t rest = 64 - offset;
	           uint32_t temp0, temp1;
	           memcpy(&temp0, my_cache[block_num].data + offset, len);
	           temp1 = cache_read(paddr + rest, len - rest, my_cache);
	           ret = temp0 | (temp1 << (8 * rest));
	       }
	       break;
	    }
	}
	
	//if (get == true) hit ++;
	//else n_hit ++;
	if (get == false){
	    memcpy(&ret, hw_mem+paddr, len);
	    for (int i = 0; i < 8; i++)
	    {
	        uint32_t block_num = group_num * 8 + i;
	        if (my_cache[block_num].valid_bit == 0){
	            get = true;
	            my_cache[block_num].valid_bit = 1;
	            my_cache[block_num].label = label;
	            memcpy(my_cache[block_num].data, hw_mem + paddr - offset, 64);
	            break;
	        }
	    }
	    
	    if (get == false){
	        srand((unsigned)time(0));
	        uint32_t random_num = rand() % 8;
	        uint32_t block_num = group_num * 8 + random_num;
	        my_cache[block_num].valid_bit = 1;
	        my_cache[block_num].label = label;
	        memcpy(my_cache[block_num].data, hw_mem + paddr - offset, 64);
	    }
	}
	
	return ret;
}
#endif
