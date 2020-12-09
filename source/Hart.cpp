#include"../includes.h"
#include"../headers/Hart.h"
#include"../headers/Memory.h"
#include"../headers/Instruction.h"
#include"../headers/Executor.h"

Hart::Hart(Memory *mem)
{
    m_pc = 0;
    m_memory = mem;

    for(int i = 0; i < 32; i++)
        m_reg[i] = 0;

    m_reg[RegName::s0] = (RegVal)m_memory->get_frame_pointer();
    m_reg[RegName::sp] = m_reg[RegName::s0];
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

void Hart::print_regs()
{
    printf("Zero = %08x\n", m_reg[RegName::Zero]);
    printf("ra   = %08x\n", m_reg[RegName::ra]);
    printf("sp   = %08x\n", m_reg[RegName::sp]);
    printf("gp   = %08x\n", m_reg[RegName::gp]);
    printf("tp   = %08x\n", m_reg[RegName::tp]);
    printf("t0   = %08x\n", m_reg[RegName::t0]);
    printf("t1   = %08x\n", m_reg[RegName::t1]);
    printf("t2   = %08x\n", m_reg[RegName::t2]);
    printf("s0   = %08x\n", m_reg[RegName::s0]);
    printf("s1   = %08x\n", m_reg[RegName::s1]);
    printf("a0   = %08x\n", m_reg[RegName::a0]);
    printf("a1   = %08x\n", m_reg[RegName::a1]);
    printf("a2   = %08x\n", m_reg[RegName::a2]);
    printf("a3   = %08x\n", m_reg[RegName::a3]);
    printf("a4   = %08x\n", m_reg[RegName::a4]);
    printf("a5   = %08x\n", m_reg[RegName::a5]);
    printf("a6   = %08x\n", m_reg[RegName::a6]);
    printf("a7   = %08x\n", m_reg[RegName::a7]);
    printf("s2   = %08x\n", m_reg[RegName::s2]);
    printf("s3   = %08x\n", m_reg[RegName::s3]);
    printf("s4   = %08x\n", m_reg[RegName::s4]);
    printf("s5   = %08x\n", m_reg[RegName::s5]);
    printf("s6   = %08x\n", m_reg[RegName::s6]);
    printf("s7   = %08x\n", m_reg[RegName::s7]);
    printf("s8   = %08x\n", m_reg[RegName::s8]);
    printf("s9   = %08x\n", m_reg[RegName::s9]);
    printf("s10  = %08x\n", m_reg[RegName::s10]);
    printf("s11  = %08x\n", m_reg[RegName::s11]);
    printf("t3   = %08x\n", m_reg[RegName::t3]);
    printf("t4   = %08x\n", m_reg[RegName::t4]);
    printf("t5   = %08x\n", m_reg[RegName::t5]);
    printf("t6   = %08x\n", m_reg[RegName::t6]);
};

void Hart::run()
{
    Instr32raw raw = 0;
    Instruction instr;
    Executor exec(this);
    int is_branch = 0;

    ifstream *memory = m_memory->get_prog_mem();

    while(1)
    {
        fetch(&raw, memory, m_pc);

        if(memory->eof())
            break;

        printf("PC[0x%06x]\t0x%08x", m_pc, raw);

        decode(&instr, raw);

        assert((int)instr.get_exec_id() != 47);
        printf("\t EXECID = %d\n", (int)instr.get_exec_id());

        execute(&exec, &instr);

        if(!instr.is_branch())
            m_pc += 4;
    }
};

