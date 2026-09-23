#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Utils.h"

int verifica_numero(char CARACTER);
int verifica_ano_bisexto(int iAno);
int valida_numeros(int iDia, int iMes, int iAno);

int validar_data(char *data)
{
    int iDia;
    int iMes;
    int iAno;
    int retorno;
    char sDia[3];
    char sMes[3];
    char sAno[5];
    int iCont;
    int jCont;
    int verificacao;
    
    //CONVERTE DIA******************************************************************************
    for (iCont = 0; data[iCont] != '/'; iCont++)
    {
        if (data[iCont] == '\0')
            return 0;
        if(iCont >= 3)
            return 0;
        sDia[iCont] = data[iCont];
    }

    sDia[iCont] = '\0';
    
    for (jCont = 0; sDia[jCont] != '\0'; jCont++)
    {
        verificacao = verifica_numero(sDia[jCont]);
        if(verificacao == 0)
            return 0;
    }
    
    if (strlen(sDia) == 0)
        return 0;
 
    iDia = atoi(sDia); 
 
    // CONVERTE MES*****************************************************************************
    for (iCont = iCont+1, jCont = 0; data[iCont] != '/'; iCont++, jCont++)
    {
        if (data[iCont] == '\0')
            return 0;
        if(jCont >= 3)
            return 0;
        sMes[jCont] = data[iCont];
    }
    
    sMes[jCont] = '\0';
    
    for (jCont = 0; sMes[jCont] != '\0'; jCont++){
        verificacao= verifica_numero(sMes[jCont]);
            if(verificacao == 0)
                return 0;
    }
    
    if (strlen(sMes) == 0)
        return 0;
    
    iMes = atoi(sMes); 
  
    // CONVERTE ANO****************************************************************************
    
    for (iCont = iCont+1, jCont=0; data[iCont] != '\0' ; iCont++, jCont++)
    {
        /* BUGFIX: o codigo original nao limitava o tamanho do ano,
           o que podia estourar o buffer sAno[5]. */
        if(jCont >= 4)
            return 0;
        sAno[jCont] = data[iCont];
    }
    sAno[jCont] = '\0';
    
    for (jCont = 0; sAno[jCont] != '\0'; jCont++)
    {
        verificacao= verifica_numero(sAno[jCont]);
        if(verificacao == 0)
            return 0;
    }
        
    if (strlen(sAno) == 0)
        return 0;
    
    iAno = atoi(sAno); 
 
    // *****************************************************************************************
    retorno= valida_numeros(iDia, iMes, iAno);
    
    return retorno;
}

int valida_numeros(int iDia, int iMes, int iAno)
{
    
    int bissexto = verifica_ano_bisexto(iAno);
    
    if (iDia < 1 || iDia > 31)
        return 0;
    
    else if (iDia >= 31 && (iMes == 4 || iMes == 6 || iMes == 9 || iMes == 11 ))
        return 0;
    
    else if (iMes < 1 || iMes > 12)
        return 0;
    
    else if(iDia> 29 && iMes == 2 && bissexto == 1)
        return 0;
    
    else if(iDia> 28 && iMes == 2 && bissexto == 0)
        return 0;
    
    else if (iAno <= 0 )
        return 0;
      
    else if (iAno >= 10000)
        return 0;
    
  return 1;  
}

int verifica_ano_bisexto(int iAno)
{
    /* BUGFIX: a regra correta de ano bissexto eh "divisivel por 4 E
       (nao divisivel por 100 OU divisivel por 400)". O codigo original
       exigia tambem "nao divisivel por 400", o que fazia anos como
       2000 (que SAO bissextos) serem classificados como nao-bissextos. */
    if (((iAno % 4) == 0 && (iAno % 100) != 0) || ((iAno % 400) == 0))
        return 1;
    return 0;
}

int verifica_numero(char CARACTER)
{
    /* BUGFIX: o codigo original usava "&&" em vez de "||". Como um
       caractere nunca e, ao mesmo tempo, menor que '0' E maior que '9',
       a condicao original nunca era verdadeira e a funcao sempre
       retornava 1 (valido), mesmo para letras e simbolos. */
    if (CARACTER < '0' || CARACTER > '9')
        return 0;
    
    return 1;
}

void preencher_campos_data(Data *data)
{
    /* So deve ser chamada apos validar_data() retornar 1, entao aqui
       o formato dd/mm/aaaa ja esta garantido. */
    sscanf(data->dataCompleta, "%d/%d/%d", &data->dia, &data->mes, &data->ano);
}

int validar_cpf(char cpf[])
{
    int digitos[11];
    int n = 0;
    int i;
    int igual;
    int soma, resto, dv1, dv2;

    for (i = 0; cpf[i] != '\0'; i++)
    {
        if (isdigit((unsigned char) cpf[i]))
        {
            if (n >= 11)
                return 0; /* mais de 11 digitos numericos */
            digitos[n] = cpf[i] - '0';
            n++;
        }
    }
    if (n != 11)
        return 0;

    /* CPFs com todos os digitos iguais (ex: 111.111.111-11) passariam
       no calculo do digito verificador, mas nao existem na pratica. */
    igual = 1;
    for (i = 1; i < 11; i++)
    {
        if (digitos[i] != digitos[0])
        {
            igual = 0;
            break;
        }
    }
    if (igual)
        return 0;

    soma = 0;
    for (i = 0; i < 9; i++)
        soma += digitos[i] * (10 - i);
    resto = soma % 11;
    dv1 = (resto < 2) ? 0 : 11 - resto;
    if (dv1 != digitos[9])
        return 0;

    soma = 0;
    for (i = 0; i < 10; i++)
        soma += digitos[i] * (11 - i);
    resto = soma % 11;
    dv2 = (resto < 2) ? 0 : 11 - resto;
    if (dv2 != digitos[10])
        return 0;

    return 1;
}

void remover_quebra_linha(char str[])
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}
