// #################################################
//  Instituto Federal da Bahia - Salvador - BA
//  Curso de Análise e Desenvolvimento de Sistemas
//  Disciplina: INF029 - Laboratório de Programação
//  Professor: Renato Novais

//  ----- Dados do Aluno -----
//  Nome:
//  email:
//  Matrícula:
//  Semestre:

//  Questão 8: jogo da velha
// #################################################

#include <stdio.h>
#include <stdlib.h>

#define TAM 3
#define VAZIO ' '
#define MARCA_J1 'X'
#define MARCA_J2 'O'

void iniciaTabuleiro(char t[TAM][TAM])
{
    int i, j;
    for (i = 0; i < TAM; i++)
        for (j = 0; j < TAM; j++)
            t[i][j] = VAZIO;
}

void mostraTabuleiro(char t[TAM][TAM])
{
    int i, j;

    printf("\n      1   2   3\n");
    for (i = 0; i < TAM; i++)
    {
        printf("  %c  ", 'A' + i);
        for (j = 0; j < TAM; j++)
        {
            printf(" %c ", t[i][j]);
            if (j < TAM - 1)
                printf("|");
        }
        printf("\n");
        if (i < TAM - 1)
            printf("     ---+---+---\n");
    }
    printf("\n");
}

// retorna a marca do vencedor, ou VAZIO se ainda não há vencedor
char verificaVencedor(char t[TAM][TAM])
{
    int i;

    for (i = 0; i < TAM; i++)
    {
        if (t[i][0] != VAZIO && t[i][0] == t[i][1] && t[i][1] == t[i][2])
            return t[i][0]; // linha
        if (t[0][i] != VAZIO && t[0][i] == t[1][i] && t[1][i] == t[2][i])
            return t[0][i]; // coluna
    }

    if (t[1][1] != VAZIO)
    {
        if (t[0][0] == t[1][1] && t[1][1] == t[2][2])
            return t[1][1]; // diagonal principal
        if (t[0][2] == t[1][1] && t[1][1] == t[2][0])
            return t[1][1]; // diagonal secundária
    }

    return VAZIO;
}

// lê a célula (ex.: B3). Retorna 1 se o formato e os limites estão corretos
int leCelula(int *linha, int *coluna)
{
    char buf[100];
    char letra;
    int numero;

    if (fgets(buf, sizeof(buf), stdin) == NULL)
    {
        printf("\nEntrada encerrada.\n");
        exit(0);
    }

    if (sscanf(buf, " %c%d", &letra, &numero) != 2)
        return 0;

    if (letra >= 'a' && letra <= 'z')
        letra = letra - 'a' + 'A';

    if (letra < 'A' || letra >= 'A' + TAM || numero < 1 || numero > TAM)
        return 0;

    *linha = letra - 'A';
    *coluna = numero - 1;
    return 1;
}

int main()
{
    char tabuleiro[TAM][TAM];
    char vencedor = VAZIO;
    int jogada, jogador, linha, coluna;
    char marca;

    iniciaTabuleiro(tabuleiro);

    for (jogada = 0; jogada < TAM * TAM && vencedor == VAZIO; jogada++)
    {
        jogador = (jogada % 2 == 0) ? 1 : 2;
        marca = (jogador == 1) ? MARCA_J1 : MARCA_J2;

        mostraTabuleiro(tabuleiro);

        for (;;)
        {
            printf("Jogador %d (%c), informe sua jogada (ex: B3): ", jogador, marca);

            if (!leCelula(&linha, &coluna))
            {
                printf("Celula invalida! Use uma letra de A a C e um numero de 1 a 3.\n");
                continue;
            }
            if (tabuleiro[linha][coluna] != VAZIO)
            {
                printf("Essa celula ja esta ocupada! Escolha outra.\n");
                continue;
            }
            break;
        }

        tabuleiro[linha][coluna] = marca;
        vencedor = verificaVencedor(tabuleiro);
    }

    mostraTabuleiro(tabuleiro);

    if (vencedor == MARCA_J1)
        printf("O Jogador 1 (%c) venceu!\n", MARCA_J1);
    else if (vencedor == MARCA_J2)
        printf("O Jogador 2 (%c) venceu!\n", MARCA_J2);
    else
        printf("Deu velha! Nao houve ganhador.\n");

    return 0;
}
