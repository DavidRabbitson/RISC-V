#ifndef __EXECUTOR__
#define __EXECUTOR__

#include"../includes.h"

enum ExecId
{
    LUI, AUIPC,
    JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU,
    SB, SH, SW,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    FENCE, FENCEi,
    ECALL, EBREAK,
    CSRRW, CSRRS, CSRRC, CSRRWI, CSRRSI, CSRRCI,
    FAULT
};

class Executor
{
    private:
        class Hart *m_hart;
        void (* m_executors [48])(class Hart*, class Instruction*);
    public:
        Executor(class Hart *hart);
        void execute(class Instruction *instr);
};

void dummy(Hart *hart, Instruction *instr);
void addi(Hart *hart, Instruction *instr);
void add(Hart *hart, Instruction *instr);

#endif
