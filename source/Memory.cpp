#include"../includes.h"
#include"../headers/Memory.h"

Memory::Memory(ifstream *program)
{
    m_program = program;
    m_stack = new RegVal[1024];
};

Memory::~Memory()
{
    delete[] m_stack;
};

RegVal *Memory::get_frame_pointer()
{
    return (m_stack + 1024);
};

ifstream *Memory::get_prog_mem()
{
    return m_program;
};

