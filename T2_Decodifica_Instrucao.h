/*
 *  Biblioteca para ler o código hexadecimal armazenado na memória "MEMO"
 *  e decodificá-lo, gerando os sinais de controle.
 */

/* vamos evitar que T2_Memoria.h seja carregado duas vezes */
#ifndef MEMORIA
#define MEMORIA
#include "T2_Memoria.h"
#endif

/* vamos evitar que T2_ULA.h seja carregado duas vezes */
#ifndef ULA
#define ULA
#include "T2_ULA.h"
#endif

/* vamos evitar que T2_Registers.h seja carregado duas vezes */
#ifndef REGS
#define REGS
#include "T2_Registers.h"
#endif

enum Codigos
{
	op0, // código de operação 0x00, depende da definição da função
	op1, // código de operação 0x01, depende da definição da função
	j,	 // jump incondicional
	jal, // jamp and link incondicional
	beq, // desvio se conteúdos dos registradores forem iguais
	bne, // desvio se conteúdos dos registradores forem diferentes
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

enum Func
{
	sll,	// deslocamento lógico (bit-a-bit) à esquerda, fixo
	f1,		// Função não definida
	srl,	// deslocamento lógico (bit-a-bit) à direita, fixo
	sra,	// deslocamento aritmético à direita
	sllv, // deslocamento lógico à esquerda, variável
	f5,		// Função não definida
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
	TV, // div
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

#define TRUE 1
#define FALSE 0

struct ControlStruct
{
	char RegDst, Jump, Branch, MemRead, MemWrite,
			MemToReg, ALUop, ALUsrc, RegWrite;
};
/* Estrutura que contém todos os campos da palavra de controle.
 * RegDst   - Controle do multiplexador que seleciona se o registrador a ser escrito é o RT (0) ou o RD (1)
 * Jump     - Salto tipo JR (endereço em RA): TRUE (1) ou FALSE (0)
 * Branch   - Desvio condicional
 * MemRead  - Ler dado da memória: TRUE (1) ou FALSE (0)
 * MemWrite - Escrever dado da memória: TRUE (1) ou FALSE (0)
 * MemToReg - Controle do multiplexador que seleciona se o dado a ser escrito no registrador vem da ULA (0) ou da memória (1)
 * ALUop    - Operação a ser executada pela ULA (NOP = nenhuma operação)
 * ALUsrc   - Controle do multiplexador que seleciona a origem do segundo operando para a ULA: Data_2 (0) ou Immediate (1)
 * RegWrite - Escrever dado no registrador destino: TRUE (1) ou FALSE (0)
 */

struct ControlStruct setControlWord(char RegDst, char Jump, char Branch,
																		char MemRead, char MemWrite, char MemToReg,
																		char ALUop, char ALUsrc, char RegWrite)
{
	struct ControlStruct CW;
	CW.RegDst = RegDst;
	CW.Jump = Jump;
	CW.Branch = Branch;
	CW.MemRead = MemRead;
	CW.MemWrite = MemWrite;
	CW.MemToReg = MemToReg;
	CW.ALUop = ALUop;
	CW.ALUsrc = ALUsrc;
	CW.RegWrite = RegWrite;
	return CW;
}

struct ControlStruct decodifica(unsigned int codigo_hexa,
																unsigned int *rs, unsigned int *rt, unsigned int *rd,
																unsigned int *imm, unsigned int *shamt)
{

	struct ControlStruct controlWord;

	unsigned int op_code = 0, funct = 0;

	op_code = codigo_hexa >> 26;							/* Os  6 MSB */
	*rs = (codigo_hexa >> 21) & 0x0000001F;		/* Registrador RS */
	*rt = (codigo_hexa >> 16) & 0x0000001F;		/* Registrador RT */
	*rd = (codigo_hexa >> 11) & 0x0000001F;		/* Registrador RD */
	*shamt = (codigo_hexa >> 6) & 0x0000001F; /* Shift Amount */
	funct = codigo_hexa & 0x0000003F;					/* Os  6 LSB (para instrução R) */
	*imm = codigo_hexa & 0x0000FFFF;					/* Os 16 LSB (para instrução I) */

	if (*imm & 0x00008000)
		*imm |= 0xFFFF0000; /* extensão de sinal */

	/* setControlWord (RegDst, Jump, Branch, MemRead, MemWrite, MemToReg, ALUop, ALUsrc, RegWrite) */

	switch (op_code)
	{

	case op0:

		switch (funct)
		{
		case add:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, ADD, Data2, TRUE);
			break;

		case sll:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, SLL, IMM, TRUE);
			break;

		case srl:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, SRL, IMM, TRUE);
			break;

		case Or:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, OR, Data2, TRUE);
			break;

		case And:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, AND, Data2, TRUE);
			break;

		case sub:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, SUB, Data2, TRUE);
			break;
		// testa depois
		case slt:
			controlWord = setControlWord(RD, FALSE, FALSE, FALSE, FALSE, FALSE, SLT, Data2, TRUE);
			break;
		// testa depois
		case xori:
			controlWord = setControlWord(RT, FALSE, FALSE, FALSE, FALSE, FALSE, XORI, IMM, TRUE);
			break;

		default:
			printf("Erro1 \n");
			break;
		}
		break;

	case lw:
		controlWord = setControlWord(RT, FALSE, FALSE, TRUE, FALSE, TRUE, ADD, IMM, TRUE);
		break;

	case sw:
		controlWord = setControlWord(RT, FALSE, FALSE, FALSE, TRUE, FALSE, ADD, IMM, FALSE);
		break;

	case j:
		controlWord = setControlWord(RT, TRUE, FALSE, FALSE, FALSE, FALSE, NOP, IMM, FALSE);
		break;

	case ori:
		controlWord = setControlWord(RT, FALSE, FALSE, FALSE, FALSE, FALSE, ORI, IMM, TRUE);
		break;

	case lui:
		controlWord = setControlWord(RT, FALSE, FALSE, FALSE, FALSE, FALSE, LUI, IMM, TRUE);
		break;

	case op1:
            switch (*rt) {
                case 0: 
                    controlWord = setControlWord (RT, FALSE, TRUE, FALSE, FALSE, FALSE, NOP, IMM, FALSE);
                break;
            }
    break;

	default:
		printf("Erro2 \n");
		break;
	}
	return controlWord;
}
