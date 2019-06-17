//--------------------------------------------------
// GERENTE DE PROCESSOS
//--------------------------------------------------

#include "GerenteProcessos.hpp"

extern MaquinaVirtual * mMV1;

extern bool bRodandoCPU;

PCB * processRunning = nullptr;

int nIDProcesses = 0;

std::list<PCB> * listaReady = new std::list<PCB>;
std::list<PCB> * listaBlocked = new std::list<PCB>;

// 0 nao tem interrupcao
// 1 interrupcao do timer
int interrupcao = 0;

void sleepcp(int milliseconds) // Cross-platform sleep function
{
    #ifdef WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif // win32
}

void trataInterrupcao(int teste)
{
    if(!listaReady->empty() || teste == 5)
    {
        if(listaReady->begin()->getState() != RUNNING || teste == 5)
        {
            // Salva o contexto---------------------
            if(processRunning != nullptr)
            {
                for(int i = 0 ; i < REG_QNT ; i++)
                {
                    processRunning->setRegN(i, mMV1->getREG_Pos(i));
                }
                processRunning->setPC(mMV1->getPC());
                processRunning->setR1(mMV1->getR1());
                processRunning->setR2(mMV1->getR2());

                if(teste != 5) // Se teste eh diferente de 5 eh porque uma interrupcao do timer
                {
                    processRunning->setState(READY);
                    listaReady->push_back(*processRunning);
                }
                else
                {
                    processRunning->setState(BLOCKED);
                    listaBlocked->push_back(*processRunning);
                }
            }
            else
                processRunning = new PCB();

            // --------------------------------------------------------

            if(!listaReady->empty() && teste != 5) // Se nao tem nada na lista ready nao faz nada com o processo que estava rodando
            {
                std::list<PCB>::iterator k;
                k = listaReady->begin();
                (*processRunning) = (*k);
                listaReady->pop_front();

                processRunning->setState(RUNNING);

                // Recuperacao do contexto --------------------------------
                mMV1->setPC(processRunning->getPC());

                for(int i = 0 ; i < REG_QNT ; i++)
                    mMV1->setREG_Pos(i, processRunning->getREGN(i));

                mMV1->setR1(processRunning->getR1());
                mMV1->setR2(processRunning->getR2());
                // --------------------------------------------------------
            }
            else if(teste == 5)
            {
                std::cout << "Teste TRAP\n";
                processRunning = nullptr;
            }
        }
    }
    if (teste != 5)
        interrupcao = 0;
}


int criaProcesso(const std::string progName)
{
    PCB * pcb = new PCB();

    short teste = cargaProg(progName, pcb);
    if(teste < 0)
        pcb = nullptr;
    else
    {
        pcb->setID(nIDProcesses);
        pcb->setPC(pcb->getPageTablePos(0) * N_FPOSICOES);
        pcb->setFrameAtual(pcb->getPageTablePos(0));
        listaReady->push_back(*pcb);
    }

    nIDProcesses++;

    return teste;
}

void testaInterrupcao()
{
    if (interrupcao == 1)
        trataInterrupcao(0);
}

void finalizaProcesso()
{
    std::cout << "\n\nProcesso " << processRunning->getID() << " terminado.\n";

    for(int i = 0; i < 128; i++)
        printf("\Memoria<%d>: %#08x\n", i, mMV1->getMEM_Pos(i));

    liberaFrames(processRunning);

    processRunning = nullptr;
}

//-------------------------------------------------
// THREAD TIMER
//-------------------------------------------------

void timerFunc()
{
    while(bRodandoCPU == true )
    {
        sleepcp(TIME_SLICE);
        interrupcao = 1;
    }

}
//-------------------------------------------------
