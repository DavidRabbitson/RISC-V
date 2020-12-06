#include"../includes.h"
#include"../headers/Hart.h"
#include"../headers/Memory.h"
#include"../headers/Instruction.h"
#include"../headers/Executor.h"

Hart::Hart(Memory *mem)
{
    m_pc = 0;
    for(int i = 0; i < 32; i++)
        m_reg[i] = 0;
    m_memory = mem;
};

void Hart::fetch(Instr32raw *buf, ifstream *program, int pc)
{
    program->seekg(pc, program->beg);
    program->read((char*)buf, sizeof(Instr32raw));
};

void Hart::decode(Instruction *instr, Instr32raw raw)
{
    instr->set_fields(raw);
    instr->find_executor();
};

void Hart::execute(Executor *exec, Instruction *instr)
{
    exec->execute(instr);
};

void Hart::run()
{
    Instr32raw raw = 0;
    Instruction instr;
    Executor exec(this);

    ifstream *memory = m_memory->get_prog_mem();

    while(1)
    {
        fetch(&raw, memory, m_pc);

        if(memory->eof())
            break;

        printf("PC[0x%06x]\t0x%08x", m_pc, raw);

        decode(&instr, raw);

        printf("\t EXECID = %d\n", (int)instr.get_exec_id());

        execute(&exec, &instr);

        m_pc += 4;
    }
};

