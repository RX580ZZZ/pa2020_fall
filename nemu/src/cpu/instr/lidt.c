#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/
make_instr_func(lidt){
    OPERAND rm;
    int len = 1 + modrm_rm(eip + 1, &rm);
    rm.data_size = 16;
    operand_read(&rm);
    cpu.idtr.limit = rm.val;
    rm.addr += 2;
    rm.data_size = 32;
    operand_read(&rm);
    cpu.idtr.base = rm.val;
    print_asm_1("lidt", "", len, &rm);
    
    return len;
}