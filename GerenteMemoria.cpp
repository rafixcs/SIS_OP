//--------------------------------
// GERENTE DE MEMORIA
//--------------------------------

#include "GerenteMemoria.hpp"

bool bFrameOcupado[64];

int FramesDisp(int nNPaginas)
{
    int nvFrames[nNPaginas + 1];

    memset(nvFrames, 0, sizeof(nvFrames));

    int j = 0;

    for(int i = 0; i < nNPaginas; i++)
    {
        if(bFrameOcupado[i] == false)
        {
            bFrameOcupado[i] = true;
            nvFrames[j] = i;
            j++;
        }
    }

    nvFrames[j] =

    return nvFrames;
}

int carregaNaMemoria(const std::string &fileName)
{
    FILE *fp;

    if (!(fp = fopen(fileName.c_str(), "r")))
    {
        printf("Nao foi possivel abrir o arquivo!\n");
        return -1;
    }
    else
    {
        int i = 0;
        int nNPaginas = 0;

        fscanf(fp, "%d", &nNPaginas);

        int nvPaginas[nNPaginas] = FramesDisp(nNPaginas);

        while (!feof(fp))
        {
            int nPosFrame = nvPaginas[i];

            for (int j = 0; j < 16; j++)
                fscanf(fp, "%d", &nMemoria[j * nPosFrame]);

            int i++;
        }

        fclose(fp);

        return nNPaginas;
    }
}

void cargaProg(const std::string name_file)
{
    int carregaNaMemoria(name_file);
}
