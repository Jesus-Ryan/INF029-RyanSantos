// #################################################
//  Instituto Federal da Bahia
//  Salvador - BA
//  Curso de Análise e Desenvolvimento de Sistemas http://ads.ifba.edu.br
//  Disciplina: INF029 - Laboratório de Programação
//  Professor: Renato Novais - renato@ifba.edu.br

//  ----- Orientações gerais -----
//  Descrição: esse arquivo deve conter as questões do trabalho do aluno.
//  O aluno deve preencher seus dados abaixo, e implementar as questões do trabalho

//  ----- Dados do Aluno -----
//  Nome: Ryan Jesus Bispo dos Santos
//  email: ryanbispo2018@gmail.com
//  Matrícula: 20252160033
//  Semestre: 2026.2

//  Copyright © 2016 Renato Novais. All rights reserved.
// Última atualização: 07/05/2021 - 19/08/2016 - 17/10/2025

// #################################################

#include <stdio.h>
#include "trabalho1.h" 
#include <stdlib.h>

DataQuebrada quebraData(char data[]);

/* ---------- funções auxiliares (usadas internamente) ---------- */
static int ehBissexto(int ano);
static int diasNoMes(int mes, int ano);
static int dataExiste(int dia, int mes, int ano);
static int comparaDatas(int d1, int m1, int a1, int d2, int m2, int a2);
static long numeroDoDia(int dia, int mes, int ano);
static void somaMeses(DataQuebrada base, int meses, int *dia, int *mes, int *ano);
static int ehContinuacao(char c);
static int proximoCaractere(char *s, int *i);
static int semAcento(int c);
static int minuscula(int c);

/*
## função utilizada para testes  ##

 somar = somar dois valores
@objetivo
    Somar dois valores x e y e retonar o resultado da soma
@entrada
    dois inteiros x e y
@saida
    resultado da soma (x + y)
 */
int somar(int x, int y)
{
    int soma;
    soma = x + y;
    return soma;
}

/*
## função utilizada para testes  ##

 fatorial = fatorial de um número
@objetivo
    calcular o fatorial de um número
@entrada
    um inteiro x
@saida
    fatorial de x -> x!
 */
int fatorial(int x)
{ //função utilizada para testes
  int i, fat = 1;
    
  for (i = x; i > 1; i--)
    fat = fat * i;
    
  return fat;
}

int teste(int a)
{
    int val;
    if (a == 2)
        val = 3;
    else
        val = 4;

    return val;
}

/*
 Q1 = validar data
@objetivo
    Validar uma data
@entrada
    uma string data. Formatos que devem ser aceitos: dd/mm/aaaa, onde dd = dia, mm = mês, e aaaa, igual ao ano. dd em mm podem ter apenas um digito, e aaaa podem ter apenas dois digitos.
@saida
    0 -> se data inválida
    1 -> se data válida
 @restrições
    Não utilizar funções próprias de string (ex: strtok)   
    pode utilizar strlen para pegar o tamanho da string
 */
int q1(char data[])
{
  // quebra a string em dia, mês e ano (sem usar funções de string)
  DataQuebrada dq = quebraData(data);

  if (dq.valido == 0)
      return 0;

  // confere se o dia existe naquele mês (considerando ano bissexto)
  if (dataExiste(dq.iDia, dq.iMes, dq.iAno))
      return 1;
  else
      return 0;
}



/*
 Q2 = diferença entre duas datas
 @objetivo
    Calcular a diferença em anos, meses e dias entre duas datas
 @entrada
    uma string datainicial, uma string datafinal. 
 @saida
    Retorna um tipo DiasMesesAnos. No atributo retorno, deve ter os possíveis valores abaixo
    1 -> cálculo de diferença realizado com sucesso
    2 -> datainicial inválida
    3 -> datafinal inválida
    4 -> datainicial > datafinal
    Caso o cálculo esteja correto, os atributos qtdDias, qtdMeses e qtdAnos devem ser preenchidos com os valores correspondentes.
 */
DiasMesesAnos q2(char datainicial[], char datafinal[])
{

    //calcule os dados e armazene nas três variáveis a seguir
    DiasMesesAnos dma;
    dma.qtdDias = 0;
    dma.qtdMeses = 0;
    dma.qtdAnos = 0;

    if (q1(datainicial) == 0){
      dma.retorno = 2;
      return dma;
    }else if (q1(datafinal) == 0){
      dma.retorno = 3;
      return dma;
    }else{
      DataQuebrada ini = quebraData(datainicial);
      DataQuebrada fim = quebraData(datafinal);
      int d, m, a;
      int totalMeses = 0;

      //verifique se a data final não é menor que a data inicial
      if (comparaDatas(ini.iDia, ini.iMes, ini.iAno, fim.iDia, fim.iMes, fim.iAno) > 0){
        dma.retorno = 4;
        return dma;
      }

      //calcule a distancia entre as datas
      // 1) descobre o maior número de meses inteiros que cabem entre as datas.
      //    Somar meses mantém o dia original; se o mês não tem esse dia
      //    (ex.: 29/02 em ano não bissexto), usa o último dia do mês.
      for (;;){
        somaMeses(ini, totalMeses + 1, &d, &m, &a);
        if (comparaDatas(d, m, a, fim.iDia, fim.iMes, fim.iAno) > 0)
          break;
        totalMeses++;
      }

      // 2) os dias são o que sobra entre essa data e a data final
      somaMeses(ini, totalMeses, &d, &m, &a);
      dma.qtdAnos = totalMeses / 12;
      dma.qtdMeses = totalMeses % 12;
      dma.qtdDias = (int)(numeroDoDia(fim.iDia, fim.iMes, fim.iAno) - numeroDoDia(d, m, a));

      //se tudo der certo
      dma.retorno = 1;
      return dma;
      
    }
    
}

/*
 Q3 = encontrar caracter em texto
 @objetivo
    Pesquisar quantas vezes um determinado caracter ocorre em um texto
 @entrada
    uma string texto, um caracter c e um inteiro que informa se é uma pesquisa Case Sensitive ou não. Se isCaseSensitive = 1, a pesquisa deve considerar diferenças entre maiúsculos e minúsculos.
        Se isCaseSensitive != 1, a pesquisa não deve  considerar diferenças entre maiúsculos e minúsculos.
 @saida
    Um número n >= 0.
 */
int q3(char *texto, char c, int isCaseSensitive)
{
    int qtdOcorrencias = 0;
    int i = 0;
    int alvo = semAcento((unsigned char) c); // 'á' passa a valer 'a'

    if (isCaseSensitive != 1)
        alvo = minuscula(alvo);

    while (texto[i] != '\0')
    {
        // lê o próximo caractere (trata acentos em UTF-8 e em Latin-1)
        int atual = semAcento(proximoCaractere(texto, &i));

        if (isCaseSensitive != 1)
            atual = minuscula(atual);

        if (atual == alvo)
            qtdOcorrencias++;
    }

    return qtdOcorrencias;
}

/*
 Q4 = encontrar palavra em texto
 @objetivo
    Pesquisar todas as ocorrências de uma palavra em um texto
 @entrada
    uma string texto base (strTexto), uma string strBusca e um vetor de inteiros (posicoes) que irá guardar as posições de início e fim de cada ocorrência da palavra (strBusca) no texto base (texto).
 @saida
    Um número n >= 0 correspondente a quantidade de ocorrências encontradas.
    O vetor posicoes deve ser preenchido com cada entrada e saída correspondente. Por exemplo, se tiver uma única ocorrência, a posição 0 do vetor deve ser preenchido com o índice de início do texto, e na posição 1, deve ser preenchido com o índice de fim da ocorrencias. Se tiver duas ocorrências, a segunda ocorrência será amazenado nas posições 2 e 3, e assim consecutivamente. Suponha a string "Instituto Federal da Bahia", e palavra de busca "dera". Como há uma ocorrência da palavra de busca no texto, deve-se armazenar no vetor, da seguinte forma:
        posicoes[0] = 13;
        posicoes[1] = 16;
        Observe que o índice da posição no texto deve começar ser contado a partir de 1.
        O retorno da função, n, nesse caso seria 1;

 */
int q4(char *strTexto, char *strBusca, int posicoes[30])
{
    int qtdOcorrencias = 0;
    int tamBytes = 0;   // tamanho da palavra de busca em bytes
    int tamChars = 0;   // tamanho da palavra de busca em caracteres
    int i, k;
    int pos = 0;        // posição (a partir de 1) do caractere atual

    for (i = 0; strBusca[i] != '\0'; i++)
    {
        tamBytes++;
        if (!ehContinuacao(strBusca[i]))
            tamChars++;
    }

    if (tamBytes == 0)
        return 0;

    i = 0;
    while (strTexto[i] != '\0')
    {
        if (ehContinuacao(strTexto[i])) // 2º byte de um caractere acentuado
        {
            i++;
            continue;
        }

        pos++;

        // compara a palavra de busca com o texto a partir de i
        for (k = 0; k < tamBytes && strTexto[i + k] == strBusca[k]; k++)
        {
        }

        if (k == tamBytes) // achou
        {
            if (qtdOcorrencias < 15) // o vetor só comporta 15 pares (30 posições)
            {
                posicoes[2 * qtdOcorrencias] = pos;
                posicoes[2 * qtdOcorrencias + 1] = pos + tamChars - 1;
            }
            qtdOcorrencias++;
            i += tamBytes;          // continua depois da ocorrência
            pos += tamChars - 1;
        }
        else
        {
            i++;
        }
    }

    return qtdOcorrencias;
}

/*
 Q5 = inverte número
 @objetivo
    Inverter número inteiro
 @entrada
    uma int num.
 @saida
    Número invertido
 */

int q5(int num)
{
    int invertido = 0;

    // pega o último dígito e coloca no fim do número invertido
    // (zeros à direita somem: 5430 -> 345; o sinal é mantido)
    while (num != 0)
    {
        invertido = invertido * 10 + (num % 10);
        num = num / 10;
    }

    return invertido;
}

/*
 Q6 = ocorrência de um número em outro
 @objetivo
    Verificar quantidade de vezes da ocorrência de um número em outro
 @entrada
    Um número base (numerobase) e um número de busca (numerobusca).
 @saida
    Quantidade de vezes que número de busca ocorre em número base
 */

int q6(int numerobase, int numerobusca)
{
    int qtdOcorrencias = 0;
    long long potencia = 1;   // 10 elevado à quantidade de dígitos de numerobusca
    long long n = numerobase;
    int aux = numerobusca;

    if (numerobase < 0 || numerobusca < 0) // trabalha com números naturais
        return 0;

    do
    {
        potencia = potencia * 10;
        aux = aux / 10;
    } while (aux > 0);

    if (numerobase == 0)
        return (numerobusca == 0) ? 1 : 0;

    // olha os últimos dígitos de n; se casam, conta e descarta todos eles
    // (ocorrências não se sobrepõem: 44 em 54444 -> 2 vezes)
    while (n > 0)
    {
        if (n % potencia == numerobusca)
        {
            qtdOcorrencias++;
            n = n / potencia;
        }
        else
        {
            n = n / 10;
        }
    }

    return qtdOcorrencias;
}

/*
 Q7 = jogo busca palavras
 @objetivo
    Verificar se existe uma string em uma matriz de caracteres em todas as direções e sentidos possíves
 @entrada
    Uma matriz de caracteres e uma string de busca (palavra).
 @saida
    1 se achou 0 se não achou
 */

 int q7(char matriz[8][10], char palavra[5])
 {
     // 8 direções: cima-esq, cima, cima-dir, esq, dir, baixo-esq, baixo, baixo-dir
     int dl[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
     int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
     int l, c, d, k;

     for (l = 0; l < 8; l++)
     {
         for (c = 0; c < 10; c++)
         {
             for (d = 0; d < 8; d++)
             {
                 int ll = l, cc = c;

                 for (k = 0; k < 5; k++)
                 {
                     if (ll < 0 || ll >= 8 || cc < 0 || cc >= 10)
                         break;
                     if (matriz[ll][cc] != palavra[k])
                         break;
                     ll = ll + dl[d];
                     cc = cc + dc[d];
                 }

                 if (k == 5)
                     return 1;
             }
         }
     }

     return 0;
 }



/*
 quebraData: separa dia, mês e ano de uma string no formato d/m/aa, dd/mm/aaaa...
 Não usa funções de string e confere os limites da string.
 Ano com 2 dígitos é interpretado como 20xx (ex.: 15 -> 2015).
 valido = 0 se o formato estiver errado (não confere se o dia existe no mês).
 */
DataQuebrada quebraData(char data[]){
  DataQuebrada dq;
  int partes[3] = {0, 0, 0};   // dia, mês e ano
  int digitos[3] = {0, 0, 0};  // quantidade de dígitos de cada parte
  int parte = 0;
  int i;

  dq.iDia = 0;
  dq.iMes = 0;
  dq.iAno = 0;
  dq.valido = 0;

  for (i = 0; data[i] != '\0'; i++){
    if (data[i] == '/'){
      parte++;
      if (parte > 2)                 // barras demais
        return dq;
    }else if (data[i] >= '0' && data[i] <= '9'){
      digitos[parte]++;
      if (digitos[parte] > 4)        // parte grande demais
        return dq;
      partes[parte] = partes[parte] * 10 + (data[i] - '0');
    }else{
      return dq;                     // caractere que não é número nem barra
    }
  }

  if (parte != 2)                                    // precisa ter duas barras
    return dq;
  if (digitos[0] < 1 || digitos[0] > 2)              // dia com 1 ou 2 dígitos
    return dq;
  if (digitos[1] < 1 || digitos[1] > 2)              // mês com 1 ou 2 dígitos
    return dq;
  if (digitos[2] != 2 && digitos[2] != 4)            // ano com 2 ou 4 dígitos
    return dq;

  if (digitos[2] == 2)
    partes[2] = partes[2] + 2000;

  dq.iDia = partes[0];
  dq.iMes = partes[1];
  dq.iAno = partes[2];
  dq.valido = 1;

  return dq;
}

/* ---------- implementação das funções auxiliares ---------- */

static int ehBissexto(int ano)
{
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

static int diasNoMes(int mes, int ano)
{
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mes == 2 && ehBissexto(ano))
        return 29;
    return dias[mes - 1];
}

static int dataExiste(int dia, int mes, int ano)
{
    if (ano < 1 || mes < 1 || mes > 12 || dia < 1)
        return 0;
    return dia <= diasNoMes(mes, ano);
}

// retorna -1 se a data 1 é anterior, 0 se são iguais, 1 se a data 1 é posterior
static int comparaDatas(int d1, int m1, int a1, int d2, int m2, int a2)
{
    if (a1 != a2) return (a1 < a2) ? -1 : 1;
    if (m1 != m2) return (m1 < m2) ? -1 : 1;
    if (d1 != d2) return (d1 < d2) ? -1 : 1;
    return 0;
}

// quantidade de dias desde o ano 1 até a data (serve para subtrair duas datas)
static long numeroDoDia(int dia, int mes, int ano)
{
    int m;
    long total = 365L * (ano - 1) + (ano - 1) / 4 - (ano - 1) / 100 + (ano - 1) / 400;

    for (m = 1; m < mes; m++)
        total = total + diasNoMes(m, ano);

    return total + dia;
}

// soma "meses" à data base. Se o dia não existir no mês de destino, usa o último dia do mês.
static void somaMeses(DataQuebrada base, int meses, int *dia, int *mes, int *ano)
{
    int total = base.iAno * 12 + (base.iMes - 1) + meses;

    *ano = total / 12;
    *mes = total % 12 + 1;
    *dia = base.iDia;

    if (*dia > diasNoMes(*mes, *ano))
        *dia = diasNoMes(*mes, *ano);
}

// retorna 1 se o byte é o 2º byte de um caractere UTF-8 (não começa um caractere novo)
static int ehContinuacao(char c)
{
    return (((unsigned char) c) & 0xC0) == 0x80;
}

// lê o próximo caractere de s a partir de *i e avança *i.
// Aceita UTF-8 (á = 0xC3 0xA1) e Latin-1 (á = 0xE1). Retorna o código Latin-1.
static int proximoCaractere(char *s, int *i)
{
    unsigned char b = (unsigned char) s[*i];
    unsigned char b2 = (unsigned char) s[*i + 1];

    if (b == 0xC3 && b2 >= 0x80 && b2 <= 0xBF)
    {
        *i = *i + 2;
        return 0xC0 + (b2 - 0x80);
    }

    *i = *i + 1;
    return b;
}

// tira o acento das vogais (á, à, â, ã, ä ... viram a), mantendo maiúscula/minúscula
static int semAcento(int c)
{
    if (c >= 0xC0 && c <= 0xC5) return 'A';
    if (c >= 0xC8 && c <= 0xCB) return 'E';
    if (c >= 0xCC && c <= 0xCF) return 'I';
    if (c >= 0xD2 && c <= 0xD6) return 'O';
    if (c >= 0xD9 && c <= 0xDC) return 'U';
    if (c >= 0xE0 && c <= 0xE5) return 'a';
    if (c >= 0xE8 && c <= 0xEB) return 'e';
    if (c >= 0xEC && c <= 0xEF) return 'i';
    if (c >= 0xF2 && c <= 0xF6) return 'o';
    if (c >= 0xF9 && c <= 0xFC) return 'u';
    return c;
}

static int minuscula(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}
