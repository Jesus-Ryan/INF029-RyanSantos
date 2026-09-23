#ifndef UTILS_H
#define UTILS_H

typedef struct dma {
	char dataCompleta[11];
  	int dia;
  	int mes;
	int ano; 
} Data; 


int validar_data(char data[]);

/* Depois que validar_data() retornar 1 (data valida), chame esta funcao
   para preencher os campos dia/mes/ano da struct a partir da
   dataCompleta ja validada. */
void preencher_campos_data(Data *data);

/* Valida um CPF (aceita tanto "12345678901" quanto "123.456.789-01").
   Confere os 11 digitos e os dois digitos verificadores.
   Retorna 1 se valido, 0 caso contrario. */
int validar_cpf(char cpf[]);

/* Remove o '\n' final deixado pelo fgets(), de forma segura mesmo se a
   string estiver vazia (evita indice negativo). */
void remover_quebra_linha(char str[]);

#endif /* UTILS_H */
