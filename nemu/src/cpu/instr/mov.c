#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_r2cr_l){
    OPERAND r, cr;
    int len = 1;
    cr.data_size = r.data_size = 32;
    len += modrm_r_rm(eip + 1, &cr, &r);
    cr.type = OPR_CREG;
    
    operand_read(&r);
    cr.val = r.val;
    operand_write(&cr);
    
    print_asm_2("mov", cr.data_size == 8 ? "b" : (cr.data_size == 16 ? "w" : "l"), len, &r, &cr);
    return len;
}

make_instr_func(mov_cr2r_l){
    OPERAND r, cr;
    int len = 1;
    cr.data_size = r.data_size = 32;
    len += modrm_r_rm(eip + 1, &r, &cr);
    cr.type = OPR_CREG;
    
    operand_read(&cr);
    r.val = cr.val;
    operand_write(&r);
    
    print_asm_2("mov", r.data_size == 8 ? "b" : (r.data_size == 16 ? "w" : "l"), len, &cr, &r);
    return len;
}

make_instr_func(mov_rm2s_w) {
        OPERAND s, rm;
        s.data_size = 16;
        rm.data_size = 16;
        int len = modrm_r_rm(eip + 1, &s, &rm) + 1;
        s.type = OPR_SREG;
        operand_read(&rm);
        s.val = rm.val;
        
        operand_write(&s);
        load_sreg(s.addr);
	print_asm_2("mov", "w", len, &rm, &s);
        return len;
} 