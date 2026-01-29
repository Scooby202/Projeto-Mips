/*
 * T2_Memoria.h
 * Biblioteca de Memória para o Simulador MIPS
 */

#ifndef T2_MEMORIA_H
#define T2_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>

// Definição do tamanho da memória conforme descrição (0xFFFF = 65536 palavras)
#define MEM_SIZE 0xFFFF
#define TEXT_LIMIT 0x7FFF  // Metade da memória para instruções

// Variáveis Globais de Memória
unsigned int MEMO[MEM_SIZE];

// Endereços base padrão do MIPS (serão atualizados pelo leArquivo)
unsigned int textBase = 0x00400000;
unsigned int dataBase = 0x10010000;
unsigned int stackBase = 0x7FFFEFFC; // Endereço inicial da pilha [cite: 73]

/* * Função Auxiliar: Mapeia Endereço MIPS (32 bits) -> Índice do Vetor MEMO 
 * Isso satisfaz a necessidade de separar memórias de instrução e dados 
 * dentro de um único array físico.
 */
unsigned int mapAddress(unsigned int address) {
    unsigned int index;
    
    // Se o endereço for da região de DADOS (>= dataBase)
    if (address >= dataBase) {
        // Mapeia para a segunda metade do vetor MEMO
        // (Address - Base) / 4 ajusta byte-addressing para word-addressing
        index = ((address - dataBase) >> 2) + (TEXT_LIMIT + 1);
    } 
    // Se o endereço for da região de INSTRUÇÃO (>= textBase)
    else if (address >= textBase) {
        // Mapeia para a primeira metade do vetor MEMO
        index = (address - textBase) >> 2;
    }
    else {
        // Fallback para endereços absolutos baixos ou inválidos (segurança)
        index = (address >> 2); 
    }

    // Proteção contra acesso fora do vetor
    if (index >= MEM_SIZE) {
        // Em um caso real, lançaria erro. Aqui retornamos 0 ou fazemos wrap.
        return index % MEM_SIZE; 
    }
    
    return index;
}

unsigned int swProgram(unsigned int address, unsigned int content)
{
    // Escreve na memória de instruções
    unsigned int index = mapAddress(address);
    MEMO[index] = content;
    return content;
}

unsigned int lwProgram(unsigned int address)
{
    // Lê da memória de instruções [cite: 66]
    unsigned int index = mapAddress(address);
    return MEMO[index];
}

unsigned int swData(unsigned int address, unsigned int content)
{
    // Escreve na memória de dados [cite: 67]
    unsigned int index = mapAddress(address);
    MEMO[index] = content;
    return content;
}

unsigned int lwData(unsigned int address)
{
    // Lê da memória de dados
    unsigned int index = mapAddress(address);
    return MEMO[index];
}

unsigned int swStack(unsigned int address, unsigned int content)
{
    // A pilha (Stack) ocupará a memória de dados 
    // Portanto, usa a mesma lógica de mapeamento de dados
    unsigned int index = mapAddress(address);
    MEMO[index] = content;
    return content;
}

unsigned int lwStack(unsigned int address)
{
    // Lê da pilha
    unsigned int index = mapAddress(address);
    return MEMO[index];
}

/*
 * Funções de Leitura de Arquivo (Mantidas e integradas com as globais)
 */

unsigned int StringParaInt(char *linha)
{
    unsigned int code_uint = 0, i = 0;
    char c;

    for (i = 0; i < 8; i++)
    {
        code_uint *= 16; 
        c = *(linha + i);
        if (c >= '0' && c <= '9') code_uint += (c - '0');
        else if (c >= 'a' && c <= 'f') code_uint += (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') code_uint += (c - 'A' + 10);
        else return 0;
    }
    return code_uint;
}

int leArquivo(char *nomeArq)
{
    int estado = 0;
    char linha[100];
    unsigned int nLinhasText = 0, nLinhasData = 0;
    unsigned int code, PC = 0, contaLinha = 0;

    FILE *arquivo = fopen(nomeArq, "r");

    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", nomeArq);
        return 0;
    }

    printf("\n\n\t* Leitura do código e dos dados no arquivo %s * \n\n", nomeArq);
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        code = StringParaInt(linha);
        // O parser ignora linhas que retornam 0 (comentários ou vazias)
        // Nota: Se o código hexadecimal real for 00000000 (nop), esta função
        // precisa de cuidado. Assumindo que StringParaInt retorna 0 apenas para erro
        // conforme comentário original.
        
        if (code != 0 || (linha[0] == '0')) // Pequeno ajuste de robustez
        { 
            switch (estado)
            {
            case 0:
                textBase = code; // Define o endereço base do texto [cite: 70]
                estado = 1;
                break;
            case 1:
                nLinhasText = code;
                estado = 2;
                break;
            case 2:
                dataBase = code; // Define o endereço base dos dados
                estado = 3;
                break;
            case 3:
                nLinhasData = code;
                PC = textBase;
                contaLinha = 0;
                estado = 4;
                break;
            case 4:
                // Grava instrução na memória usando endereço absoluto
                swProgram(PC + 4 * contaLinha, code);
                contaLinha++;
                if (contaLinha == nLinhasText)
                {
                    contaLinha = 0;
                    PC = dataBase;
                    estado = 5;
                }
                break;
            case 5:
                // Grava dados na memória
                swData(PC + 4 * contaLinha, code);
                contaLinha++;
                if (contaLinha >= nLinhasData)
                {
                    contaLinha = 0;
                    estado = 6;
                }
                break;
            default:
                estado = 6;
            }
        }
    }

    fclose(arquivo);
    return (nLinhasText);
}

#endif