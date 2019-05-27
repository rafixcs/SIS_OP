#include "GerenteProcessos.hpp"
#include "Instrucoes.hpp"
#include <thread>

#define CPU_DELAY 50

bool bRodandoCPU = true;

std::string nomeProg(short option)
{
    std::string file_name;

    switch(option)
        {
        case 1:
            file_name = "Fibonacci10.txt";
            break;
        case 2:
            file_name = "FibonacciN2.txt";
            break;
        case 3:
            file_name = "FatorialN.txt";
            break;
        }

    return file_name;
}

extern MaquinaVirtual * mMV1;
extern std::list<PCB> * listaReady;
extern PCB * processRunning;
extern std::list<PCB> * listaBlocked;

//-------------------------------------------------------
// operacoes sobre a cpu
//-------------------------------------------------------

void incrementaPC()
{
    mMV1->setPC(mMV1->getPC() + 1);
}


/*** testePC testa o PC para ver se nao esta apontando para algum lugar indevido
 * retorna false se esta apontando para algum lugar indevido
 * retorna true se esta tudo certo com o PC
*/
bool testePC()
{
    // Base + 100 = espaco reservado para os dados do programa

    return ( mMV1->getPC() < processRunning->getBase() ||
        mMV1->getPC() >= processRunning->getBase() + 100) ? false : true;
}

//-------------------------------------------------------
// THREAD CPU
//-------------------------------------------------------

void threadCPU()
{
    while(bRodandoCPU)
    {
        testaInterrupcao();

        if(processRunning != nullptr)   // Se processRunning é nullptr significa que nao ha processo rodando
        {
            int instrucao = mMV1->executaInst(processRunning->getBase());

            // Verifica se o PC esta apontando para algum lugar que nao deveria
            // e se ja chegou ao fim do programa
            if(testePC() == false || instrucao == STOP)
            {
                finalizaProcesso();
            }
            else if(instrucao == TRAP)
            {
                incrementaPC();
                trataInterrupcao(TRAP);
            }
            else
            {
                incrementaPC();

                // Atraso da CPU para melhor percpcao do escalonamento
                sleepcp(CPU_DELAY);
            }
        }
    }
}
//-------------------------------------------------------

//-------------------------------------------------------
// THREAD JUNTOS E SHELL_O_NOW
//-------------------------------------------------------

bool testaEndereco(int nEndereco)
{
    int n = listaBlocked->front().getBase() + 127;

    return (nEndereco > n) ? false : true;
}

void liberaFrontListBlocked()
{
    listaBlocked->front().setState(READY);
    listaReady->push_back(listaBlocked->front());
    listaBlocked->pop_front();
}

void threadShell()
{
    while(bRodandoCPU)
    {
        if(!listaBlocked->empty())
            std::cout << ">> Processo <" << listaBlocked->front().getID() << "> pedindo I/O" << std::endl;

        std::string sTt;

        std::cout << ">> Insira Exec para executar um program ou Input para entrar com algum dado: ";
        std::cin >> sTt;

        if(sTt == "Exec" || sTt == "exec")
        {

            int nOption;

            std::cout << ">> 1 - Primeiros 10 digitos da serie de fibonacci\n";
            std::cout << ">> 2 - Fibonacci de N numeros\n";
            std::cout << ">> 3 - Calcular o fatorial de N\n";
            std::cout << ">> 0 - Sair\n\n";

            std::cout << ">> Opcao: ";
            std::cin >> nOption;

            std::cout << std::endl;
            std::cout << std::endl;

            if(nOption < 0 || nOption > 3)
                std::cout << ">> Opcao invalida! Tente novamente...\n";
            else
            {
                if(nOption == 0)
                    bRodandoCPU = false;
                else
                {
                    std::string file_name = nomeProg(nOption);

                    int nProcSt = criaProcesso(file_name);

                    if(nProcSt != 1)
                        std::cout << ">> Error: " << nProcSt << std::endl;

                }
            }
        }
        else if(sTt == "Input" || sTt == "input")
        {
            if(listaBlocked->empty())
                std::cout << ">> Nenhum processo pedindo entrada ou saida de dado!\n";

            else
            {
                int dado;

                int nAddress = listaBlocked->front().getR2() + listaBlocked->front().getBase();

                if(!testaEndereco(nAddress))
                    std::cout << ">> Local de memoria não acessavel!\n";
                else
                {
                    if(listaBlocked->front().getR1() == 1)
                    {
                        std::cout << ">> Processo " << listaBlocked->front().getID() << std::endl;

                        std::cout << ">> Insira o dado a ser computado : ";
                        std::cin >> dado;

                        mMV1->setMEM_Pos(nAddress, dado);

                        liberaFrontListBlocked();
                    }
                }
            }
        }
    }
}
//-------------------------------------------------------

//-------------------------------------------------------
// THREAD TRAP PRINT @R1 = 2
//-------------------------------------------------------
void threadTRAPPrint()
{
    while(bRodandoCPU == true)
    {
        if(!listaBlocked->empty())
        {
            if(listaBlocked->front().getR1() == 2)
            {
                std::cout << "\n\n>> Processo " << listaBlocked->front().getID() << std::endl;

                int nAddress = listaBlocked->front().getR2() + listaBlocked->front().getBase();

                if(!testaEndereco(nAddress))
                    std::cout << ">> Local de memoria não acessavel!\n";
                else
                {
                    std::cout << ">> Imprimindo um dado de saida do processo: "
                        << mMV1->getMEM_Pos(nAddress) << std::endl;
                }

                liberaFrontListBlocked();
            }
        }
    }

}
//-------------------------------------------------------


int main()
{

    std::cout << "==============================================\n";
    std::cout << ">> Maquina Virtual 1.4 - Sistemas Operacionais\n";
    std::cout << "==============================================\n\n";

    std::thread tTimer(timerFunc);
    std::thread tCPU(threadCPU);
    std::thread tShell(threadShell);
    std::thread tTrapPrint(threadTRAPPrint);

    tTimer.join();
    std::cout << "Timer cl\n";
    tCPU.join();
    std::cout << "CPU cl\n";
    tShell.join();
    std::cout << "SHELL cl\n";
    tTrapPrint.join();
    std::cout << "TRAP cl\n";

    std::cout << "\n\n>> Encerrando a execucao da MV 1.4\n\n";

    return 0;
}
