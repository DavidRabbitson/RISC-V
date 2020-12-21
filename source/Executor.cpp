#include"../includes.h"
#include"../headers/Hart.h"
#include"../headers/Memory.h"
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
    RegVal offset = (instr->get_imm() << 12) + hart->get_pc();
    hart->set_reg(instr->get_rd(), offset);
};

//-------------------------------------------------------------------
//BRANCHES

void jal(Hart *hart, Instruction *instr)
{
    hart->set_reg(instr->get_rd(), hart->get_pc() + 4);
    hart->print_regs();

    RegVal offset = instr->get_imm();
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), hart->get_pc() + 4);

    offset = ((offset & 0x80000) +     ((offset >> 9) & 0x3ff) + \
             ((offset & 0x100) << 2) + ((offset & 0xff) << 11)) << 1;

    if(offset & 0x100000)
        offset += 0xffe00000;

    printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
    hart->set_pc(hart->get_pc() + offset);
};

void jalr(Hart *hart, Instruction *instr)
{
    RegVal offset = instr->get_imm();

    if(offset & 0x800)
        offset += 0xfffff000;
    offset &= 0xfffffffe;

    offset = (hart->get_reg(instr->get_rs1()) + offset) & 0xfffffffe;

    printf("\nNEXT PC = %x\n\n", offset);
    if(instr->get_rd() != 0)
        hart->set_reg(instr->get_rd(), hart->get_pc() + 4);

    hart->set_pc(offset);
};

void beq(Hart *hart, Instruction *instr)
{
    if(hart->get_reg(instr->get_rs1()) == hart->get_reg(instr->get_rs2()))
    {
        RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

        offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
                 ((offset & 0x001) << 10)) << 1;

        if(offset & 0x1000)
            offset += 0xffffe000;

        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc(hart->get_pc() + offset);
    }
    else
        hart->set_pc(hart->get_pc() + 4);
};

void bne(Hart *hart, Instruction *instr)
{    
    if(hart->get_reg(instr->get_rs1()) != hart->get_reg(instr->get_rs2()))
    {
        RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

        offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
                 ((offset & 0x001) << 10)) << 1;

        if(offset & 0x1000)
            offset += 0xffffe000;

        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc(hart->get_pc() + offset);
    }
    else
        hart->set_pc(hart->get_pc() + 4);
};

void blt(Hart *hart, Instruction *instr)
{
    if((SignedRegVal)hart->get_reg(instr->get_rs1()) < \
       (SignedRegVal)hart->get_reg(instr->get_rs2()))
    {
        RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

        offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
                 ((offset & 0x001) << 10)) << 1;

        if(offset & 0x1000)
            offset += 0xffffe000;

        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc(hart->get_pc() + offset);
    }
    else
        hart->set_pc(hart->get_pc() + 4);
};

void bge(Hart *hart, Instruction *instr)
{
    if((SignedRegVal)hart->get_reg(instr->get_rs1()) >= \
       (SignedRegVal)hart->get_reg(instr->get_rs2()))
    {
        RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

        offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
                 ((offset & 0x001) << 10)) << 1;

        if(offset & 0x1000)
            offset += 0xffffe000;

        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc(hart->get_pc() + offset);
    }
    else
        hart->set_pc(hart->get_pc() + 4);
};

void bltu(Hart *hart, Instruction *instr)
{
    if(hart->get_reg(instr->get_rs1()) < hart->get_reg(instr->get_rs2()))
    {
        RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

        offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
                 ((offset & 0x001) << 10)) << 1;

        if(offset & 0x1000)
            offset += 0xffffe000;

        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc(hart->get_pc() + offset);
    }
    else
        hart->set_pc(hart->get_pc() + 4);
};

void bgeu(Hart *hart, Instruction *instr)
{
    if(hart->get_reg(instr->get_rs1()) >= hart->get_reg(instr->get_rs2()))
    {
        RegVal offset = (instr->get_imm() << 5) + instr->get_rd();

        offset = ((offset & 0x800) + ((offset & 0x7fe) >> 1) + \
                 ((offset & 0x001) << 10)) << 1;

        if(offset & 0x1000)
            offset += 0xffffe000;

        printf("\nNEXT PC = %x\n\n", offset + hart->get_pc());
        hart->set_pc(hart->get_pc() + offset);
    }
    else
        hart->set_pc(hart->get_pc() + 4);
};

//-------------------------------------------------------------------
//  LOADS AND STORES

void lb(Hart *hart, Instruction *instr)
{
    Word word = 0;
    RegVal offset = instr->get_imm();

    if(offset & 0x800)
        offset += 0xfffff000;

    offset = hart->get_reg(instr->get_rs1()) + (SignedRegVal)(offset);

	//(hart->get_mem())->dram_read(&word, , sizeof(Byte));

    if(word & 0x80)
        word += 0xffffff;

    hart->set_reg(instr->get_rs1(), word);
};

void lh(Hart *hart, Instruction *instr)
{
    Word word = 0;

	(hart->get_mem())->dram_read(&word, hart->get_reg(instr->get_rd()), sizeof(HalfWord));

    if(word & 0x8000)
        word += 0xffff;

    hart->set_reg(instr->get_rs1(), word);
};

void lw(Hart *hart, Instruction *instr)
{
    Word word = 0;

	(hart->get_mem())->dram_read(&word, hart->get_reg(instr->get_rd()), sizeof(Word));

    hart->set_reg(instr->get_rs1(), (word));
};

void lbu(Hart *hart, Instruction *instr)
{
    Word word = 0;

	(hart->get_mem())->dram_read(&word, hart->get_reg(instr->get_rd()), sizeof(Byte));

    hart->set_reg(instr->get_rs1(), word);
};

void lhu(Hart *hart, Instruction *instr)
{
    Word word = 0;

	(hart->get_mem())->dram_read(&word, hart->get_reg(instr->get_rd()), sizeof(HalfWord));

    hart->set_reg(instr->get_rs1(), word);
};

void sb(Hart *hart, Instruction *instr)
{
    //(hart->get_mem())->dram_write(hart->get_reg(instr->get_rs2()), );
	//hart->dram_write(instr->get_rs2(), instr->get_rs1(), sizeof(Byte));
};

void sh(Hart *hart, Instruction *instr)
{
	//hart->dram_write(instr->get_rs2(), instr->get_rs1(), sizeof(HalfWord));
};

void sw(Hart *hart, Instruction *instr)
{
	//hart->dram_write(instr->get_rs2(), instr->get_rs1(), sizeof(Word));
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

