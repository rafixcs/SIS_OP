#define REG_QNT 8
#include <vector>

enum state{READY, RUNNING, BLOCKED};

class PCB{
private:
    int id;
    std::vector<int> tablePage;
    int nFrameAtual;
    int nPc;
    int nRegs[REG_QNT];
    int nState;
    short nR1;
    short nR2;
public:
    PCB()
    {
        nState = READY;
        for(int i = 0 ; i < REG_QNT ; i++)
            nRegs[i] = 0;
    }

    void setID(int n) { id = n; }
    void setPC(int n) { nPc = n; }
    void setRegN(int n, int val) { nRegs[n] = val; }
    void setState(int s) { nState = s; }
    void setR1(int n) { nR1 = n; }
    void setR2(int n) { nR2 = n; }
    void setPageTablePush_Back(int dado) { tablePage.push_back(dado); }
    void setFrameAtual(int nf) { nFrameAtual = nf; }

    int getID() { return id; }
    int getState() { return nState; }
    int getPC() { return nPc; }
    int getREGN(int n) { return nRegs[n]; }
    short getR1()  { return nR1; }
    short getR2()  { return nR2; }
    int getPageTablePos(int i) { return tablePage[i]; }
    int getPageTableSize() { return tablePage.size(); }
    int getFrameAtual() { return nFrameAtual; }

};
