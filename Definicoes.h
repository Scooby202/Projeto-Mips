/* 
 *  Enumeracoes e Estruturas
 */
 
#define  TRUE  1
#define  FALSE 0
 
/* Write Register  */
enum RegDstEnum   { RT, RD };

/* Origem do segundo operando da ALU */
enum AluSrcEnum       { Data2, IMM};

/* Operacoes previstas para a ALU */
enum AluOprEnum       { NOP, ADD, SUB, OR, SLH, XOR, SRL, SLL, LUI, ORI};

/* Codigos de operacoes */
enum Codigos {   
op0,	// codigo de operacao 0x00, depende da definicao da funcao 
op1,	// codigo de operacao 0x01, depende do campo RT 
j, 		// jump incondicional
jal,  	// jamp and link incondicional
beq,  	// desvio se conteúdos dos registradores forem iguais
bne, 	// desvio se conteúdos dos registradores forem diferentes
blez,
bgtz,
addi,
addiu,
slti,
sltiu,
andi,
ori,
xori,
lui,
z0,
z1,
z2,
op19,
beql,
bnel,
blezl,
bgtzl,
op24,
op25,
op26,
op27,
op28,
op29,
op30,
op31,
lb,
lh,
lwl,
lw,
lbu,
lhu,
lwr,
op39,
sb,
sh,
swl,
sw,
op44,
op45,
swr,
cache,
ll,
lwc1,
lwc2,
pref,
op52,
ldc1,
ldc2,
op55,
sc,
swc1,
swc2,
op59,
op60,
sdc1,
sdc2,
op63
};


/* Codigos das funcoes */
enum Func {
sll,	// deslocamento logico (bit-a-bit) a� esquerda, fixo
f1,		// Funcao nao definida
srl, 	// deslocamento logico (bit-a-bit) a� direita, fixo
sra, 	// deslocamento aritmetico a� direita
sllv, 	// deslocamento logico a� esquerda, variavel
f5, 	// Funcao nao definida
srlv,
srav,
jr,
jalr,
movz,
movn,
syscall,
break_f13,
f14,
sync,
mfhi,
mthi,
mflo,
mtlo,
f20,
f21,
f22,
f23,
mult,
multu,
TV, //div
divu,
f28,
f29,
f30,
f31,
add,
addu,
sub,
subu,
And,
Or,
Xor,
nor,
f40,
f41,
slt,
sltu,
f44,
f45,
f46,
f47,
tge,
tgeu,
tlt,
tltu,
teq,
f53,
tne,
f54,
f55,
f56,
f57,
f58,
f59,
f60,
f61,
f62,
f63
};


/* Estrutura que contem todos os campos da palavra do codigo de instrucao.  */
 struct campos { unsigned int op, funct, shamt, rs, rt, rd, imm, jump_address; };
 


/* Estrutura que contem todos os campos da palavra de controle.
 * RegDst   - Controle do multiplexador que seleciona se o registrador a ser escrito eh o RT (0) ou o RD (1)
 * Jump     - Salto tipo JR (endereço em RA): TRUE (1) ou FALSE (0)
 * Branch   - Desvio condicional
 * MemRead  - Ler dado da memoria: TRUE (1) ou FALSE (0)
 * MemWrite - Escrever dado da memoria: TRUE (1) ou FALSE (0)
 * MemToReg - Controle do multiplexador que seleciona se o dado a ser escrito no registrador vem da ULA (0) ou da memoria (1)
 * ALUop    - Operacao a ser executada pela ULA (NOP = nenhuma operacao)
 * ALUsrc   - Controle do multiplexador que seleciona a origem do segundo operando para a ULA: Data_2 (0) ou Immediate (1)
 * RegWrite - Escrever dado no registrador destino: TRUE (1) ou FALSE (0)
 */
 struct sinais_de_controle {char RegDst, Jump, Branch, MemRead, MemWrite, 
						   MemToReg, ALUop, ALUsrc, RegWrite;		};
						   
 

