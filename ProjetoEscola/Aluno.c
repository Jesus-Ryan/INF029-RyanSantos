#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Escola.h"
#include "Aluno.h"
#include "Disciplina.h"

int inserirAluno(Aluno** inicio);
int excluirAluno(Aluno** inicio, Disciplina* inicioDisciplina);
int atualizarAluno(Aluno** inicio);
void listarAlunos(Aluno** inicio);
int menuAluno();

//v3
int geraMatricula()
{
	static int num = 0;
	num++;
	return num;
}

int menuAluno(){

	int opcao;

	printf("#### Módulo de Aluno ####\n");
	printf("#### Digite a opção: ####\n");
	printf("0 - Voltar para o menu geral\n");
	printf("1 - Inserir Aluno\n");
	printf("2 - Excluir Aluno\n");
	printf("3 - Listar Alunos\n");
	printf("4 - Atualizar Aluno\n");
	scanf("%d",&opcao);

	return opcao;

}


void mainAluno(Aluno** inicioListaAluno, Disciplina* inicioDisciplina){
	int opcao, retorno;
	int sair = 0;

	while (!sair){
    
	    opcao = menuAluno();
	    
	    switch(opcao){
	      case 0:{
	        sair = 1;
	        break;
	      }
	      case 1: {
	      	retorno = inserirAluno(inicioListaAluno);

	      	//v2 - adicao do controle de retorno da funcao
	      	if (retorno == SUCESSO_CADASTRO){ 
	      		printf("Aluno cadastrado com sucesso\n");
	      	}else{
	      		switch(retorno){
	      			case ERRO_CADASTRO_MATRICULA:{
	      				printf("Matrícula Inválida. Deve ser maior que 0.\n");
	      				break;
	      			}
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
	      	retorno = excluirAluno(inicioListaAluno, inicioDisciplina);
	      	//v2 - adicao do controle de retorno da funcao
	      	if (retorno == SUCESSO_EXCLUSAO){ 
	      		printf("Aluno excluido com sucesso\n");
	      	}else{
	      		switch(retorno){
	      			case LISTA_VAZIA:{
	      				printf("Lista Vazia.\n");
	      				break;
	      			}
	      			case NAO_ENCONTRADO:{
	      				printf("Não foi encontrado o aluno com a matrícula digitada.\n");
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
	      	listarAlunos(inicioListaAluno);
	      	break;	
	      }
	      case 4: {
	      	retorno = atualizarAluno(inicioListaAluno);
	      	if (retorno == SUCESSO_ATUALIZACAO){
	      		printf("Aluno atualizado com sucesso\n");
	      	}else if (retorno == NAO_ENCONTRADO){
	      		printf("Não foi encontrado o aluno com a matrícula digitada.\n");
	      	}
	      	break;
	      }default:{
	      	printf("opcao inválida\n");
	      }
	  	}
	}


}

void inserirAlunoNaLista(Aluno** inicio, Aluno* novoAluno){
    Aluno *atual;
    
    if (*inicio == NULL)
        *inicio = novoAluno;
    else{
        atual = *inicio;

        while(atual->prox != NULL)
            atual = atual->prox;
        
        atual->prox = novoAluno;
    }
    
    novoAluno->prox = NULL;
}

int inserirAluno(Aluno** inicio){
    int retorno = SUCESSO_CADASTRO;

    //criar o aluno
    Aluno* novoAluno = (Aluno *)malloc(sizeof(Aluno));
    
    printf("\n### Cadastro de Aluno ###\n");
    getchar();

	printf("Digite o nome: ");
    fgets(novoAluno->nome, 50, stdin); 
    /*o fgets é uma das funções mais indicadas para ler string do teclado. 
    Ela controla o tamanho do buffer, e não deixa o buffer com lixo. 
    Entretanto ela guarda o \n ao final da string, por isso é preciso removê-lo, como feito a seguir*/
    remover_quebra_linha(novoAluno->nome);
    
    printf("Digite o sexo: ");
    scanf("%c", &novoAluno->sexo);
    
    novoAluno->sexo = toupper(novoAluno->sexo);
    if (novoAluno->sexo != 'M' && novoAluno->sexo != 'F') {
        retorno = ERRO_CADASTRO_SEXO;
    }else{
	    printf("Digite a data de nascimento (dd/mm/aaaa): ");
	    scanf("%10s", novoAluno->data_nascimento.dataCompleta);
	    getchar();

	    int dataValida = validar_data(novoAluno->data_nascimento.dataCompleta);
	    if (dataValida == 0){
	        retorno = ERRO_DATA_INVALIDA;
	    }else{
		    preencher_campos_data(&novoAluno->data_nascimento);

		    printf("Digite o CPF: ");
		    fgets(novoAluno->cpf, 15, stdin); 
		    remover_quebra_linha(novoAluno->cpf);

		    if (!validar_cpf(novoAluno->cpf)){
		    	retorno = ERRO_CADASTRO_CPF;
		    }
	    }

    }

    if (retorno == SUCESSO_CADASTRO){
    	novoAluno->matricula = geraMatricula();
    	inserirAlunoNaLista(inicio, novoAluno);
    	return SUCESSO_CADASTRO;
    }else{
    	//v2 - free no objeto criado
    	free(novoAluno);
    	return retorno;

    }
    
}

//v2
int excluirAlunoNaLista(Aluno** inicio, int matricula){
	if (*inicio == NULL)
		return LISTA_VAZIA; // lista vazia

	Aluno* anterior = *inicio;
	Aluno* atual = *inicio;
	Aluno* proximo = atual->prox;
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
//v2
int excluirAluno(Aluno** inicio, Disciplina* inicioDisciplina){
	int matricula;
	int retorno;

	printf("Digite a matrícula: ");    
    scanf("%d", &matricula);
    getchar();

	retorno = excluirAlunoNaLista(inicio, matricula);

	//v4 - ao excluir o aluno, remove-lo de todas as disciplinas em que
	//estiver matriculado, para nao deixar "matricula fantasma" na Disciplina.
	if (retorno == SUCESSO_EXCLUSAO)
		removerAlunoDeTodasDisciplinas(inicioDisciplina, matricula);

	return retorno;
	
}

//v4
int atualizarAluno(Aluno** inicio){
	int matricula;
	Aluno* aluno;
	int opcao;
	int sair = 0;

	printf("Digite a matrícula do aluno a atualizar: ");
	scanf("%d", &matricula);
	getchar();

	aluno = buscarAlunoPorMatricula(*inicio, matricula);
	if (aluno == NULL)
		return NAO_ENCONTRADO;

	while (!sair){
		printf("\n### Atualizar Aluno (matrícula %d) ###\n", matricula);
		printf("Atual -> Nome: %s | Sexo: %c | Nascimento: %s | CPF: %s\n",
		       aluno->nome, aluno->sexo, aluno->data_nascimento.dataCompleta, aluno->cpf);
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
				fgets(aluno->nome, 50, stdin);
				remover_quebra_linha(aluno->nome);
				break;
			}
			case 2: {
				char sexo;
				printf("Novo sexo (M/F): ");
				scanf("%c", &sexo);
				getchar();
				sexo = toupper(sexo);
				if (sexo == 'M' || sexo == 'F')
					aluno->sexo = sexo;
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
					strcpy(aluno->data_nascimento.dataCompleta, dataStr);
					preencher_campos_data(&aluno->data_nascimento);
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
					strcpy(aluno->cpf, cpf);
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

Aluno* buscarAlunoPorMatricula(Aluno* inicio, int matricula){
	Aluno* atual = inicio;
	while (atual != NULL){
		if (atual->matricula == matricula)
			return atual;
		atual = atual->prox;
	}
	return NULL;
}

void listarAlunos(Aluno** inicio){
    Aluno* alunoAtual = *inicio;
    if (*inicio == NULL){
        printf("Lista Vazia\n");
        
    }else{
    	printf("\n### Alunos Cadastrados ####\n");
        do{
            printf("-----\n");
            printf("Matrícula: %d\n", alunoAtual->matricula);
            printf("Nome: %s\n", alunoAtual->nome);
            printf("Sexo: %c\n", alunoAtual->sexo);
            printf("Data Nascimento: %s\n", alunoAtual->data_nascimento.dataCompleta);
            printf("CPF: %s\n", alunoAtual->cpf);
            
            alunoAtual = alunoAtual->prox;

        }while (alunoAtual != NULL);
    }    
    printf("-----\n\n");
}

//v2 - Liberação da lista de aluno
void liberarListaAluno(Aluno* inicio){

	Aluno* atual = inicio;
	Aluno* tmp;

	while(atual != NULL){
		tmp = atual->prox;
		free(atual);
		atual = tmp;
	}
}
