#ifndef __MEMORY__
#define __MEMORY__

#include"../includes.h"

class Memory
{
    private:
        ifstream *m_program;
        RegVal *m_stack;
    public:
        Memory(ifstream *program);

        ifstream *get_prog_mem();
        RegVal *get_frame_pointer();

        ~Memory();
};

#endif
