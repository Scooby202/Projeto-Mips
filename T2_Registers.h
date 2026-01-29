/*
 *  Biblioteca para movimentar dados no banco de registradores "REGISTERS"
 */

/*
 *  32 registradores de 32 bits
 */

unsigned int REGISTERS[0x20];

/* Write Register  */
enum RegDstEnum
{
	RS,
	RT,
	RD
};

/*
 *  Função para ler e escrever dados nos registradores.
 *  Recebe as linhas de controle RegDst e char RegWrite
 *  e os endereços dos registradores rs, rt e rd.
 *  Pode escrever os valores nas palavras de saída data_1 e data_2,
 *  ou pode escrever um valor no registrador de escrita.
 */
void readReg(unsigned int rs, unsigned int rt, unsigned int rd,
						 unsigned int *d_1, unsigned int *d_2, unsigned int *d_3)
{
	/* Por enquanto só faz a leitura de registradores  rs e rt */
	*d_1 = REGISTERS[rs];
	*d_2 = REGISTERS[rt];
	*d_3 = REGISTERS[rd];
}

void writeReg(unsigned int rs, unsigned int rt, unsigned int rd,
							char RegDest, char RegWr, unsigned int newVal)
{
	unsigned int rDest;

	if (RegWr)
	{ /* Multiplexador M1 */
		if (RegDest == RT)
			rDest = rt;
		if (RegDest == RS)
			rDest = rs;
		if (RegDest == RD)
			rDest = rd;
	}
	REGISTERS[rDest] = newVal;
	printf("* Reg * RS: $t%d : %X, RT: $t%d : %X, RD: $t%d : %X.\n", rs, REGISTERS[rs], rt, REGISTERS[rt], rd, REGISTERS[rd]);
}