#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/
static void instr_execute_1op()
{
	operand_read(&opr_src);
	cpu.esp -= data_size / 8;
	opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
	opr_dest.addr = cpu.esp; 
	opr_dest.type = OPR_MEM;   
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
	
}
//make_instr_impl_1op(push, m, v)
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, v)
make_instr_func(push_i_b){
    int len = 1;
    opr_src.data_size = 8;
	cpu.esp -= data_size / 8;
	opr_src.type = OPR_IMM;
	opr_src.addr = eip + 1;
	opr_src.sreg = SREG_CS;
	len += 1;
	print_asm_1("push", "b", len, &opr_src);
	operand_read(&opr_src);
	opr_src.val = sign_ext(opr_src.val, 8); 
	opr_dest.data_size = data_size;
	opr_dest.type = OPR_MEM; 
	opr_dest.addr = cpu.esp;
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
	return len;
}

make_instr_func(pusha){
    int len = 1;
    print_asm_0("pusha", "", 1);
    
    uint32_t temp_esp = cpu.esp;
    OPERAND temp;
    temp.data_size = 32;
    temp.type = OPR_MEM;
    temp.sreg = SREG_DS;
    
    for (int i = 0; i < 8; i++)
    {
        temp_esp -= data_size / 8;
        temp.addr = temp_esp;
        temp.val = cpu.gpr[i].val;
        operand_write(&temp);
    }
    
    cpu.esp = temp_esp;
    return len;
}