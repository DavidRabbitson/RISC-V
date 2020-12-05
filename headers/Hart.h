#ifndef __HART__
#define __HART__

#include"../includes.h"
#include"Executor.h"

class Hart
{
    private:
        int m_pc;
        RegVal m_reg[32];
        class Memory *m_memory;

        void fetch(Instr32raw *buf, ifstream *program, int pc);
        void decode(class Instruction *instr, Instr32raw raw);
        void execute(class Executor *exec, class Instruction *instr);

    public:
        Hart(class Memory *mem);

        RegVal get_reg(RegId reg){ return m_reg[reg]; };
        void set_reg(RegId reg, RegVal val){ m_reg[reg] = val; };

        void run();
};

#endif

