#ifndef __HART__
#define __HART__

#include"../includes.h"
#include"Memory.h"
#include"Instruction.h"

class Hart
{
    private:
        int m_pc;
        RegVal m_reg[32];
        Memory *m_memory;

        void fetch(Instr32raw *buf, ifstream *program, int pc);
        void decode(Instruction *instr, Instr32raw raw);
        void execute();

    public:
        Hart(Memory *mem);

        RegVal get_reg(RegId reg){ return m_reg[reg]; };
        void set_reg(RegId reg, RegVal val){ m_reg[reg] = val; };

        void run();
};

#endif

