#include "cpu/cpu.h"

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/

    uint32_t res = 0;
    res = dest + src; //获取计算结果
    
    set_CF_add(res,src,data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/

    uint32_t res = 0;
    res = dest + src + cpu.eflags.CF; //获取计算结果
    
    set_CF_adc(res,src,dest,data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    uint32_t res = 0;
    res = dest - src; 
    
    set_CF_sub(dest,src,data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/

    uint32_t res = 0;
    res = dest - src - cpu.eflags.CF; 
    
    set_CF_sbb(dest,src,data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    dest = dest & (0xFFFFFFFF >> (32 - data_size));
    src = src & (0xFFFFFFFF >> (32 - data_size));
    
    uint64_t res = 0;
    res = (uint64_t)dest * src; //获取计算结果
    
    if (res >> data_size){ // 设置标志位
        cpu.eflags.CF = 1;
        cpu.eflags.OF = 1;
    }else{
        cpu.eflags.CF = 0;
        cpu.eflags.OF = 0;
    }
    return res;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    src = sign_ext(src & (0xFFFFFFFF >>(32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >>(32 - data_size)), data_size);
    
    int64_t res = 0;
    res = (int64_t)dest * src; //获取计算结果
    return res;
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    dest = dest & (0xFFFFFFFFFFFFFFFF >> (64 - data_size));
    src = src & (0xFFFFFFFFFFFFFFFF >> (64 - data_size));
    
    uint32_t res = dest / src;
    
    return res;
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    int32_t res = dest / src;
    
    return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    uint32_t res = dest % src;
    return res;
}

int32_t alu_imod(int64_t src, int64_t dest)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    int32_t res = dest % src;
    return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/

    uint32_t res = 0;
    res = dest & src; 
    
    cpu.eflags.CF = 0;//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    cpu.eflags.OF = 0;
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    uint32_t res = 0;
    res = dest ^ src; 
    
    cpu.eflags.CF = 0;//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    cpu.eflags.OF = 0;
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    uint32_t res = 0;
    res = dest | src; 
    
    cpu.eflags.CF = 0;//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    cpu.eflags.OF = 0;
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/

    uint32_t res = 0;
    res = dest << src; 
    
    set_CF_shl(dest, src, data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    uint32_t res = 0;
    res = (dest >> src) & (0xFFFFFFFF >> (32 - data_size + src)); 
    
    set_CF_shr(dest, src, data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    
    int32_t res = 0;
    res = ((int32_t)dest >> src); 
    
    set_CF_shr(dest, src, data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    
    return (uint32_t)res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
/*#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif*/
    
    uint32_t res = 0;
    res = dest << src; 
    
    set_CF_shl(dest, src, data_size);//设置标志位
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    
    return res & (0xFFFFFFFF >> (32 - data_size));//高位清零并返回
}

void set_CF_add(uint32_t result, uint32_t src, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = (((result - cpu.eflags.CF) < src) || ((result - cpu.eflags.CF)< dest) || (result < src) || (result < dest));
}

void set_CF_sub(uint32_t dest, uint32_t src, size_t data_size){
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = dest < src;
}

void set_CF_sbb(uint32_t dest, uint32_t src, size_t data_size){
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = (((dest - cpu.eflags.CF) < src) || (dest < src) || (dest < (src + cpu.eflags.CF)));
}

void set_CF_shl(uint32_t dest, uint32_t src, size_t data_size){
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    for (int i = 0; i < src; i++){
        cpu.eflags.CF = sign(dest << (32 - data_size));
        dest = dest << 1;
    }
}

void set_CF_shr(uint32_t dest, uint32_t src, size_t data_size){
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    for (int i = 0; i < src; i++){
        cpu.eflags.CF = sign(dest << 31);
        dest = dest >> 1;
    }
}

void set_ZF(uint32_t result, size_t data_size){
    result = result & (0xFFFFFFFF >> (32 - data_size));
    cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >>(32 - data_size)), data_size);
    cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result){
     result ^= (result >> 1);
     result ^= (result >> 2);
     result ^= (result >> 4);
     result = (result << 31);
     cpu.eflags.PF = !sign(result);
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            result = sign_ext(result & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            result = sign_ext(result & 0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default: break;
    }
    if (sign(src) == sign(dest)){
        if(sign(src) != sign(result))
            cpu.eflags.OF = 1;
        else 
            cpu.eflags.OF = 0;
    }else{
        cpu.eflags.OF = 0;
    }
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            result = sign_ext(result & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            result = sign_ext(result & 0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default: break;
    }
    if (sign(src) != sign(dest)){
        if(sign(dest) != sign(result))
            cpu.eflags.OF = 1;
        else 
            cpu.eflags.OF = 0;
    }else{
        cpu.eflags.OF = 0;
    }
}

