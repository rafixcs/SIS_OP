//--------------------------------
// GERENTE DE MEMORIA
//--------------------------------

#include "GerenteMemoria.hpp"

MaquinaVirtual * mMV1 = new MaquinaVirtual();

const int nParticoes = TAM_MEM / TAM_PARTICAO;

bool bParticaoOcupada[nParticoes];    // Particao n esta livre ou ocupada

//-------------------------------------------
// Retorna a primeira particao livre que achar
int achaParticao()
{
    for(int i = 0; i < nParticoes; i++)
        if(bParticaoOcupada[i] == false)
            return i;

    return -1;
}

//-------------------------------------------
// Acha uma particao livre, aloca e retorna qual a particao
// para colocar o programa
int alocaParticao()
{
    int nParticaoPos = achaParticao();

    if(nParticaoPos > -1)
    {
        bParticaoOcupada[nParticaoPos] = true;
        return nParticaoPos;
    }
    // Returna -1 se nao ha particao livre
    return -1;
}


//-------------------------------------------
void liberaParticao(int p)
{
    bParticaoOcupada[p] = false;
}

//-------------------------------------------
// Carga do programa
// Retorna -1 se nao existe particao disponivel
// Retorna -2 se nao cabe na particao

int cargaProg(const std::string name_file)
{
    int nParticaoPos = alocaParticao();

    if(nParticaoPos > -1)
    {
        int base = TAM_PARTICAO * nParticaoPos;
        int limite = base + 127;

        if(mMV1->readFile(name_file, base, limite) == false)
            return -2;
    }
    else
        return -1;

    return nParticaoPos;
}
