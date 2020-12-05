#include"../includes.h"
#include"../headers/Instruction.h"

void Instruction::set_fields(Instr32raw instr)
{
    Instr32raw tmp = instr;

    m_opcode = tmp & 0x7f;

    tmp = tmp >> 7;
    m_rd = tmp & 0x1f;

    tmp = tmp >> 5;
    m_funct3 = tmp & 0x7;
    m_imm = tmp & 0xfffff;

    tmp = tmp >> 3;
    m_rs1 = tmp & 0x1f;

    tmp = tmp >> 5;
    m_rs2 = tmp & 0x1f;

    tmp = tmp >> 5;
    m_funct7 = tmp & 0x7f;
};

void Instruction::find_executor()
{
    switch(m_opcode)
    {
        case 0x03:
            m_imm = m_imm >> 8;

            switch(m_funct3)
            {
                case 0x0:
                    m_executor = ExecId::LB;
                    break;

                case 0x1:
                    m_executor = ExecId::LH;
                    break;

                case 0x2:
                    m_executor = ExecId::LW;
                    break;

                case 0x4:
                    m_executor = ExecId::LBU;
                    break;

                case 0x5:
                    m_executor = ExecId::LHU;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        case 0xf:
            m_imm = m_imm >> 8;

            switch(m_funct3)
            {
                case 0x0:
                    m_executor = ExecId::FENCE;
                    break;

                case 0x1:
                    m_executor = ExecId::FENCEi;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        case 0x13:
            m_imm = m_imm >> 8;

            switch(m_funct3)
            {
                case 0x0:
                    m_executor = ExecId::ADDI;
                    break;

                case 0x1:
                    m_executor = ExecId::SLLI;
                    break;

                case 0x2:
                    m_executor = ExecId::SLTI;
                    break;

                case 0x3:
                    m_executor = ExecId::SLTIU;
                    break;

                case 0x4:
                    m_executor = ExecId::XORI;
                    break;

                case 0x5:
                    m_imm = m_imm >> 5;

                    switch(m_imm)
                    {
                        case 0x0:
                            m_executor = ExecId::SRLI;
                            break;

                        case 0x20:
                            m_executor = ExecId::SRAI;
                            break;

                        default:
                            m_executor = ExecId::FAULT;
                            break;
                    }
                    break;

                case 0x6:
                    m_executor = ExecId::ORI;
                    break;

                case 0x7:
                    m_executor = ExecId::ANDI;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        case 0x17:
            m_executor = ExecId::AUIPC;
            break;

        case 0x23:
            m_imm = m_imm >> 13;

            switch(m_funct3)
            {
                case 0x0:
                    m_executor = ExecId::SB;
                    break;

                case 0x1:
                    m_executor = ExecId::SH;
                    break;

                case 0x2:
                    m_executor = ExecId::SW;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        case 0x33:
            m_imm = m_imm >> 13;

            switch(m_funct3)
            {
                case 0x0:
                    switch(m_imm)
                    {
                        case 0x0:
                            m_executor = ExecId::ADD;
                            break;

                        case 0x20:
                            m_executor = ExecId::SUB;
                            break;

                        default:
                            m_executor = ExecId::FAULT;
                            break;
                    }
                    break;

                case 0x1:
                    m_executor = ExecId::SLL;
                    break;

                case 0x2:
                    m_executor = ExecId::SLT;
                    break;

                case 0x3:
                    m_executor = ExecId::SLTU;
                    break;

                case 0x4:
                    m_executor = ExecId::XOR;
                    break;

                case 0x5:
                    switch(m_imm)
                    {
                        case 0x0:
                            m_executor = ExecId::SRL;
                            break;
                        case 0x20:
                            m_executor = ExecId::SRA;
                            break;

                        default:
                            m_executor = ExecId::FAULT;
                            break;
                    }
                    break;

                case 0x6:
                    m_executor = ExecId::OR;
                    break;

                case 0x7:
                    m_executor = ExecId::AND;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        case 0x37:
            m_executor = ExecId::LUI;
            break;

        case 0x63:
            m_imm = m_imm >> 13;

            switch(m_funct3)
            {
                case 0x0:
                    m_executor = ExecId::BEQ;
                    break;

                case 0x1:
                    m_executor = ExecId::BNE;
                    break;

                case 0x4:
                    m_executor = ExecId::BLT;
                    break;

                case 0x5:
                    m_executor = ExecId::BGE;
                    break;

                case 0x6:
                    m_executor = ExecId::BLTU;
                    break;

                case 0x7:
                    m_executor = ExecId::BGEU;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        case 0x67:
            m_executor = ExecId::JALR;
            break;

        case 0x6f:
            m_executor = ExecId::JAL;
            break;

        case 0x73:
            m_imm = m_imm >> 8;

            switch(m_funct3)
            {
                case 0x0:
                    switch(m_imm)
                    {
                        case 0x0:
                            m_executor = ExecId::ECALL;
                            break;

                        case 0x1:
                            m_executor = ExecId::EBREAK;
                            break;

                        default:
                            m_executor = ExecId::FAULT;
                            break;
                    }
                    break;

                case 0x1:
                    m_executor = ExecId::CSRRW;
                    break;

                case 0x2:
                    m_executor = ExecId::CSRRS;
                    break;

                case 0x3:
                    m_executor = ExecId::CSRRC;
                    break;

                case 0x5:
                    m_executor = ExecId::CSRRWI;
                    break;

                case 0x6:
                    m_executor = ExecId::CSRRSI;
                    break;

                case 0x7:
                    m_executor = ExecId::CSRRCI;
                    break;

                default:
                    m_executor = ExecId::FAULT;
                    break;
            }
            break;

        default:
            m_executor = ExecId::FAULT;
            break;
    }
};


