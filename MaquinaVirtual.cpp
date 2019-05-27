#include "MaquinaVirtual.hpp"
#include "BitOperators.hpp"
#include "Instrucoes.hpp"

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

bool MaquinaVirtual::readFile(const std::string &fileName, int base, int limite)
{
    FILE *fp;

    if (!(fp = fopen(fileName.c_str(), "r")))
    {
        printf("Nao foi possivel abrir o arquivo!\n");
        return false;
    }
    else
    {
        int i = 0;

        while (!feof(fp) || i >= limite)
        {
            fscanf(fp, "%d", &nMemoria[i + base]);

            i++;
        }

        fclose(fp);

        // Se o programa eh maior que a particao retorna false
        if (i >= limite)
            return false;
        else
            return true;
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

int MaquinaVirtual::executaInst(int base)
{

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
        nPc = pegaParametros(nMemoria[nPc]) - 1 + base;
        break;
    case JMPI:
        nPc = nRegs[nIRs] - 1 + base;
        break;
    case JMPIG:
        if (nRegs[nIRc] > 0)
            nPc = nRegs[nIRs] - 1 + base;
        break;
    case JMPIL:
        if (nRegs[nIRc] < 0)
            nPc = nRegs[nIRs] - 1 + base;
        break;
    case JMPIE:
        if (nRegs[nIRc] == 0)
            nPc = nRegs[nIRs] - 1 + base;
        break;
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
        short addrs = pegaParametros(nMemoria[nPc]) + base;
        if(validaEndRW(addrs, base))
            nRegs[nIRd] = nMemoria[addrs];
    }
        break;
    case STD: /// Store direct to data memory
    {
        short addrs = pegaParametros(nMemoria[nPc]) + base;
        if(validaEndRW(addrs, base))
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
            short addrs = nRegs[nIRs] + base;
            if(validaEndRW(addrs, base))
                nRegs[nIRd] = nMemoria[addrs];
    }
        break;
    case STX: /// Indirect storage to memory
    {

        short addrs = nRegs[nIRd] + base;
        if(validaEndRW(addrs, base))
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


        /*std::cout << "REGESTERS VALUES --------=====\n";
        for(int i = 0 ; i < REG_QNT ; i++)
            std::cout << "Reg<"<< i << ">: " << nRegs[i] << std::endl;
        std::cout << "Instruction: " << nTipoInst << std::endl;
        std::cout << "PC: " << nPc + base << std::endl;
        std::cout << "RS: " << nIRs << std::endl;
        std::cout << "RD: " << nIRd << std::endl;
        std::cout << "RC: " << nIRc << std::endl;
        std::cout << "R1: " << nR1 << std::endl;
        std::cout << "R2: " << nR2 << std::endl;
        printf("Memoria<%d>: %#08x", nIRd, nMemoria[nPc]);
        std::cout << "\n===================================\n\n\n";*/


   return nTipoInst;
}

bool MaquinaVirtual::validaEndRW(int endereco, int base)
{
    return (endereco > base + 127 || endereco < base + 100) ? false : true;
}
