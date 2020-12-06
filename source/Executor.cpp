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
    m_executors[ExecId::LUI]    = &lui;
    m_executors[ExecId::AUIPC]  = &auipc;
    m_executors[ExecId::JAL]    = &jal;
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
    m_executors[ExecId::SLTI]   = &slti;
    m_executors[ExecId::SLTIU]  = &sltiu;
    m_executors[ExecId::XORI]   = &xori;
    m_executors[ExecId::ORI]    = &ori;
    m_executors[ExecId::ANDI]   = &andi;
    m_executors[ExecId::SLLI]   = &slli;
    m_executors[ExecId::SRLI]   = &srli;
    m_executors[ExecId::SRAI]   = &srai;
    m_executors[ExecId::ADD]    = &add;
    m_executors[ExecId::SUB]    = &sub;
    m_executors[ExecId::SLL]    = &sll;
    m_executors[ExecId::SLT]    = &slt;
    m_executors[ExecId::SLTU]   = &sltu;
    m_executors[ExecId::XOR]    = &m_xor;
    m_executors[ExecId::SRL]    = &srl;
    m_executors[ExecId::SRA]    = &sra;
    m_executors[ExecId::OR]     = &m_or;
    m_executors[ExecId::AND]    = &m_and;
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

void dummy(Hart *hart, Instruction *instr){ /*printf("\nDUMMY\n\n"); */};

void lui(Hart *hart, Instruction *instr)
{
    hart->set_reg(instr->get_rd(), instr->get_imm() << 12);
};

void auipc (Hart *hart, Instruction *instr)
{
    RegVal offset = instr->get_imm() << 12 + hart->get_pc();
    hart->set_reg(instr->get_rd(), offset);
};

//-------------------------------------------------------------------
//BRANCHES

void jal(Hart *hart, Instruction *instr)
{
    hart->set_reg(instr->get_rd(), hart->get_pc() + 4);
    RegVal offset = instr->get_imm();
    offset = ((offset & 0x80000) + ((offset << 9) & 0x7fe00) + ((offset >> 2) & 0x00100) + ((offset >> 11) & 0x000ff)) << 1;
    hart->set_pc_offset(offset);
};

void jalr  (Hart *hart, Instruction *instr){};

void beq   (Hart *hart, Instruction *instr){};

void bne   (Hart *hart, Instruction *instr){};

void blt   (Hart *hart, Instruction *instr){};

void bge   (Hart *hart, Instruction *instr){};

void bltu  (Hart *hart, Instruction *instr){};

void bgeu  (Hart *hart, Instruction *instr){};

//-------------------------------------------------------------------
//  LOADS AND STORES

void lb    (Hart *hart, Instruction *instr){};

void lh    (Hart *hart, Instruction *instr){};

void lw    (Hart *hart, Instruction *instr){};

void lbu   (Hart *hart, Instruction *instr){};

void lhu   (Hart *hart, Instruction *instr){};

void sb    (Hart *hart, Instruction *instr){};

void sh    (Hart *hart, Instruction *instr){};

void sw    (Hart *hart, Instruction *instr){};

//-------------------------------------------------------------------
//  IMMEDIATE ARITHMETICS

void addi(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) + instr->get_imm()) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void slti(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) < (SignedRegVal)(instr->get_imm());
    hart->set_reg(instr->get_rd(), res);
};

void sltiu(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1())) < (instr->get_imm());
    hart->set_reg(instr->get_rd(), res);
};

void xori(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) ^ instr->get_imm()) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void ori(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) | instr->get_imm()) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void andi(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) & instr->get_imm()) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void slli(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) << hart->get_reg(instr->get_rs2())) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void srli(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1())) >> hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

void srai(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) >> hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

//-------------------------------------------------------------------
//  REGISTER ARITHMETICS

void add(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) + hart->get_reg(instr->get_rs2())) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void sub(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) - hart->get_reg(instr->get_rs2())) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void sll(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) << hart->get_reg(instr->get_rs2())) & 0x1f;
    hart->set_reg(instr->get_rd(), res);
};

void slt(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) < (SignedRegVal)(hart->get_reg(instr->get_rs2()));
    hart->set_reg(instr->get_rd(), res);
};

void sltu(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) < hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

void m_xor(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) ^ hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

void srl(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) >> hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

void sra(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) >> hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

void m_or(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) | hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

void m_and(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) & hart->get_reg(instr->get_rs2());
    hart->set_reg(instr->get_rd(), res);
};

//-------------------------------------------------------------------
//  SYSTEM CALLS

void fence (Hart *hart, Instruction *instr){};

void fencei(Hart *hart, Instruction *instr){};

void ecall (Hart *hart, Instruction *instr){};

void ebreak(Hart *hart, Instruction *instr){};

//-------------------------------------------------------------------
//  CONTROL AND STATUS REGISTER COMMANDS

void csrrw (Hart *hart, Instruction *instr){};

void csrrs (Hart *hart, Instruction *instr){};

void csrrc (Hart *hart, Instruction *instr){};

void csrrwi(Hart *hart, Instruction *instr){};

void csrrsi(Hart *hart, Instruction *instr){};

void csrrci(Hart *hart, Instruction *instr){};

