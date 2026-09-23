#ifndef ALUNO_H
#define ALUNO_H


/* Forward declaration: Aluno.h nao precisa conhecer os detalhes da
   struct Disciplina, so o tipo, para poder receber um ponteiro para a
   lista de disciplinas (usado ao excluir um aluno, para desmatricula-lo
   de todas as disciplinas em que estiver). */
struct dados_disciplina;

/*Criando a struct aluno */
typedef struct dados_aluno
{
  int matricula;
  char nome[50];
  char sexo; //M - Masculino, F - Feminino
  Data data_nascimento;
  char cpf[15];
  struct dados_aluno *prox;
    
} Aluno;

void mainAluno(Aluno** inicio, struct dados_disciplina* inicioDisciplina);
void liberarListaAluno(Aluno* inicio); //v2
void listarAlunos(Aluno** inicio);

/* Retorna o ponteiro para o aluno com a matricula informada, ou NULL
   se nao encontrado. Usada pelo modulo de Disciplina. */
Aluno* buscarAlunoPorMatricula(Aluno* inicio, int matricula);

#endif /* ALUNO_H */
