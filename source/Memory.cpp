#include"../includes.h"
#include"../headers/Memory.h"

Memory::Memory(ifstream *program)
{
    m_program = program;
    m_stack = new RegVal[1024];
    m_dram = new Byte[mem_size];
};

Memory::~Memory()
{
    delete[] m_stack;
    delete[] m_dram;
};

RegVal *Memory::get_frame_pointer()
{
    return (m_stack + 1024);
};

ifstream *Memory::get_prog_mem()
{
    return m_program;
};

int Memory::dram_read(Word* word, RegVal addr, size_t size)
{
	if(size == sizeof(Byte))
		*word = (Byte) m_dram[addr];	
	else if(size == sizeof(HalfWord))
		*word = (HalfWord) *((HalfWord*) (m_dram + addr * sizeof(Byte)));	
	else if(size == sizeof(Word))
		*word = (Word) *((Word*) (m_dram + addr * sizeof(Byte)));	
	else
		return -1;


	return 0;
};

int Memory::dram_write(Word* word, RegVal addr, size_t size)
{
	if(size == sizeof(Byte))
		m_dram[addr] = *((Byte*) word);
	else if(size == sizeof(HalfWord))
		*((HalfWord*) (m_dram + addr * sizeof(Byte))) = *((HalfWord*) word);
	else if(size == sizeof(Word))
		*((Word*) (m_dram + addr * sizeof(Byte))) = *((Word*) word);	
	else
		return -1;

	return 0;
};


