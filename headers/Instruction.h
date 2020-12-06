#ifndef __INSTRUCTION__
#define __INSTRUCTION__

#include"../includes.h"
#include"Executor.h"

class Instruction
{
    private:
        Word m_imm;
        RegVal m_rd;
        RegVal m_rs1;
        RegVal m_rs2;
        Byte m_opcode;
        Byte m_funct3;
        Byte m_funct7;
        ExecId m_executor;
    public:
        RegVal get_rd() { return m_rd; };
        RegVal get_rs1(){ return m_rs1; };
        RegVal get_rs2(){ return m_rs2; };
        RegVal get_imm(){ return m_imm; };
        void set_fields(Instr32raw instr);
        void find_executor();
        ExecId get_exec_id(){ return m_executor; };
};

#endif
