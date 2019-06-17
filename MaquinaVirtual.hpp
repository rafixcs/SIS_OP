#define TAM_MEM 1024
#define REG_QNT 8

#include <iostream>
#include <stdio.h>
#include <string.h>

class MaquinaVirtual{
private:
    short nPc;
    short nTipoInst;
    short nIRs;
    short nIRd;
    short nIRc;
    short nR1; // se conter 1 a TRAP faz leitura, se for 2 faz escrita
    short nR2; // endereco para tratar a TRAP
    unsigned int nMemoria[TAM_MEM];
    int nRegs[REG_QNT];

public:
    MaquinaVirtual();

    short indice23_21(int a);
    short indice26_24(int a);
    short pegaParametros(int a);
    short decoInstrucao(int a);
    bool readFile(const std::string &fileName, int base, int limite);
    int executaInst(int base);

    short getPC() { return nPc; }
    short getTipoInst() { return nTipoInst; }
    short getIRS() { return nIRs; }
    short getIRD() { return nIRd; }
    short getIRC() { return nIRc; }
    short getR1()  { return nR1; }
    short getR2()  { return nR2; }
    int getMEM_Pos(int n) { return nMemoria[n]; }
    int getREG_Pos(int n) { return nRegs[n]; }

    void setREG_Pos(int n, int dado) { nRegs[n] = dado; }
    void setMEM_Pos(int n, int dado) { nMemoria[n] = dado; }
    void setPC(int n) { nPc = n; }
    void setR1(int n) { nR1 = n; }
    void setR2(int n) { nR2 = n; }

};
