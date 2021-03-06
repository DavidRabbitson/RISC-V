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
    m_executors[ExecId::JALR]   = &jalr;
    m_executors[ExecId::BEQ]    = &beq;
    m_executors[ExecId::BNE]    = &bne;
    m_executors[ExecId::BLT]    = &blt;
    m_executors[ExecId::BGE]    = &bge;
    m_executors[ExecId::BLTU]   = &bltu;
    m_executors[ExecId::BGEU]   = &bgeu;
    m_executors[ExecId::LB]     = &lb;
    m_executors[ExecId::LH]     = &lh;
    m_executors[ExecId::LW]     = &lw;
    m_executors[ExecId::LBU]    = &lbu;
    m_executors[ExecId::LHU]    = &lhu;
    m_executors[ExecId::SB]     = &sb;
    m_executors[ExecId::SH]     = &sh;
    m_executors[ExecId::SW]     = &sw;
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
    RegVal offset = (instr->get_imm() << 12) + hart->get_pc();
    hart->set_reg(instr->get_rd(), offset);
};

//-------------------------------------------------------------------
//BRANCHES

void jal(Hart *hart, Instruction *instr)
{
    hart->set_reg(instr->get_rd(), hart->get_pc() + 4);

    RegVal offset = instr->get_imm();
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), hart->get_pc() + 4);

    offset = ((offset & 0x80000) +     ((offset >> 9) & 0x3ff) + \
             ((offset & 0x100) << 2) + ((offset & 0xff) << 11)) << 1;

    if(offset & 0x100000)
        offset += 0xffe00000;

    printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
    hart->set_pc_offset(offset);
};

void jalr(Hart *hart, Instruction *instr)
{
    RegVal offset = instr->get_imm();
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), hart->get_pc() + 4);

    if(offset & 0x800)
        offset += 0xfffff000;
    offset &= 0xfffffffe;

    printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
    //hart->print_regs();

    if(offset == 0)
        offset = 4;

    hart->set_pc_offset(offset);
};

void beq(Hart *hart, Instruction *instr)
{
    RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

    offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
             ((offset & 0x001) << 10)) << 1;

    if(offset & 0x1000)
        offset += 0xffffe000;

    if(hart->get_reg(instr->get_rs1()) == hart->get_reg(instr->get_rs2()))
    {
        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc_offset(offset);
    }
    else
        hart->set_pc_offset(4);
};

void bne(Hart *hart, Instruction *instr)
{
    RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

    offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
             ((offset & 0x001) << 10)) << 1;

    if(offset & 0x1000)
        offset += 0xffffe000;
    
    if(hart->get_reg(instr->get_rs1()) != hart->get_reg(instr->get_rs2()))
    {
        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc_offset(offset);
    }
    else
        hart->set_pc_offset(4);
};

void blt(Hart *hart, Instruction *instr)
{
    RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

    offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
             ((offset & 0x001) << 10)) << 1;

    if(offset & 0x1000)
        offset += 0xffffe000;

    if((SignedRegVal)hart->get_reg(instr->get_rs1()) < \
       (SignedRegVal)hart->get_reg(instr->get_rs2()))
    {
        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc_offset(offset);
    }
    else
        hart->set_pc_offset(4);
};

void bge(Hart *hart, Instruction *instr)
{
    RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

    offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
             ((offset & 0x001) << 10)) << 1;

    if(offset & 0x1000)
        offset += 0xffffe000;

    if((SignedRegVal)hart->get_reg(instr->get_rs1()) >= \
       (SignedRegVal)hart->get_reg(instr->get_rs2()))
    {
        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc_offset(offset);
    }
    else
        hart->set_pc_offset(4);
};

void bltu(Hart *hart, Instruction *instr)
{
    RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

    offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
             ((offset & 0x001) << 10)) << 1;

    if(offset & 0x1000)
        offset += 0xffffe000;

    if(hart->get_reg(instr->get_rs1()) < hart->get_reg(instr->get_rs2()))
    {
        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc_offset(offset);
    }
    else
        hart->set_pc_offset(4);
};

void bgeu(Hart *hart, Instruction *instr)
{
    RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

    offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
             ((offset & 0x001) << 10)) << 1;

    if(offset & 0x1000)
        offset += 0xffffe000;

    if(hart->get_reg(instr->get_rs1()) >= hart->get_reg(instr->get_rs2()))
    {
        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc_offset(offset);
    }
    else
        hart->set_pc_offset(4);
};

//-------------------------------------------------------------------
//  LOADS AND STORES

void lb    (Hart *hart, Instruction *instr)
{
	hart->dram_read(instr.m_rs1, instr->m_rd, sizeof(Byte));
};

void lh    (Hart *hart, Instruction *instr)
{
	hart->dram_read(instr.m_rs1, instr->m_rd, sizeof(HalfWord));
};

void lw    (Hart *hart, Instruction *instr)
{
	hart->dram_read(instr.m_rs1, instr->m_rd, sizeof(Word));
};

void lbu   (Hart *hart, Instruction *instr)
{
	hart->dram_read(instr.m_rs1, instr->m_rd, sizeof(Byte));		// ???
};

void lhu   (Hart *hart, Instruction *instr)
{
	hart->dram_read(instr.m_rs1, instr->m_rd, sizeof(HalfWord));	// ???
};

void sb    (Hart *hart, Instruction *instr)
{
	hart->dram_write(instr.m_rs2, instr->m_rs1, sizeof(Byte));
};

void sh    (Hart *hart, Instruction *instr)
{
	hart->dram_write(instr.m_rs2, instr->m_rs1, sizeof(HalfWord));
};

void sw    (Hart *hart, Instruction *instr)
{
	hart->dram_write(instr.m_rs2, instr->m_rs1, sizeof(Word));
};

//-------------------------------------------------------------------
//  IMMEDIATE ARITHMETICS

void addi(Hart *hart, Instruction *instr)
{
    RegVal res = instr->get_imm();

    if(res & 0x800)
        res += 0xfffff000;

    res = hart->get_reg(instr->get_rs1()) + res;
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void slti(Hart *hart, Instruction *instr)
{
    RegVal res = instr->get_imm();

    if(res & 0x800)
        res += 0xfffff000;

    res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) < (SignedRegVal)(res);
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void sltiu(Hart *hart, Instruction *instr)
{
    RegVal res = instr->get_imm();

    if(res & 0x800)
        res += 0xfffff000;

    res = hart->get_reg(instr->get_rs1()) < res;
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void xori(Hart *hart, Instruction *instr)
{
    RegVal res = instr->get_imm();

    if(res & 0x800)
        res += 0xfffff000;

    res = (hart->get_reg(instr->get_rs1()) ^ res);
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void ori(Hart *hart, Instruction *instr)
{
    RegVal res = instr->get_imm();

    if(res & 0x800)
        res += 0xfffff000;

    res = hart->get_reg(instr->get_rs1()) | res;
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void andi(Hart *hart, Instruction *instr)
{
    RegVal res = instr->get_imm();

    if(res & 0x800)
        res += 0xfffff000;

    res = hart->get_reg(instr->get_rs1()) & res;
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void slli(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) << instr->get_rs2();
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void srli(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) >> instr->get_rs2();
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void srai(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) >> instr->get_rs2();
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

//-------------------------------------------------------------------
//  REGISTER ARITHMETICS

void add(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) + hart->get_reg(instr->get_rs2())) & 0xffffffff;
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void sub(Hart *hart, Instruction *instr)
{
    RegVal res = (hart->get_reg(instr->get_rs1()) - hart->get_reg(instr->get_rs2())) & 0xffffffff;
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void sll(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) << hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void slt(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) < (SignedRegVal)(hart->get_reg(instr->get_rs2()));
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void sltu(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) < hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void m_xor(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) ^ hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void srl(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) >> hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void sra(Hart *hart, Instruction *instr)
{
    RegVal res = (SignedRegVal)(hart->get_reg(instr->get_rs1())) >> hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void m_or(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) | hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

void m_and(Hart *hart, Instruction *instr)
{
    RegVal res = hart->get_reg(instr->get_rs1()) & hart->get_reg(instr->get_rs2());
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), res);
};

//-------------------------------------------------------------------
//  SYSTEM CALLS

void fence (Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void fencei(Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void ecall (Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void ebreak(Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

//-------------------------------------------------------------------
//  CONTROL AND STATUS REGISTER COMMANDS

void csrrw (Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void csrrs (Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void csrrc (Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void csrrwi(Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void csrrsi(Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

void csrrci(Hart *hart, Instruction *instr){printf("\nSYSTEM CALL\n\n");};

