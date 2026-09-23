#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Escola.h"
#include "Disciplina.h"
#include "Aluno.h"
#include "Professor.h"

int incluirDisciplina(Disciplina** inicio, Professor* inicioProfessor);
int excluirDisciplina(Disciplina** inicio, char codigo[]);
int atualizarDisciplina(Disciplina** inicio, Professor* inicioProfessor);
int matricularAluno(Disciplina* inicioDisciplina, Aluno* inicioAluno);
int desmatricularAluno(Disciplina* inicioDisciplina);
void listarDisciplinas(Disciplina** inicio, Professor* inicioProfessor);
int menuDisciplina();

void liberarListaMatricula(Matricula* inicio){
	Matricula* atual = inicio;
	Matricula* tmp;

	while(atual != NULL){
		tmp = atual->prox;
		free(atual);
		atual = tmp;
	}
}

int menuDisciplina(){
	int opcao;

	printf("#### Módulo de Disciplina ####\n");
	printf("#### Digite a opção: ####\n");
	printf("0 - Voltar para o menu geral\n");
	printf("1 - Incluir Disciplina\n");
	printf("2 - Excluir Disciplina\n");
	printf("3 - Atualizar Disciplina\n");
	printf("4 - Listar Disciplinas\n");
	printf("5 - Matricular aluno em disciplina\n");
	printf("6 - Remover aluno de disciplina\n");
	scanf("%d", &opcao);

	return opcao;
}

void mainDisciplina(Disciplina** inicioListaDisciplina, Aluno* inicioAluno, Professor* inicioProfessor){
	int opcao, retorno;
	int sair = 0;
	char codigo[10];

	while (!sair){

		opcao = menuDisciplina();

		switch(opcao){
			case 0:{
				sair = 1;
				break;
			}
			case 1: {
				retorno = incluirDisciplina(inicioListaDisciplina, inicioProfessor);
				if (retorno == SUCESSO_CADASTRO){
					printf("Disciplina cadastrada com sucesso\n");
				}else if (retorno == ERRO_DISCIPLINA_JA_EXISTE){
					printf("Já existe uma disciplina com esse código.\n");
				}else if (retorno == ERRO_PROFESSOR_INEXISTENTE){
					printf("Professor não encontrado. Cadastre o professor antes.\n");
				}else{
					printf("Erro desconhecido.\n");
				}
				break;
			}
			case 2: {
				printf("Digite o código da disciplina: ");
				scanf("%9s", codigo);
				getchar();
				retorno = excluirDisciplina(inicioListaDisciplina, codigo);
				if (retorno == SUCESSO_EXCLUSAO){
					printf("Disciplina excluída com sucesso\n");
				}else if (retorno == LISTA_VAZIA){
					printf("Lista Vazia.\n");
				}else{
					printf("Não foi encontrada disciplina com esse código.\n");
				}
				break;
			}
			case 3: {
				retorno = atualizarDisciplina(inicioListaDisciplina, inicioProfessor);
				if (retorno == SUCESSO_ATUALIZACAO){
					printf("Disciplina atualizada com sucesso\n");
				}else if (retorno == NAO_ENCONTRADO){
					printf("Não foi encontrada disciplina com esse código.\n");
				}
				break;
			}
			case 4: {
				listarDisciplinas(inicioListaDisciplina, inicioProfessor);
				break;
			}
			case 5: {
				retorno = matricularAluno(*inicioListaDisciplina, inicioAluno);
				if (retorno == SUCESSO_MATRICULA){
					printf("Aluno matriculado com sucesso\n");
				}else if (retorno == ERRO_DISCIPLINA_INEXISTENTE){
					printf("Disciplina não encontrada.\n");
				}else if (retorno == ERRO_ALUNO_INEXISTENTE){
					printf("Aluno não encontrado.\n");
				}else if (retorno == ERRO_ALUNO_JA_MATRICULADO){
					printf("Esse aluno já está matriculado nessa disciplina.\n");
				}
				break;
			}
			case 6: {
				retorno = desmatricularAluno(*inicioListaDisciplina);
				if (retorno == SUCESSO_EXCLUSAO){
					printf("Aluno removido da disciplina com sucesso\n");
				}else if (retorno == ERRO_DISCIPLINA_INEXISTENTE){
					printf("Disciplina não encontrada.\n");
				}else if (retorno == ERRO_ALUNO_NAO_MATRICULADO){
					printf("Esse aluno não está matriculado nessa disciplina.\n");
				}
				break;
			}default:{
				printf("opcao inválida\n");
			}
		}
	}
}

void inserirDisciplinaNaLista(Disciplina** inicio, Disciplina* novaDisciplina){
	Disciplina *atual;

	if (*inicio == NULL)
		*inicio = novaDisciplina;
	else{
		atual = *inicio;

		while(atual->prox != NULL)
			atual = atual->prox;

		atual->prox = novaDisciplina;
	}

	novaDisciplina->prox = NULL;
}

int incluirDisciplina(Disciplina** inicio, Professor* inicioProfessor){
	int retorno = SUCESSO_CADASTRO;
	int matriculaProfessor;

	Disciplina* novaDisciplina = (Disciplina *)malloc(sizeof(Disciplina));

	printf("\n### Cadastro de Disciplina ###\n");
	getchar();

	printf("Digite o código da disciplina: ");
	scanf("%9s", novaDisciplina->codigo);
	getchar();

	if (buscarDisciplinaPorCodigo(*inicio, novaDisciplina->codigo) != NULL){
		retorno = ERRO_DISCIPLINA_JA_EXISTE;
	}else{
		printf("Digite o nome da disciplina: ");
		fgets(novaDisciplina->nome, 50, stdin);
		remover_quebra_linha(novaDisciplina->nome);

		printf("Digite o semestre (ex: 1 para 2026.1): ");
		scanf("%d", &novaDisciplina->semestre);
		getchar();

		printf("Digite a matrícula do professor responsável: ");
		scanf("%d", &matriculaProfessor);
		getchar();

		if (buscarProfessorPorMatricula(inicioProfessor, matriculaProfessor) == NULL){
			retorno = ERRO_PROFESSOR_INEXISTENTE;
		}else{
			novaDisciplina->matriculaProfessor = matriculaProfessor;
		}
	}

	if (retorno == SUCESSO_CADASTRO){
		novaDisciplina->alunosMatriculados = NULL;
		novaDisciplina->totalAlunosMatriculados = 0;
		inserirDisciplinaNaLista(inicio, novaDisciplina);
		return SUCESSO_CADASTRO;
	}else{
		free(novaDisciplina);
		return retorno;
	}
}

int excluirDisciplina(Disciplina** inicio, char codigo[]){
	if (*inicio == NULL)
		return LISTA_VAZIA;

	Disciplina* anterior = *inicio;
	Disciplina* atual = *inicio;
	Disciplina* proximo = atual->prox;
	int achou = 0;

	while(atual != NULL){
		if (strcmp(atual->codigo, codigo) == 0){
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
		liberarListaMatricula(atual->alunosMatriculados);
		free(atual);
		return SUCESSO_EXCLUSAO;
	}else
		return NAO_ENCONTRADO;
}

int atualizarDisciplina(Disciplina** inicio, Professor* inicioProfessor){
	char codigo[10];
	Disciplina* disc;
	int opcao;
	int sair = 0;

	printf("Digite o código da disciplina a atualizar: ");
	scanf("%9s", codigo);
	getchar();

	disc = buscarDisciplinaPorCodigo(*inicio, codigo);
	if (disc == NULL)
		return NAO_ENCONTRADO;

	while (!sair){
		printf("\n### Atualizar Disciplina (código %s) ###\n", codigo);
		printf("Atual -> Nome: %s | Semestre: %d | Matrícula Professor: %d\n",
		       disc->nome, disc->semestre, disc->matriculaProfessor);
		printf("1 - Alterar nome\n");
		printf("2 - Alterar semestre\n");
		printf("3 - Alterar professor responsável\n");
		printf("0 - Concluir\n");
		scanf("%d", &opcao);
		getchar();

		switch (opcao){
			case 1: {
				printf("Novo nome: ");
				fgets(disc->nome, 50, stdin);
				remover_quebra_linha(disc->nome);
				break;
			}
			case 2: {
				printf("Novo semestre: ");
				scanf("%d", &disc->semestre);
				getchar();
				break;
			}
			case 3: {
				int matProf;
				printf("Nova matrícula do professor: ");
				scanf("%d", &matProf);
				getchar();
				if (buscarProfessorPorMatricula(inicioProfessor, matProf) == NULL)
					printf("Professor não encontrado. Mantido o anterior.\n");
				else
					disc->matriculaProfessor = matProf;
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

int matricularAluno(Disciplina* inicioDisciplina, Aluno* inicioAluno){
	char codigo[10];
	int matriculaAluno;
	Disciplina* disc;
	Matricula* atual;
	Matricula* novaMatricula;

	printf("Digite o código da disciplina: ");
	scanf("%9s", codigo);
	getchar();

	disc = buscarDisciplinaPorCodigo(inicioDisciplina, codigo);
	if (disc == NULL)
		return ERRO_DISCIPLINA_INEXISTENTE;

	printf("Digite a matrícula do aluno: ");
	scanf("%d", &matriculaAluno);
	getchar();

	if (buscarAlunoPorMatricula(inicioAluno, matriculaAluno) == NULL)
		return ERRO_ALUNO_INEXISTENTE;

	atual = disc->alunosMatriculados;
	while (atual != NULL){
		if (atual->matriculaAluno == matriculaAluno)
			return ERRO_ALUNO_JA_MATRICULADO;
		atual = atual->prox;
	}

	novaMatricula = (Matricula *)malloc(sizeof(Matricula));
	novaMatricula->matriculaAluno = matriculaAluno;
	novaMatricula->prox = NULL;

	if (disc->alunosMatriculados == NULL){
		disc->alunosMatriculados = novaMatricula;
	}else{
		atual = disc->alunosMatriculados;
		while (atual->prox != NULL)
			atual = atual->prox;
		atual->prox = novaMatricula;
	}
	disc->totalAlunosMatriculados++;

	return SUCESSO_MATRICULA;
}

int desmatricularAluno(Disciplina* inicioDisciplina){
	char codigo[10];
	int matriculaAluno;
	Disciplina* disc;
	Matricula* anterior;
	Matricula* atual;
	int achou = 0;

	printf("Digite o código da disciplina: ");
	scanf("%9s", codigo);
	getchar();

	disc = buscarDisciplinaPorCodigo(inicioDisciplina, codigo);
	if (disc == NULL)
		return ERRO_DISCIPLINA_INEXISTENTE;

	printf("Digite a matrícula do aluno a remover: ");
	scanf("%d", &matriculaAluno);
	getchar();

	anterior = disc->alunosMatriculados;
	atual = disc->alunosMatriculados;

	while (atual != NULL){
		if (atual->matriculaAluno == matriculaAluno){
			achou = 1;
			break;
		}
		anterior = atual;
		atual = atual->prox;
	}

	if (!achou)
		return ERRO_ALUNO_NAO_MATRICULADO;

	if (atual == disc->alunosMatriculados)
		disc->alunosMatriculados = atual->prox;
	else
		anterior->prox = atual->prox;

	free(atual);
	disc->totalAlunosMatriculados--;

	return SUCESSO_EXCLUSAO;
}

/* Usada pelo modulo de Aluno.c ao excluir um aluno definitivamente. */
void removerAlunoDeTodasDisciplinas(Disciplina* inicio, int matriculaAluno){
	Disciplina* disc = inicio;

	while (disc != NULL){
		Matricula* anterior = disc->alunosMatriculados;
		Matricula* atual = disc->alunosMatriculados;
		int achou = 0;

		while (atual != NULL){
			if (atual->matriculaAluno == matriculaAluno){
				achou = 1;
				break;
			}
			anterior = atual;
			atual = atual->prox;
		}

		if (achou){
			if (atual == disc->alunosMatriculados)
				disc->alunosMatriculados = atual->prox;
			else
				anterior->prox = atual->prox;
			free(atual);
			disc->totalAlunosMatriculados--;
		}

		disc = disc->prox;
	}
}

Disciplina* buscarDisciplinaPorCodigo(Disciplina* inicio, char codigo[]){
	Disciplina* atual = inicio;
	while (atual != NULL){
		if (strcmp(atual->codigo, codigo) == 0)
			return atual;
		atual = atual->prox;
	}
	return NULL;
}

int existeDisciplinaComProfessor(Disciplina* inicio, int matriculaProfessor){
	Disciplina* atual = inicio;
	while (atual != NULL){
		if (atual->matriculaProfessor == matriculaProfessor)
			return 1;
		atual = atual->prox;
	}
	return 0;
}

void listarDisciplinas(Disciplina** inicio, Professor* inicioProfessor){
	Disciplina* atual = *inicio;

	if (*inicio == NULL){
		printf("Lista Vazia\n");
	}else{
		printf("\n### Disciplinas Cadastradas ####\n");
		do{
			Professor* prof = buscarProfessorPorMatricula(inicioProfessor, atual->matriculaProfessor);
			printf("-----\n");
			printf("Código: %s\n", atual->codigo);
			printf("Nome: %s\n", atual->nome);
			printf("Semestre: %d\n", atual->semestre);
			printf("Professor: %s\n", (prof != NULL) ? prof->nome : "(não encontrado)");
			printf("Alunos matriculados: %d\n", atual->totalAlunosMatriculados);

			atual = atual->prox;
		}while (atual != NULL);
	}
	printf("-----\n\n");
}

void liberarListaDisciplina(Disciplina* inicio){
	Disciplina* atual = inicio;
	Disciplina* tmp;

	while(atual != NULL){
		tmp = atual->prox;
		liberarListaMatricula(atual->alunosMatriculados);
		free(atual);
		atual = tmp;
	}
}
