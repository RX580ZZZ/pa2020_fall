#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
#ifdef CACHE_ENABLED
		ret = cache_read(paddr, len, my_cache);     // 通过cache进行读
		//printf("hit = %d, n_hit = %d", hit, n_hit);
#else
        int is_io = is_mmio(paddr);         
        if (is_io == -1)		
            ret = hw_mem_read(paddr, len);
        else
        {
            ret = mmio_read(paddr, len, is_io);
            //printf("IO read! \n");
        }
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data)
{
#ifdef CACHE_ENABLED
		cache_write(paddr, len, data, my_cache);    // 通过cache进行写
#else
        int is_io = is_mmio(paddr);         
        if (is_io == -1)		
            hw_mem_write(paddr, len, data);
        else
        {
            mmio_write(paddr, len, data, is_io);
            //printf("IO write! \n");
        }		

        
#endif
}

uint32_t laddr_read(laddr_t laddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	uint32_t paddr = laddr;
	//uint32_t page_len = len + ((laddr << 20)& 0xfff);
	if (cpu.cr0.pg == 1 && cpu.cr0.pe == 1) {
    	/*if (page_len > 0x1000) assert(0);
	    else*/ paddr = page_translate(laddr);
	}
	return paddr_read(paddr, len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	uint32_t paddr = laddr;
	//uint32_t page_len = len + ((laddr) & 0xfff);
	if (cpu.cr0.pg == 1 && cpu.cr0.pe == 1) {
	    /*if (page_len > 0x1000) assert(0);
	    else*/ paddr = page_translate(laddr);
	}
	paddr_write(paddr, len, data);
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
	uint32_t laddr = vaddr;
	if( cpu.cr0.pe == 1) {
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_read(laddr, len);
#endif

}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_write(vaddr, len, data);
#else
	uint32_t laddr = vaddr;
	if( cpu.cr0.pe == 1) {
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_write(laddr, len, data);
#endif


}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
	init_cache();                             // 初始化cache
#endif
#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}
