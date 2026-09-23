/**
Projeto Escola: Sistema de controle de uma escola.

Desenvolvimento em C

Realizado no contexto da disciplina INF029 - Laboratório de Programação
do curso de Análise e Desenvolvimento de Sistemas do Instituto Federal da Bahia, campus Salvador
http://ads.ifba.edu.br

Prof. Renato Novais

@2020

*/

#include <stdio.h>

#include "Escola.h"
#include "Aluno.h"
#include "Professor.h"
#include "Disciplina.h"

int menuGeral(){

	int opcao;

	printf("\n#### Digite a opção: ####\n");
	printf("0 - Sair\n");
	printf("1 - Gerenciar Aluno\n");
	printf("2 - Gerenciar Professor\n");
	printf("3 - Gerenciar Disciplina\n");

	scanf("%d",&opcao);

	return opcao;

}

//v4
void finalizarEscola(Aluno* inicioAluno, Professor* inicioProfessor, Disciplina* inicioDisciplina){
	liberarListaAluno(inicioAluno);
	liberarListaProfessor(inicioProfessor);
	liberarListaDisciplina(inicioDisciplina);
}

int main(){
	Aluno* inicioListaAluno = NULL;           // inicio da lista de alunos
	Professor* inicioListaProfessor = NULL;   // inicio da lista de professores
	Disciplina* inicioListaDisciplina = NULL; // inicio da lista de disciplinas

	int opcao;
	int sair = 0;

	while (!sair){
    
	    opcao = menuGeral();
	    
	    switch(opcao){
	      case 0:{
	        printf("Finalizando Escola\n");
	        finalizarEscola(inicioListaAluno, inicioListaProfessor, inicioListaDisciplina); //v4
	        sair = 1;
	        break;
	      }
	      case 1: {
	      	mainAluno(&inicioListaAluno, inicioListaDisciplina);
	      	break;
	      }
	      case 2: {
	      	mainProfessor(&inicioListaProfessor, inicioListaDisciplina);
	      	break;
	      }
	      case 3: {
	      	mainDisciplina(&inicioListaDisciplina, inicioListaAluno, inicioListaProfessor);
	      	break;
	      }default:{
	      	printf("opcao inválida\n");
	      }
	  	}
	}

	return 0;
}
