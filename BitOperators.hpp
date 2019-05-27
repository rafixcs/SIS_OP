// Manipulacao de bit
#define clearBit(valor, bit) (valor &= ~(1<<bit))   // limpar um bit
#define testBit(valor,bit) (valor & (1<<bit))       // testa se o bit @bit é 1
#define toogleBit(valor,bit) (valor ^= (1<<bit))    // troca o estado logico de @bit

// Operacoes logicas
#define operAnd(valor, bits) (valor &= bits)   // a = a AND b
#define operOR(valor, bits) (valor |= bits)   // a = a OR b
#define operSHL(valor, bits) (valor <<= bits)
#define operSHR(valor, bits) (valor >>= bits)
#define operNot(valor) (valor = ~valor)
