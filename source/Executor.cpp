#include"../includes.h"
#include"../headers/Hart.h"
#include"../headers/Instruction.h"

void add(Hart *hart, Instruction *instr)
{
    RegVal sum = hart->get_reg(instr->get_rs1()) + hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), sum);
};

