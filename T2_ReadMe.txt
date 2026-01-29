Encontra-se aqui o programa em linguagem C que será a base para o desenvolvimento do trabalho prático 2. 

São 5 arquvivos:

1) T2_principal.c
   É o programa principal que incluirá todas as bibliotecas (.h) e comandará a execução.
   
2) T2_Memoria.h 
   É a biblioteca da memória. Simula a memória do MIPS, utilizando um vetor de apenas 65536 posições, unsigned int MEMO[0xFFFF].
   Oferece 6 funções para escrita e leitura:
   swProgram - escreve uma palavra de código (32 bits) na memória de programa (.text)
   lwProgram - lê uma palavra de código (32 bits) da memória de programa (.text)
   swData    - escreve uma palavra de dado (32 bits) na memória de dados (.data)
   lwData    - lê uma palavra de dado (32 bits) da memória de dados (.data)
   swStack   - escreve um valor de 32 bits na memória de pilha, apontada pelo Stack Pointer
   lwStack   - lê um valor de 32 bits da memória de pilha, apontada pelo Stack Pointer
   
   Função "leArquivo"
   Além das funções de leitura e escrita em memória, a biblioteca T2_Memória.h possui uma função para ler o programa Assembly
   em um arquivo texto e armazená-lo em código hexadecimal na memória (MEMO).
   Utiliza uma máquina de estados para a leitura.
   Linhas do arquivo texto que possuam caracteres diferentes dos 16 hexadecimais, nas 8 primeiras colunas, são desprezadas.
   As 4 primeira linhas úteis devem conter:
   - Endereco inicial de codigo (Ex.: 00400000);
   - Número de linhas de codigo (Ex.: 00000011), tem que ser escrito com 8 dígitos hexadecimais; 
   - Endereco inicial de dados  (Ex.: 10010000);
   - Número de linhas de dados  (Ex.: 00000005), tem que ser escrito com 8 dígitos hexadecimais.
   As demais linhas úteis deverão conter a sequência de código do programa (em hexadecimais) 
   e a sequência de dados (em hexadecimais), se houver.
      
3) T2_Decodifica_Instrucao.h
   É aqui que começa o trabalho.
   Tendo carregado o programa na memória (MEMO), agora o programa principal deverá colocar o PC (contador de programa do MIPS)
   no endereço inicial (Ex.: 00400000) e executar os ciclos de instrução (busca, decodificação, execução).
   A função "decodifica", traz como exemplo a decodificação dos OP_codes "add", "lui" e "ori".
   A decodificação de cada instrução implica em atribuir valores aos membros da estrutura "controlWord", que representam
   os sinais de controle do MIPS.
   
4) T2_Registradores.h
   É uma biblioteca que simula o conjunto de 32 registradores do MIPS.
   Oferece a função readReg para ler dois valores dos registradores rs e rt 
   e a função writeReg para escrever um valor em um registrador. O multiplexador M1 está incorporado nesta função.    
   
5) T2_ULA.h
   Parte do programa que simula o funcionamento da Unidade Lógica e Aritmética.
   A função "runULA" recebe o código de operação "ALUop" e os possíveis operandos e o sinal de controle ALUsrc.
   O multiplexador M2 está incorporado nessa função. 
   A operação definida por ALUop será executada e seu resultado será enviado como variável de retorno. 
   Algumas operações, como ADD e OR já estão implementadas. As demais deverão ser feitas. 




#
# Código de máquina para calcular os 9 primeiros elementos da Série de Fibonacci
# Restrições: SEM ADDI, ADDIU, BNE, BEQ
#
00400000    # Endereco inicial de codigo
00000012    # 18 Linhas de codigo em Hexadecimal (Total de instrucoes)
10010000    # Endereco inicial de dados
00000000    # 0 Linhas de dados em Hexadecimal

# CÓDIGO (18 Instruções)

# -- INICIALIZAÇÃO DE VARIÁVEIS (F0, F1 e Contador) --
34080000    # 1. ori $t0, $zero, 0   # $t0 (F_n-2) = 0
34090001    # 2. ori $t1, $zero, 1   # $t1 (F_n-1) = 1
340B0007    # 3. ori $t3, $zero, 7   # $t3 (Contador) = 7

# -- INICIALIZAÇÃO DE CONSTANTES (PARA SUBSTITUIR ADDI/ADDIU) --
3C0D1001    # 4. lui $t5, 0x1001     # $t5 (Base de Dados) = 0x10010000
34100004    # 5. ori $s0, $zero, 4   # $s0 = Constante 4 (para incremento de endereço)
34110008    # 6. ori $s1, $zero, 8   # $s1 = Constante 8 (para salto inicial de endereço)
34120001    # 7. ori $s2, $zero, 1   # $s2 = Constante 1 (para decremento do contador)

# -- PRÉ-LOOP: SALVAMENTO INICIAL --
AD080000    # 8. sw $t0, 0($t5)      # Salva F0 (0)
AD290004    # 9. sw $t1, 4($t5)      # Salva F1 (1)
03AE6820    # 10. add $t5, $t5, $s1  # [NOVO: ADD] $t5 = $t5 + 8 (Avança a base para F2)

# -- LOOP (Cálculo de F2 a F8) --
# Endereço do Loop: 0x00400028
01095020    # 11. add $t2, $t0, $t1  # $t2 = F(n)
AD4A0000    # 12. sw $t2, 0($t5)     # Salva F(n)

01204020    # 13. add $t0, $t1, $zero# [NOVO: ADD] $t0 = $t1 (Atualiza F_n-2)
01404820    # 14. add $t1, $t2, $zero# [NOVO: ADD] $t1 = $t2 (Atualiza F_n-1)

03A56820    # 15. add $t5, $t5, $s0  # [NOVO: ADD] $t5 = $t5 + 4 (Avança endereço)
02625822    # 16. sub $t3, $t3, $s2  # [NOVO: SUB] $t3 = $t3 - 1 (Decrementa contador)

1B60FFF9    # 17. bgtz $t3, 0x00400028 # [NOVO: BGTZ] Se $t3 > 0, volta para o Loop (Linha 11)
# -- FIM DO LOOP --

08100011    # 18. j 0x00400044       # HALT: Loop infinito para parar o programa.
