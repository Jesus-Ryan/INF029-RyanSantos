#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Escola.h"
#include "Professor.h"
#include "Disciplina.h"

int inserirProfessor(Professor** inicio);
int excluirProfessor(Professor** inicio, Disciplina* inicioDisciplina);
int atualizarProfessor(Professor** inicio);
void listarProfessores(Professor** inicio);
int menuProfessor();

int geraMatriculaProfessor()
{
	static int num = 0;
	num++;
	return num;
}

int menuProfessor(){

	int opcao;

	printf("#### Módulo de Professor ####\n");
	printf("#### Digite a opção: ####\n");
	printf("0 - Voltar para o menu geral\n");
	printf("1 - Inserir Professor\n");
	printf("2 - Excluir Professor\n");
	printf("3 - Listar Professores\n");
	printf("4 - Atualizar Professor\n");
	scanf("%d",&opcao);

	return opcao;
}

void mainProfessor(Professor** inicioListaProfessor, Disciplina* inicioDisciplina){
	int opcao, retorno;
	int sair = 0;

	while (!sair){

		opcao = menuProfessor();

		switch(opcao){
			case 0:{
				sair = 1;
				break;
			}
			case 1: {
				retorno = inserirProfessor(inicioListaProfessor);

				if (retorno == SUCESSO_CADASTRO){
					printf("Professor cadastrado com sucesso\n");
				}else{
					switch(retorno){
						case ERRO_CADASTRO_SEXO:{
							printf("Sexo Inválido. Digite 'm' ou 'M' para Masculino ou 'f' ou 'F' para Feminino.\n");
							break;
						}
						case ERRO_DATA_INVALIDA:{
							printf("Data Inválida.\n");
							break;
						}
						case ERRO_CADASTRO_CPF:{
							printf("CPF Inválido.\n");
							break;
						}default:{
							printf("Erro desconhecido.\n");
						}
					}
				}
				break;
			}
			case 2: {
				retorno = excluirProfessor(inicioListaProfessor, inicioDisciplina);
				if (retorno == SUCESSO_EXCLUSAO){
					printf("Professor excluído com sucesso\n");
				}else{
					switch(retorno){
						case LISTA_VAZIA:{
							printf("Lista Vazia.\n");
							break;
						}
						case NAO_ENCONTRADO:{
							printf("Não foi encontrado o professor com a matrícula digitada.\n");
							break;
						}
						case ERRO_PROFESSOR_COM_DISCIPLINA:{
							printf("Não é possível excluir: o professor ainda leciona ao menos uma disciplina.\n");
							printf("Troque o professor responsável dessa(s) disciplina(s) ou exclua-a(s) antes.\n");
							break;
						}
						default:{
							printf("Erro desconhecido.\n");
						}
					}
				}
				break;
			}
			case 3: {
				listarProfessores(inicioListaProfessor);
				break;
			}
			case 4: {
				retorno = atualizarProfessor(inicioListaProfessor);
				if (retorno == SUCESSO_ATUALIZACAO){
					printf("Professor atualizado com sucesso\n");
				}else if (retorno == NAO_ENCONTRADO){
					printf("Não foi encontrado o professor com a matrícula digitada.\n");
				}
				break;
			}default:{
				printf("opcao inválida\n");
			}
		}
	}
}

void inserirProfessorNaLista(Professor** inicio, Professor* novoProfessor){
	Professor *atual;

	if (*inicio == NULL)
		*inicio = novoProfessor;
	else{
		atual = *inicio;

		while(atual->prox != NULL)
			atual = atual->prox;

		atual->prox = novoProfessor;
	}

	novoProfessor->prox = NULL;
}

int inserirProfessor(Professor** inicio){
	int retorno = SUCESSO_CADASTRO;

	Professor* novoProfessor = (Professor *)malloc(sizeof(Professor));

	printf("\n### Cadastro de Professor ###\n");
	getchar();

	printf("Digite o nome: ");
	fgets(novoProfessor->nome, 50, stdin);
	remover_quebra_linha(novoProfessor->nome);

	printf("Digite o sexo: ");
	scanf("%c", &novoProfessor->sexo);

	novoProfessor->sexo = toupper(novoProfessor->sexo);
	if (novoProfessor->sexo != 'M' && novoProfessor->sexo != 'F'){
		retorno = ERRO_CADASTRO_SEXO;
	}else{
		printf("Digite a data de nascimento (dd/mm/aaaa): ");
		scanf("%10s", novoProfessor->data_nascimento.dataCompleta);
		getchar();

		int dataValida = validar_data(novoProfessor->data_nascimento.dataCompleta);
		if (dataValida == 0){
			retorno = ERRO_DATA_INVALIDA;
		}else{
			preencher_campos_data(&novoProfessor->data_nascimento);

			printf("Digite o CPF: ");
			fgets(novoProfessor->cpf, 15, stdin);
			remover_quebra_linha(novoProfessor->cpf);

			if (!validar_cpf(novoProfessor->cpf)){
				retorno = ERRO_CADASTRO_CPF;
			}
		}
	}

	if (retorno == SUCESSO_CADASTRO){
		novoProfessor->matricula = geraMatriculaProfessor();
		inserirProfessorNaLista(inicio, novoProfessor);
		return SUCESSO_CADASTRO;
	}else{
		free(novoProfessor);
		return retorno;
	}
}

int excluirProfessorNaLista(Professor** inicio, int matricula){
	if (*inicio == NULL)
		return LISTA_VAZIA;

	Professor* anterior = *inicio;
	Professor* atual = *inicio;
	Professor* proximo = atual->prox;
	int achou = 0;

	while(atual != NULL){
		if (atual->matricula == matricula){
			achou = 1;
			break;
		}
		anterior = atual;
		atual = proximo;
		if (atual != NULL)
			proximo = atual->prox;
	}

	if (achou){
		if (atual == *inicio)
			*inicio = proximo;
		else
			anterior->prox = atual->prox;
		free(atual);
		return SUCESSO_EXCLUSAO;
	}else
		return NAO_ENCONTRADO;
}

int excluirProfessor(Professor** inicio, Disciplina* inicioDisciplina){
	int matricula;

	printf("Digite a matrícula: ");
	scanf("%d", &matricula);
	getchar();

	/* Nao deixa excluir um professor que ainda leciona alguma disciplina,
	   para nao deixar uma disciplina "orfa" (sem professor valido). */
	if (existeDisciplinaComProfessor(inicioDisciplina, matricula))
		return ERRO_PROFESSOR_COM_DISCIPLINA;

	return excluirProfessorNaLista(inicio, matricula);
}

int atualizarProfessor(Professor** inicio){
	int matricula;
	Professor* professor;
	int opcao;
	int sair = 0;

	printf("Digite a matrícula do professor a atualizar: ");
	scanf("%d", &matricula);
	getchar();

	professor = buscarProfessorPorMatricula(*inicio, matricula);
	if (professor == NULL)
		return NAO_ENCONTRADO;

	while (!sair){
		printf("\n### Atualizar Professor (matrícula %d) ###\n", matricula);
		printf("Atual -> Nome: %s | Sexo: %c | Nascimento: %s | CPF: %s\n",
		       professor->nome, professor->sexo, professor->data_nascimento.dataCompleta, professor->cpf);
		printf("1 - Alterar nome\n");
		printf("2 - Alterar sexo\n");
		printf("3 - Alterar data de nascimento\n");
		printf("4 - Alterar CPF\n");
		printf("0 - Concluir\n");
		scanf("%d", &opcao);
		getchar();

		switch (opcao){
			case 1: {
				printf("Novo nome: ");
				fgets(professor->nome, 50, stdin);
				remover_quebra_linha(professor->nome);
				break;
			}
			case 2: {
				char sexo;
				printf("Novo sexo (M/F): ");
				scanf("%c", &sexo);
				getchar();
				sexo = toupper(sexo);
				if (sexo == 'M' || sexo == 'F')
					professor->sexo = sexo;
				else
					printf("Sexo inválido. Mantido o anterior.\n");
				break;
			}
			case 3: {
				char dataStr[11];
				printf("Nova data de nascimento (dd/mm/aaaa): ");
				scanf("%10s", dataStr);
				getchar();
				if (validar_data(dataStr)){
					strcpy(professor->data_nascimento.dataCompleta, dataStr);
					preencher_campos_data(&professor->data_nascimento);
				}else{
					printf("Data inválida. Mantida a anterior.\n");
				}
				break;
			}
			case 4: {
				char cpf[15];
				printf("Novo CPF: ");
				fgets(cpf, 15, stdin);
				remover_quebra_linha(cpf);
				if (validar_cpf(cpf))
					strcpy(professor->cpf, cpf);
				else
					printf("CPF inválido. Mantido o anterior.\n");
				break;
			}
			case 0:
				sair = 1;
				break;
			default:
				printf("opção inválida\n");
		}
	}

	return SUCESSO_ATUALIZACAO;
}

Professor* buscarProfessorPorMatricula(Professor* inicio, int matricula){
	Professor* atual = inicio;
	while (atual != NULL){
		if (atual->matricula == matricula)
			return atual;
		atual = atual->prox;
	}
	return NULL;
}

void listarProfessores(Professor** inicio){
	Professor* profAtual = *inicio;
	if (*inicio == NULL){
		printf("Lista Vazia\n");
	}else{
		printf("\n### Professores Cadastrados ####\n");
		do{
			printf("-----\n");
			printf("Matrícula: %d\n", profAtual->matricula);
			printf("Nome: %s\n", profAtual->nome);
			printf("Sexo: %c\n", profAtual->sexo);
			printf("Data Nascimento: %s\n", profAtual->data_nascimento.dataCompleta);
			printf("CPF: %s\n", profAtual->cpf);

			profAtual = profAtual->prox;

		}while (profAtual != NULL);
	}
	printf("-----\n\n");
}

void liberarListaProfessor(Professor* inicio){
	Professor* atual = inicio;
	Professor* tmp;

	while(atual != NULL){
		tmp = atual->prox;
		free(atual);
		atual = tmp;
	}
}
