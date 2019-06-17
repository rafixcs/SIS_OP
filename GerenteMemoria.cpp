//--------------------------------
// GERENTE DE MEMORIA PAGINADA
//--------------------------------

#include "GerenteMemoria.hpp"

MaquinaVirtual * mMV1 = new MaquinaVirtual();
bool bFrameOcupado[N_FRAMES];

bool framesLivres(int nNPaginas, PCB * pcb)
{
    for(int i = 0; i < N_FRAMES; i++)
    {
        if(bFrameOcupado[i] == false)
        {
           bFrameOcupado[i] = true;
           pcb->setPageTablePush_Back(i);
        }
        if(pcb->getPageTableSize() == nNPaginas) break;
    }

    // Se a quantidade de frames disponiveis para alocar nao for a mesma
    // da quantidade de paginas/frames requeridas retorna false
    return (pcb->getPageTableSize() < nNPaginas) ? false : true;
}

short carregaNaMemoria(const std::string &fileName, PCB * pcb)
{
    FILE *fp;

    if (!(fp = fopen(fileName.c_str(), "r")))
    {
        std::cerr << ">> Nao foi possivel abrir o arquivo!" << std::endl;
        return -1;
    }
    else
    {
        int nNPaginas;

        fscanf(fp, "%d", &nNPaginas);

        // Se o retorno da funcao for falsa significa que a quantidade de
        // frames para alocar nao é o suficiente
        if(!framesLivres(nNPaginas, pcb))
        {
            liberaFrames(pcb);
            fclose(fp);
            return -2;
        }

        int i = 0;
        int j = 0;
        int nPosFrame = pcb->getPageTablePos(i);

        while (!feof(fp))
        {
            if(j > N_FPOSICOES - 1)
            {
                j = 0;
                i++;
                nPosFrame = pcb->getPageTablePos(i);
            }

            int dado;
            fscanf(fp, "%d", &dado);

            mMV1->setMEM_Pos(nPosFrame * N_FPOSICOES + j, dado);
            j++;
        }

        fclose(fp);

        return 1;
    }
}

short cargaProg(const std::string name_file, PCB * pcb)
{
    return carregaNaMemoria(name_file, pcb);
}

void liberaFrames(PCB * pRunnig)
{
    for(int i = 0; i < pRunnig->getPageTableSize(); i++)
        bFrameOcupado[pRunnig->getPageTablePos(i)] = false;
}
