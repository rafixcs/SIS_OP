//--------------------------------
// GERENTE DE PROCESSOS.H
//--------------------------------

#include "GerenteMemoria.hpp"
#include <list>
#include <iterator>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif // win32

#define TIME_SLICE 250

void sleepcp(int milliseconds);
void trataInterrupcao(int teste);
int criaProcesso(const std::string progName);
void finalizaProcesso();
void testaInterrupcao();
void timerFunc();

