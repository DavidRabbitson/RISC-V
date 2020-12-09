#ifndef __HART__
#define __HART__

#include"../includes.h"
#include"Executor.h"

enum RegName
{
    Zero,
    ra, sp, gp, tp,
    t0, t1, t2,
    s0, s1,
    a0, a1, a2, a3, a4, a5, a6, a7,
    s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,
    t3, t4, t5, t6
};

class Hart
{
    private:
        RegVal m_pc;
        RegVal m_reg[32];
        RegVal m_ras[256];
        class Memory *m_memory;

        void fetch(Instr32raw *buf, ifstream *program, int pc);
        void decode(class Instruction *instr, Instr32raw raw);
        void execute(class Executor *exec, class Instruction *instr);
    public:
        Hart(class Memory *mem);

        RegVal get_reg(RegId reg){ return m_reg[reg]; };
        RegVal get_pc(){ return m_pc; };
        void set_reg(RegId reg, RegVal val){ m_reg[reg] = val; };
        void set_pc_offset(SignedRegVal offset){ m_pc += offset; };
        void print_regs();

        void run();
};

#endif

