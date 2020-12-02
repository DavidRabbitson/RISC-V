#ifndef __HART__
#define __HART__

#include"includes.h"
#include"Memory.h"

class Hart
{
    private:
        int m_pc;
        Memory *m_memory;

        void fetch(Instr32raw *buf, ifstream *program, int pc);
        void decode();
        void execute();

    public:
        Hart(Memory *mem);
        void run();
};

Hart::Hart(Memory *mem)
{
    m_pc = 0;
    m_memory = mem;
};

void Hart::fetch(Instr32raw *buf, ifstream *program, int pc)
{
    program->seekg(pc, program->beg);
    program->read((char*)buf, sizeof(Instr32raw));
};

void Hart::run()
{
    Instr32raw buf = 0;
    ifstream *memory = m_memory->get_prog_mem();

    while(1)
    {
        fetch(&buf, memory, m_pc);

        if(memory->eof())
            break;

        printf("PC[0x%04x]\t0x%08x\n", m_pc, buf);

        m_pc += 4;
    }
};

#endif

