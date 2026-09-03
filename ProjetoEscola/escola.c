#include <stdio.h>
#define TAMALUNO 3
int main(void) {

    int opcao;
    int lista_aluno[TAMALUNO];
    int qtdAluno = 0;
    int sair = 0;

    while(!sair){
        printf("Projeto Escola\n");
        printf("0 - Sair\n");
        printf("1 - Aluno\n");
        printf("2 - Professor\n");
        printf("3 - Discisplina\n");

        scanf("%d", &opcao);

        switch(opcao){
            case 0:{
                sair = 1;
                break;
            }

            case 1:{
                printf("Módulo Aluno:\n"); 
                int sairAluno = 0;
                int opcaoAluno; 
                
                while(!sairAluno){
                    printf("0 - Sair\n");
                    printf("1 - Cadastrar Aluno\n");
                    printf("2 - Listar Aluno\n");
                    printf("3 - Atualizar Aluno\n");
                    printf("4 - Excluir Aluno\n");
                    scanf("%d", &opcaoAluno);  
                    
                    switch(opcaoAluno){
                        case 0:{
                            sairAluno = 1;
                            break;
                        }                      
                        case 1:{
                            printf("Cadastrar Aluno\n");
                            printf("Digite a matrícula\n");
                            int matricula;
                            scanf("%d", &matricula);
                            
                            if(matricula < 0){
                                printf("Matrícula Inválida:\n");   
                            }

                            else{
                                if(qtdAluno == TAMALUNO){
                                    printf("Lista de Alunos cheia:\n");
                                }
                                else{
                                   lista_aluno[qtdAluno] = matricula;
                                   qtdAluno++; 
                                }
                             }
                            

                           break;
                        }
                        case 2:{
                           printf("Listar Aluno\n");
                           break; 
                        }
                        case 3:{
                           printf("Atualizar Aluno\n");
                           break; 
                        }
                        case 4:{
                           printf("Excluir Aluno\n");
                           break; 
                        }

                    }
                }
            }

            case 2:{
                printf("Módulo Professor:\n");
                break;
            }

            case 3:{
                printf("Módulo Disciplina:\n");
                break;
            }
            default:{
                printf("Opção Inválida\n");
                break;
            }
        }

    }
}