#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt){
    OPERAND rm;
    int len = 1 + modrm_rm(eip + 1, &rm);
    rm.data_size = 16;
    operand_read(&rm);
    cpu.gdtr.limit = rm.val;
    rm.addr += 2;
    rm.data_size = 32;
    operand_read(&rm);
    cpu.gdtr.base = rm.val;
    print_asm_1("lgdt", "", len, &rm);
    
    return len;
}