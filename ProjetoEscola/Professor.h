#ifndef PROFESSOR_H
#define PROFESSOR_H


struct dados_disciplina;

/*Criando a struct professor */
typedef struct dados_professor
{
  int matricula;
  char nome[50];
  char sexo; //M - Masculino, F - Feminino
  Data data_nascimento;
  char cpf[15];
  struct dados_professor *prox;

} Professor;

void mainProfessor(Professor** inicio, struct dados_disciplina* inicioDisciplina);
void liberarListaProfessor(Professor* inicio);
void listarProfessores(Professor** inicio);

/* Retorna o ponteiro para o professor com a matricula informada, ou
   NULL se nao encontrado. Usada pelo modulo de Disciplina. */
Professor* buscarProfessorPorMatricula(Professor* inicio, int matricula);

#endif /* PROFESSOR_H */
