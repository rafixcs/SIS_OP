#define REG_QNT 8

enum state{READY, RUNNING, BLOCKED};

class PCB{
private:
    int id;
    int base;
    int limite;
    int nPc;
    int nRegs[REG_QNT];
    int nState;
    short nR1;
    short nR2;
public:
    PCB()
    {
        nPc = 0;
        id = 0;
        base = 0;
        limite = 0;
        nState = 0;
        for(int i = 0 ; i < REG_QNT ; i++)
            nRegs[i] = 0;
    }

    PCB(int id, int base, int limite, int state)
    {
        nPc = base;
        this->id = id;
        this->base = base;
        this->limite = limite;
        nState = state;

        for(int i = 0 ; i < REG_QNT ; i++)
            nRegs[i] = 0;

    };

    void setID(int n) { id = n; }
    void setBase(int n) { base = n; }
    void setLimite(int n) { limite = n; }
    void setPC(int n) { nPc = n; }
    void setRegN(int n, int val) { nRegs[n] = val; }
    void setState(int s) { nState = s; }
    void setR1(int n) { nR1 = n; }
    void setR2(int n) { nR2 = n; }

    int getID() { return id; }
    int getBase() { return this->base; }
    int getLimite() { return this->limite; }
    int getState() { return nState; }
    int getPC() { return nPc; }
    int getREGN(int n) { return nRegs[n]; }
    short getR1()  { return nR1; }
    short getR2()  { return nR2; }

};
