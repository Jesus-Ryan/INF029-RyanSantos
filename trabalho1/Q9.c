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

//  Questão 9: batalha naval
// #################################################

#include <stdio.h>
#include <stdlib.h>

// Parâmetro global: quantidade de navios de cada jogador.
// Para aumentar a frota, basta mudar este valor (mínimo 5).
// Tamanhos: 1 navio de 4, 1 de 3, 3 de 1 e os demais (se houver) de 2.
#define NUM_NAVIOS 5

#if NUM_NAVIOS < 5
#error "NUM_NAVIOS deve ser no minimo 5"
#endif

#define TAM 10              // tabuleiro 10 x 10
#define AGUA ' '            // [ ] mar sem nada
#define NAVIO 'N'           // [N] navio posicionado
#define ABATIDO 'O'         // [O] posição do navio abatida
#define TIRO_ERRADO 'X'     // [X] tiro na água

char tabuleiro[2][TAM][TAM];  // tabuleiro de cada jogador (0 e 1)
int celulasRestantes[2];      // partes de navio ainda não abatidas

// lê uma linha do teclado (encerra o programa se a entrada acabar)
void lerLinha(char buf[], int n)
{
    if (fgets(buf, n, stdin) == NULL)
    {
        printf("\nEntrada encerrada.\n");
        exit(0);
    }
}

// tamanho do navio de número "indice" (0 a NUM_NAVIOS - 1)
int tamanhoDoNavio(int indice)
{
    if (indice == 0) return 4;
    if (indice == 1) return 3;
    if (indice <= 4) return 1;
    return 2;
}

// converte um texto como "B7" em linha e coluna (a partir de 0). Retorna 0 se inválido
int lerCelula(char texto[], int *linha, int *coluna)
{
    char letra;
    int numero;

    if (sscanf(texto, " %c%d", &letra, &numero) != 2)
        return 0;

    if (letra >= 'a' && letra <= 'z')
        letra = letra - 'a' + 'A';

    if (letra < 'A' || letra >= 'A' + TAM || numero < 1 || numero > TAM)
        return 0;

    *linha = letra - 'A';
    *coluna = numero - 1;
    return 1;
}

void iniciaTabuleiros()
{
    int p, i, j;

    for (p = 0; p < 2; p++)
    {
        celulasRestantes[p] = 0;
        for (i = 0; i < TAM; i++)
            for (j = 0; j < TAM; j++)
                tabuleiro[p][i][j] = AGUA;
    }
}

// mostra um tabuleiro. Se mostraNavios = 0, esconde os navios (visão do adversário)
void imprimeTabuleiro(char t[TAM][TAM], int mostraNavios)
{
    int i, j;

    printf("     ");
    for (j = 1; j <= TAM; j++)
        printf("%-3d", j);
    printf("\n");

    for (i = 0; i < TAM; i++)
    {
        printf(" %c   ", 'A' + i);
        for (j = 0; j < TAM; j++)
        {
            char c = t[i][j];
            if (c == NAVIO && !mostraNavios)
                c = AGUA;
            printf("[%c]", c);
        }
        printf("\n");
    }
}

// lista o tabuleiro do jogador (com seus barcos e os tiros do oponente)
// e o tabuleiro do adversário (só os tiros certeiros e errados do jogador)
void listarTabuleiros(int jogador)
{
    printf("\n--- Seu tabuleiro (Jogador %d) ---\n", jogador + 1);
    imprimeTabuleiro(tabuleiro[jogador], 1);

    printf("\n--- Tabuleiro do adversario (seus tiros) ---\n");
    imprimeTabuleiro(tabuleiro[1 - jogador], 0);
    printf("\n");
}

// confere se o navio cabe no tabuleiro e não bate em outro navio
int navioCabe(char t[TAM][TAM], int linha, int coluna, int tam, int horizontal)
{
    int k;

    for (k = 0; k < tam; k++)
    {
        int l = linha + (horizontal ? 0 : k);
        int c = coluna + (horizontal ? k : 0);

        if (l >= TAM || c >= TAM)
            return 0;
        if (t[l][c] != AGUA)
            return 0;
    }
    return 1;
}

// o jogador informa onde posicionar cada um dos seus navios
void posicionaNavios(int jogador)
{
    char buf[100];
    int n, k;

    for (n = 0; n < NUM_NAVIOS; n++)
    {
        int tam = tamanhoDoNavio(n);
        int posicionado = 0;

        while (!posicionado)
        {
            int linha, coluna;
            int horizontal = 1;

            printf("Jogador %d - navio %d de %d (tamanho %d). Celula inicial (ex: B3): ",
                   jogador + 1, n + 1, NUM_NAVIOS, tam);
            lerLinha(buf, sizeof(buf));

            if (!lerCelula(buf, &linha, &coluna))
            {
                printf("Celula invalida!\n");
                continue;
            }

            if (tam > 1)
            {
                printf("Orientacao (H = para a direita, V = para baixo): ");
                lerLinha(buf, sizeof(buf));

                if (buf[0] == 'V' || buf[0] == 'v')
                    horizontal = 0;
                else if (buf[0] == 'H' || buf[0] == 'h')
                    horizontal = 1;
                else
                {
                    printf("Orientacao invalida!\n");
                    continue;
                }
            }

            if (!navioCabe(tabuleiro[jogador], linha, coluna, tam, horizontal))
            {
                printf("Posicao invalida: o navio sai do tabuleiro ou bate em outro navio.\n");
                continue;
            }

            for (k = 0; k < tam; k++)
            {
                int l = linha + (horizontal ? 0 : k);
                int c = coluna + (horizontal ? k : 0);
                tabuleiro[jogador][l][c] = NAVIO;
            }
            celulasRestantes[jogador] += tam;
            posicionado = 1;
        }
    }
}

// o jogador escolhe uma célula do tabuleiro do adversário e atira.
// Retorna 1 se o tiro foi realizado, 0 se a célula foi inválida
int atirar(int jogador)
{
    char buf[100];
    int linha, coluna;
    int adversario = 1 - jogador;
    char *alvo;

    printf("Informe a celula do tiro (ex: B3): ");
    lerLinha(buf, sizeof(buf));

    if (!lerCelula(buf, &linha, &coluna))
    {
        printf("Celula invalida!\n");
        return 0;
    }

    alvo = &tabuleiro[adversario][linha][coluna];

    if (*alvo == ABATIDO || *alvo == TIRO_ERRADO)
    {
        printf("Voce ja atirou nessa celula! Escolha outra.\n");
        return 0;
    }

    if (*alvo == NAVIO)
    {
        *alvo = ABATIDO;
        celulasRestantes[adversario]--;
        printf("Acertou um navio!\n");
    }
    else
    {
        *alvo = TIRO_ERRADO;
        printf("Agua!\n");
    }
    return 1;
}

// "limpa" a tela para o outro jogador não ver os navios
void escondeTela()
{
    int i;
    for (i = 0; i < 40; i++)
        printf("\n");
}

int main()
{
    char buf[100];
    int p, atual = 0;

    iniciaTabuleiros();
    printf("=== BATALHA NAVAL ===\nNavios por jogador: %d\n", NUM_NAVIOS);

    for (p = 0; p < 2; p++)
    {
        printf("\n--- Posicionamento do Jogador %d ---\n", p + 1);
        posicionaNavios(p);

        printf("\nSeu tabuleiro:\n");
        imprimeTabuleiro(tabuleiro[p], 1);
        printf("\nPressione ENTER e passe o teclado para o outro jogador...");
        lerLinha(buf, sizeof(buf));
        escondeTela();
    }

    for (;;)
    {
        int adversario = 1 - atual;
        int atirou = 0;

        printf("\n=== Vez do Jogador %d ===\n", atual + 1);

        while (!atirou)
        {
            printf("1 - Listar tabuleiros\n2 - Atirar\nEscolha: ");
            lerLinha(buf, sizeof(buf));

            if (buf[0] == '1')
                listarTabuleiros(atual);
            else if (buf[0] == '2')
                atirou = atirar(atual);
            else
                printf("Opcao invalida!\n");
        }

        if (celulasRestantes[adversario] == 0)
        {
            printf("\nO Jogador %d afundou todos os navios do oponente e VENCEU!\n", atual + 1);
            listarTabuleiros(atual);
            break;
        }

        atual = adversario;
    }

    return 0;
}
