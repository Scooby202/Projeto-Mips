

/*
 *  Função para realizar operações na Unidade Lógica e Aritmética.
 *  Incorpora a extensão de sinal do Immediate (Instruction[15-0])
 *  e o Multiplexador de seleção do segundo operando (ALUSrc).
 *  Retorna o resultado da operação e o indicador de resultado Zero.
 *  Outros indicadores deverão ser imcorporados no futuro.
 */

enum AluSrcEnum
{
	Data2,
	IMM,
	Data1
};
enum AluOprEnum
{
	NOP,
	ADD,
	SUB,
	OR,
	SLH,
	XORI,
	SRL,
	SLL,
	LUI,
	ORI,
	AND,
	SLT
};

unsigned int runULA(char ALUop, char ALUsrc, unsigned int imm,
										unsigned int d_1, unsigned int d_2, char *nIgual)
{

	unsigned int Result = 0, operando2;

	// Multiplexador M2
	if (ALUsrc == Data2)
		operando2 = d_2;
	else
		operando2 = imm;

	switch (ALUop)
	{
	case ADD:
		Result = ((int)d_1) + ((int)operando2);
		break;

	case OR:
		Result = d_1 | operando2;
		break;

	case AND:
		Result = d_1 & operando2;
		break;

	case SUB:
		Result = ((int)d_1) - ((int)operando2);
		break;

	case SLL:
		Result = d_1 << imm;
		break;

	case SRL:
		Result = d_1 >> imm;
		break;

	case LUI:
		Result = imm << 16;
		break;

	case ORI:
		Result = d_1 | imm;
		break;

	case SLT:
		if ((int)d_1 < (int)operando2)
		{
			Result = 1;
		}
		else
		{
			Result = 0;
		}
		break;

	case XORI:
		Result = d_1 ^ operando2;
		break;

	default:
		Result = d_1;
		break;
	}

	/* Preenche indicador de comparação usado por instrucoes de desvio (ex: BLTZ)
	 * Se o ponteiro nIgual for fornecido, seta para 1 quando RS < 0, senão 0.
	 */
	if (nIgual)
	{
		*nIgual = (((int)d_1) < 0) ? 1 : 0;
	}
	
	return Result;
}
