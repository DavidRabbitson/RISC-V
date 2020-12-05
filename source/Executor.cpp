#include"../includes.h"
#include"../headers/Hart.h"
#include"../headers/Instruction.h"
#include"../headers/Executor.h"

void Executor::execute(Instruction *instr)
{
    ( *m_executors[instr->get_exec_id()] ) (m_hart, instr);
};

Executor::Executor(Hart *hart)
{
    m_hart = hart;
    m_executors[ExecId::LUI]    = &dummy;
    m_executors[ExecId::AUIPC]  = &dummy;
    m_executors[ExecId::JAL]    = &dummy;
    m_executors[ExecId::JALR]   = &dummy;
    m_executors[ExecId::BEQ]    = &dummy;
    m_executors[ExecId::BNE]    = &dummy;
    m_executors[ExecId::BLT]    = &dummy;
    m_executors[ExecId::BGE]    = &dummy;
    m_executors[ExecId::BLTU]   = &dummy;
    m_executors[ExecId::BGEU]   = &dummy;
    m_executors[ExecId::LB]     = &dummy;
    m_executors[ExecId::LH]     = &dummy;
    m_executors[ExecId::LW]     = &dummy;
    m_executors[ExecId::LBU]    = &dummy;
    m_executors[ExecId::LHU]    = &dummy;
    m_executors[ExecId::SB]     = &dummy;
    m_executors[ExecId::SH]     = &dummy;
    m_executors[ExecId::SW]     = &dummy;
    m_executors[ExecId::ADDI]   = &addi;
    m_executors[ExecId::SLTI]   = &dummy;
    m_executors[ExecId::SLTIU]  = &dummy;
    m_executors[ExecId::XORI]   = &dummy;
    m_executors[ExecId::ORI]    = &dummy;
    m_executors[ExecId::ANDI]   = &dummy;
    m_executors[ExecId::SLLI]   = &dummy;
    m_executors[ExecId::SRLI]   = &dummy;
    m_executors[ExecId::SRAI]   = &dummy;
    m_executors[ExecId::ADD]    = &add;
    m_executors[ExecId::SUB]    = &dummy;
    m_executors[ExecId::SLL]    = &dummy;
    m_executors[ExecId::SLT]    = &dummy;
    m_executors[ExecId::SLTU]   = &dummy;
    m_executors[ExecId::XOR]    = &dummy;
    m_executors[ExecId::SRL]    = &dummy;
    m_executors[ExecId::SRA]    = &dummy;
    m_executors[ExecId::OR]     = &dummy;
    m_executors[ExecId::AND]    = &dummy;
    m_executors[ExecId::FENCE]  = &dummy;
    m_executors[ExecId::FENCEi] = &dummy;
    m_executors[ExecId::ECALL]  = &dummy;
    m_executors[ExecId::EBREAK] = &dummy;
    m_executors[ExecId::CSRRW]  = &dummy;
    m_executors[ExecId::CSRRS]  = &dummy;
    m_executors[ExecId::CSRRC]  = &dummy;
    m_executors[ExecId::CSRRWI] = &dummy;
    m_executors[ExecId::CSRRSI] = &dummy;
    m_executors[ExecId::CSRRCI] = &dummy;
    m_executors[ExecId::FAULT]  = &dummy;
};

void dummy(Hart *hart, Instruction *instr){ printf("\nDUMMY\n\n"); };

void addi(Hart *hart, Instruction *instr)
{
    printf("\nInital register values: r%d = %d, r%d = %d\n", instr->get_rs1(), hart->get_reg(instr->get_rs1()), instr->get_rd(), hart->get_reg(instr->get_rd()));
    printf("Adding immediate %d to r%d and storing in r%d\n", instr->get_imm(), instr->get_rs1(), instr->get_rd());

    RegVal sum = hart->get_reg(instr->get_rs1()) + instr->get_imm();
    printf("sum = %d\n", sum);

    hart->set_reg(instr->get_rd(), sum);

    printf("Result: r%d = %d\n\n", instr->get_rd(), hart->get_reg(instr->get_rd()));
};

void add(Hart *hart, Instruction *instr)
{
    RegVal sum = hart->get_reg(instr->get_rs1()) + hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), sum);
};

