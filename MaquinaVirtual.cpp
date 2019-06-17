#include "MaquinaVirtual.hpp"
#include "BitOperators.hpp"
#include "Instrucoes.hpp"

extern PCB * processRunning;

MaquinaVirtual::MaquinaVirtual()
{
    nPc = 0;
    nTipoInst = 0;
    nIRs = 0;
    nIRd = 0;
    nIRc = 0;

    for (int i = 0; i < TAM_MEM; i++)
    {
        if (i < REG_QNT)
            nRegs[i] = 0;

        nMemoria[i] = 0;
    }
}

short MaquinaVirtual::decoInstrucao(int a)
{
    a >>= 1;

    if (testBit(a, 31))
        toogleBit(a, 31);

    return (a >> 26);
}

short MaquinaVirtual::indice26_24(int a)
{
    for (int i = 31; i > 26; i--)
        clearBit(a, i);

    return (a >> 24);
}

short MaquinaVirtual::indice23_21(int a)
{
    for (int i = 31; i > 23; i--)
        clearBit(a, i);

    return (a >> 21);
}

// Retorna a metade do inteiro no qual esta a constante/endereco
short MaquinaVirtual::pegaParametros(int a)
{
    return a;
}

int MaquinaVirtual::executaInst(int * offset, int * nFrame)
{
    int tst = 0;

    // Decodificacao da instrucao
    nTipoInst = decoInstrucao(nMemoria[nPc]);

    // Decodificacao dos indices para o auxilio das opercoes com os registradores
    if ((nTipoInst >= ADD && nTipoInst <= STX) || nTipoInst == MULT) // Sera uma uma intrucao tipo r com o uso de dois registradores
    {
        nIRd = indice26_24(nMemoria[nPc]);
        nIRs = indice23_21(nMemoria[nPc]);
    }
    else if (nTipoInst >= NOT && nTipoInst != STOP) // Instrucao tipo r com um registrador
        nIRd = indice26_24(nMemoria[nPc]);
    else if (nTipoInst >= ADDI && nTipoInst <= STD) // Instrucao tipo i
        nIRd = indice26_24(nMemoria[nPc]);
    else if (nTipoInst >= JMP && nTipoInst <= TRAP) // Instrucao tipo j
    {
        if (nTipoInst >= JMPIG && nTipoInst <= TRAP)
        {
            nIRs = indice26_24(nMemoria[nPc]);  // TRAP Rs == Rt
            nIRc = indice23_21(nMemoria[nPc]);  // TRAP Rc == Rp
        }
        else if (nTipoInst == JMPI)
            nIRs = indice26_24(nMemoria[nPc]);
    }

    // Execucao da instrucao
    switch (nTipoInst)
    {
        // Instrucoes de pulo condicional e incondicional

    case JMP:
    {
        int a = pegaParametros(nMemoria[nPc]);
        *(offset) = a % N_FPOSICOES;
        *(nFrame) = a / N_FPOSICOES;
        nPc = processRunning->getPageTablePos(*nFrame) * N_FPOSICOES + *(offset) - 1;
        processRunning->setFrameAtual(*nFrame);
        break;
    }
    case JMPI:
    {
        int a = nRegs[nIRs];
        *(offset) = a % N_FPOSICOES;
        *(nFrame) = a / N_FPOSICOES;
        nPc = processRunning->getPageTablePos(*nFrame) * N_FPOSICOES + *(offset) - 1;
        processRunning->setFrameAtual(*nFrame);
        break;
    }
    case JMPIG:
    {
        if (nRegs[nIRc] > 0)
        {
            int a = nRegs[nIRs];
            *(offset) = a % N_FPOSICOES;
            *(nFrame) = a / N_FPOSICOES;
            nPc = processRunning->getPageTablePos(*nFrame) * N_FPOSICOES + *(offset);
            processRunning->setFrameAtual(*nFrame);
        }
        else
            if((*offset) < N_FPOSICOES - 1)
            {
                *(offset)++;
                incrementaPC(*offset);
            }
            else
            {
                *(offset) = 0;
                *(nFrame)++;
                processRunning->setFrameAtual(*nFrame);
                incrementaPC(*offset);
            }
        break;
    }
    case JMPIL:
    {
        if (nRegs[nIRc] < 0)
        {
            int a = nRegs[nIRs];
            *(offset) = a % N_FPOSICOES;
            *(nFrame) = a / N_FPOSICOES;
            nPc = processRunning->getPageTablePos(*nFrame) * N_FPOSICOES + *(offset);
            processRunning->setFrameAtual(*nFrame);
        }
        else
            if((*offset) < N_FPOSICOES - 1)
            {
                *(offset)++;
                incrementaPC(*offset);
            }
            else
            {
                *(offset) = 0;
                *(nFrame)++;
                processRunning->setFrameAtual(*nFrame);
                incrementaPC(*offset);
            }
        break;
    }
    case JMPIE:
    {
        if (nRegs[nIRc] == 0)
        {
            int a = nRegs[nIRs];
            *(offset) = a % N_FPOSICOES;
            *(nFrame) = a / N_FPOSICOES;
            nPc = processRunning->getPageTablePos(*nFrame) * N_FPOSICOES + *(offset);
            processRunning->setFrameAtual(*nFrame);
        }
        else
            if((*offset) < N_FPOSICOES - 1)
            {
                *(offset)++;
                incrementaPC(*offset);
            }
            else
            {
                *(offset) = 0;
                *(nFrame)++;
                processRunning->setFrameAtual(*nFrame);
                incrementaPC(*offset);
            }
        break;
    }
    case TRAP:
        nR1 = nRegs[nIRs];
        nR2 = nRegs[nIRc];
        break;

        // Instrucoes tipo I
        // Registrador rd

    case ADDI:
        nRegs[nIRd] += pegaParametros(nMemoria[nPc]);
        break;
    case SUBI:
        nRegs[nIRd] -= pegaParametros(nMemoria[nPc]);
        break;
    case ANDI:
        operAnd(nRegs[nIRd], pegaParametros(nMemoria[nPc]));
        break;
    case ORI:
        operOR(nRegs[nIRd], pegaParametros(nMemoria[nPc]));
        break;
    case LDI: /// Load immediate
        nRegs[nIRd] = pegaParametros(nMemoria[nPc]);
        break;
    case LDD: /// Load direct from data memory
    {
        int a = pegaParametros(nMemoria[nPc]);
        int nOffset = a % N_FPOSICOES;
        int frame = a / N_FPOSICOES;
        short addrs = processRunning->getPageTablePos(frame) * N_FPOSICOES + nOffset ;
        tst = addrs;
        nRegs[nIRd] = nMemoria[addrs];
    }
        break;
    case STD: /// Store direct to data memory
    {
        int a = pegaParametros(nMemoria[nPc]);
        int nOffset = a % N_FPOSICOES;
        int frame = a / N_FPOSICOES;
        short addrs = processRunning->getPageTablePos(frame) * N_FPOSICOES + nOffset;
        tst = addrs;
        nMemoria[addrs] = nRegs[nIRd];
    }
        break;

        // Instrucoes tipo a registrador 2

    case ADD:
        nRegs[nIRd] += nRegs[nIRs];
        break;
    case SUB:
        nRegs[nIRd] -= nRegs[nIRs];
        break;
    case AND:
        operAnd(nRegs[nIRd], nRegs[nIRs]);
        break;
    case OR:
        operOR(nRegs[nIRd], nRegs[nIRs]);
        break;
    case LDX: /// Indirect load from memory
    {
        int nOffset = nRegs[nIRs] % N_FPOSICOES;
        int frame = nRegs[nIRs] / N_FPOSICOES;
        short addrs = processRunning->getPageTablePos(frame) * N_FPOSICOES + nOffset;
        tst = addrs;
        nRegs[nIRd] = nMemoria[addrs];
    }
        break;
    case STX: /// Indirect storage to memory
    {
        int nOffset = nRegs[nIRd] % N_FPOSICOES;
        int frame = nRegs[nIRd] / N_FPOSICOES;
        short addrs = processRunning->getPageTablePos(frame) * N_FPOSICOES + nOffset;
        tst = addrs;
        nMemoria[addrs] = nRegs[nIRs];
    }
        break;
    case MULT:
        nRegs[nIRd] *= nRegs[nIRs];
        break;

        // Instrucoes tipo a registrador 1

    case NOT:
        operNot(nRegs[nIRd]);
        break;
    case SHL: /// Shift register left
        operSHL(nRegs[nIRd], 1);
        break;
    case SHR: /// Shift register right
        operSHR(nRegs[nIRd], 1);
        break;
    case SWAP: /// Swap nibbles
        break;
    case STOP:
        break;

    default:
        printf(">> Instrucao invalida!!!\n");
        break;
    }


        std::cout << "REGESTERS VALUES --------=====\n";
        for(int i = 0 ; i < REG_QNT ; i++)
            std::cout << "Reg<"<< i << ">: " << nRegs[i] << std::endl;
        std::cout << "Instruction: " << nTipoInst << std::endl;
        std::cout << "PC: " << nPc << std::endl;
        std::cout << "RS: " << nIRs << std::endl;
        std::cout << "RD: " << nIRd << std::endl;
        std::cout << "RC: " << nIRc << std::endl;
        std::cout << "R1: " << nR1 << std::endl;
        std::cout << "R2: " << nR2 << std::endl;
        if(nTipoInst == STX || nTipoInst == STD || nTipoInst == LDX || nTipoInst == LDD)
            printf("Memoria<%d>: %#08x", tst, nMemoria[tst]);
        else
            printf("Memoria<%d>: %#08x", nPc, nMemoria[nPc]);
        std::cout << "\n===================================\n\n\n";


   return nTipoInst;
}

bool MaquinaVirtual::validaEndRW(int endereco, int base)
{
    return (endereco > base + 127 || endereco < base + 100) ? false : true;
}

void MaquinaVirtual::incrementaPC(int i)
{
    nPc = processRunning->getFrameAtual() * N_FPOSICOES + i;
}
