#include "cpu/instr.h"
/*
Put the implementations of `cli' instructions here.
*/
make_instr_func(cli){
    cpu.eflags.IF = 0;
    int len = 1;
    print_asm_0("cli", "", len);
    return len;
}