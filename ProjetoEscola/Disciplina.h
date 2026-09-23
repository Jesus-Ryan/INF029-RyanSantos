#ifndef DISCIPLINA_H
#define DISCIPLINA_H

struct dados_aluno;
struct dados_professor;

/* Sublista: matriculas dos alunos inscritos em uma disciplina. */
typedef struct dados_matricula
{
	int matriculaAluno;
	struct dados_matricula *prox;
} Matricula;

typedef struct dados_disciplina
{
	char codigo[10];
	char nome[50];
	int semestre;
	int matriculaProfessor;
	Matricula *alunosMatriculados; /* inicio da sublista de matriculas */
	int totalAlunosMatriculados;
	struct dados_disciplina *prox;
} Disciplina;

void mainDisciplina(Disciplina** inicio, struct dados_aluno* inicioAluno,
                     struct dados_professor* inicioProfessor);
void liberarListaDisciplina(Disciplina* inicio);

/* Lista as disciplinas cadastradas (sem os alunos matriculados). */
void listarDisciplinas(Disciplina** inicio, struct dados_professor* inicioProfessor);

/* Retorna o ponteiro para a disciplina com o codigo informado, ou NULL
   se nao encontrada. */
Disciplina* buscarDisciplinaPorCodigo(Disciplina* inicio, char codigo[]);

/* Retorna 1 se existir alguma disciplina vinculada a matricula de
   professor informada; usada por Professor.c para impedir a exclusao
   de um professor que ainda leciona alguma disciplina. */
int existeDisciplinaComProfessor(Disciplina* inicio, int matriculaProfessor);

/* Usada por Aluno.c ao excluir um aluno: remove esse aluno de todas as
   disciplinas em que estiver matriculado. */
void removerAlunoDeTodasDisciplinas(Disciplina* inicio, int matriculaAluno);

#endif /* DISCIPLINA_H */
