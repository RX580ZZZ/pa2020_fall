#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op()
{
	opr_dest.data_size = data_size;
	opr_dest.addr = cpu.esp; 
	opr_dest.type = OPR_MEM; 
	operand_read(&opr_dest);
	opr_src.val = opr_dest.val;
	operand_write(&opr_src);
	cpu.esp += data_size / 8;
}
make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_func(popa){
    OPERAND temp;
    temp.data_size = 32;
    temp.type = OPR_MEM;
    temp.sreg = SREG_DS;
    
    for (int i = 7; i >= 0; i--)
    {
        if (i != 4)
        {
            temp.addr = cpu.esp;
            operand_read(&temp);
            cpu.gpr[i].val = temp.val;
        }
        cpu.esp += 4;
    }
    
    print_asm_0("popa", "", 1);
    return 1;
}