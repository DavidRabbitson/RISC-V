#ifndef __MEMORY__
#define __MEMORY__

#include"../includes.h"

class Memory
{
    private:
        ifstream *m_program;
        RegVal *m_stack;
        Byte *m_dram;
        
    public:
        Memory(ifstream *program);

        ifstream *get_prog_mem();
        RegVal *get_frame_pointer();
        
        int dram_read(Word* word, RegVal addr, size_t size);
        int dram_write(Word word, RegVal addr, size_t size);
        

        ~Memory();
};

#endif
