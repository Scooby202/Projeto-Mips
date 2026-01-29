/* Programa para testar as funções de leitura do programa em arquivo texto
 * e testar as funções de escrita e leitura em memória
 */

#include <stdio.h>

#include "T2_Registers.h"
#define REGS

#include "T2_Decodifica_Instrucao.h"

extern unsigned int textBase;
extern unsigned int dataBase;
/* vamos evitar que T2_ULA.h seja carregado duas vezes */
#ifndef ULA
#define ULA
#include "T2_ULA.h"
#endif

int main()
{
	char nomeArq[30] = "teste_0.txt";

	unsigned int SP = 0xFFFFFFFF;				 /* Stack Pointer */
	unsigned int PC, codigo_hexa, PCfim; /* Contador de programa e código */
	unsigned int rs, rt, rd, immediate, shamt, data_1, data_2, data_3, writeData, saidaULA, memData;
	int totalLinhas, contaLinha = 0;
	char Diferente;

	struct ControlStruct controlWord;

	/* Teste de leitura de programa em arquivo texto */
	totalLinhas = leArquivo(nomeArq);
	if (totalLinhas == 0)
		return 1; // se não foi possível ler o arquivo

	for (contaLinha = 0; contaLinha < totalLinhas; contaLinha++)
	{
		codigo_hexa = lwProgram(textBase + 4 * contaLinha);
		controlWord = decodifica(codigo_hexa, &rs, &rt, &rd, &immediate, &shamt);
	}
	printf("\n \n");

	/* Aqui começa a execução do programa */
	PC = textBase;
	PCfim = PC + 4 * totalLinhas;

	while (PC < PCfim)
	{
		codigo_hexa = lwProgram(PC);
		printf("\n PC= %X, codigo_hexa= %X, ", PC, codigo_hexa);
		PC += 4;
		/* decodifica (codigo_hexa, *rs, *rt, *rd, *imm, *shamt )    */
		controlWord = decodifica(codigo_hexa, &rs, &rt, &rd, &immediate, &shamt);
		/* Lê registradores  */
		readReg(rs, rt, rd, &data_1, &data_2, &data_3); /* somente leitura por enquanto */
		/* RunULA (ALUop, ALUsrc, imm, d_1, d_2, *nIgual )   */
		saidaULA = runULA(controlWord.ALUop, controlWord.ALUsrc, immediate, data_1, data_2, &Diferente);

		/* Escrever resultado */
		if (controlWord.MemRead)
		{ /* Le dado da memoria */
			memData = lwData(saidaULA);
		}
		if (controlWord.MemWrite)
		{ /* Escreve dado na memoria */
			swData(saidaULA, data_2);
		}
		if (controlWord.MemToReg)
		{ /* Multiplexador M3 */
			writeData = memData;
		}
		else
		{
			writeData = saidaULA;
		}
		if (controlWord.RegWrite)
		{ /* Escreve no registrador */
			writeReg(rs, rt, rd, controlWord.RegDst, controlWord.RegWrite, writeData);
		}

		/* Definir novo PC se ocorreu Jump ou Branch */

		if (controlWord.Branch && Diferente)
		{
			/* Branch taken: PC <- PC + (immediate << 2) */
			PC = PC + ((int)immediate << 2);
		}
		if (controlWord.Jump)
		{
			unsigned int jump = codigo_hexa & 0x03FFFFFF; // Máscara para 26 bits (0x3FFFFFF)
			PC = (PC & 0xF0000000) | (jump << 2);
			;
		}
	}

	return 0;
}

/*

#
# Exemplo de código binário - Soma dois inteiros - 2025
# Copiar esta sequência em um arquivo texto com o nome "teste_0.txt"
# São lidos apenas os 8 primeiros caracteres de cada linha
# Qualquer linha que contenha um caractere diferente de algum algarismo
# hexadecimal entre os 8 primeiros, será desconsiderada pelo programa.
#

00400000    # Endereco inicial de codigo
00000003    # Linhas de codigo em Hexadecimal
10010000    # Endereco inicial de dados
00000001    # Linhas de dados  em Hexadecimal

# código
3c081234  	lui $8,0x00001234     	3   lui $t0, 0x00001234
34095678  	ori $9,$0,0x00005678  	4   ori $t1, $zero, 0x00005678
01285020  	add $10,$9,$8         	5   add $t2, $t1, $t0

# dados
00000000

# Fim do programa

*/
