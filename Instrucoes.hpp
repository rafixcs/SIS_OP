// Instrucoes de pulo condicional e incondicional
#define JMP 0   /// Direct Jump
#define JMPI 1
#define JMPIG 2
#define JMPIL 3
#define JMPIE 4
#define TRAP 5

// Instrucoes tipo I

#define ADDI 8
#define SUBI 9
#define ANDI 10
#define ORI 11
#define LDI 12  /// Load immediate
#define LDD 13  /// Load direct from data memory
#define STD 14  /// Store direct to data memory

// Instrucoes tipo a registrador 2

#define ADD 16
#define SUB 17
#define AND 18
#define OR 19
#define LDX 20  /// Indirect load from memory
#define STX 21  /// Indirect storage to memory
#define MULT 28

// Instrucoes tipo a registrador 1

#define NOT 24
#define SHL 25  /// Shift register left
#define SHR 26  /// Shift register right
#define SWAP 27 /// Swap nibbles

// Insturcao para terminar o programa
#define STOP 31

