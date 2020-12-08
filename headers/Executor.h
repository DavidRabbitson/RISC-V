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

void dummy (Hart *hart, Instruction *instr);//implemented
void lui   (Hart *hart, Instruction *instr);//implemented
void auipc (Hart *hart, Instruction *instr);//implemented

//BRANCHES
void jal   (Hart *hart, Instruction *instr);//implemented?
void jalr  (Hart *hart, Instruction *instr);
void beq   (Hart *hart, Instruction *instr);
void bne   (Hart *hart, Instruction *instr);
void blt   (Hart *hart, Instruction *instr);
void bge   (Hart *hart, Instruction *instr);
void bltu  (Hart *hart, Instruction *instr);
void bgeu  (Hart *hart, Instruction *instr);

//LOADS AND STORES
void lb    (Hart *hart, Instruction *instr);//need test
void lh    (Hart *hart, Instruction *instr);//need test
void lw    (Hart *hart, Instruction *instr);//need test
void lbu   (Hart *hart, Instruction *instr);//need test
void lhu   (Hart *hart, Instruction *instr);//need test
void sb    (Hart *hart, Instruction *instr);//need test
void sh    (Hart *hart, Instruction *instr);//need test
void sw    (Hart *hart, Instruction *instr);//need test

//IMMEDIATE ARITHMETICS
void addi  (Hart *hart, Instruction *instr);//implemented
void slti  (Hart *hart, Instruction *instr);//implemented
void sltiu (Hart *hart, Instruction *instr);//implemented
void xori  (Hart *hart, Instruction *instr);//implemented
void ori   (Hart *hart, Instruction *instr);//implemented
void andi  (Hart *hart, Instruction *instr);//implemented
void slli  (Hart *hart, Instruction *instr);//implemented
void srli  (Hart *hart, Instruction *instr);//implemented
void srai  (Hart *hart, Instruction *instr);//implemented

//REGISTER ARITHMETICS
void add   (Hart *hart, Instruction *instr);//implemented
void sub   (Hart *hart, Instruction *instr);//implemented
void sll   (Hart *hart, Instruction *instr);//implemented
void slt   (Hart *hart, Instruction *instr);//implemented
void sltu  (Hart *hart, Instruction *instr);//implemented
void m_xor (Hart *hart, Instruction *instr);//implemented
void srl   (Hart *hart, Instruction *instr);//implemented
void sra   (Hart *hart, Instruction *instr);//implemented
void m_or  (Hart *hart, Instruction *instr);//implemented
void m_and (Hart *hart, Instruction *instr);//implemented

//SYSTEM CALLS
void fence (Hart *hart, Instruction *instr);
void fencei(Hart *hart, Instruction *instr);
void ecall (Hart *hart, Instruction *instr);
void ebreak(Hart *hart, Instruction *instr);

//CONTROL AND STATUS REGISTER COMMANDS
void csrrw (Hart *hart, Instruction *instr);
void csrrs (Hart *hart, Instruction *instr);
void csrrc (Hart *hart, Instruction *instr);
void csrrwi(Hart *hart, Instruction *instr);
void csrrsi(Hart *hart, Instruction *instr);
void csrrci(Hart *hart, Instruction *instr);

#endif
