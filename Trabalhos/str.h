// str.h
// -----
// interface para o TAD Str (para a disciplina ed23a)
//
// alterações:
//    3abr inclusão de stdio.h
//         str_altera deveria ser void
//    3abr adição de str_numbytes, para parte II

// não altere este arquivo

// evita inclusões múltiplas
#ifndef _STR_H_
#define _STR_H_

#include <stdbool.h>
#include <stdio.h>

// o tipo de dados opaco (Str é um ponteiro para uma struct desconhecida chamada _str)
typedef struct _str *Str;

// construtores

// cria uma Str a partir de uma string C codificada em UTF8
// ela deve ser destruída pelo usuário mais tarde
Str str_cria(char *s);

// cria uma Str a partir de uma linha do arquivo
// deve conter todos os caracteres lidos do arquivo, antes de um '\n'
//   ou do final do arquivo
// retorna NULL caso o arquivo esteja no final ou em caso de erro no acesso
// o arquivo está codificado em UTF8
// a Str retornada deve ser destruída pelo usuário mais tarde
Str str_cria_linha(FILE *arq);

// destrutor

// libera os recursos (memória) ocupados pela string
void str_destroi(Str s);

// 

// retorna o número de caracteres unicode na string (equivalente a strlen)
int str_tam(Str s);

// retorna o número de bytes necessários para representar a string em UTF8
int str_numbytes(Str s);

// retorna o i-ésimo caractere unicode de 's' ou -1 se 'i' além dos limites
// valores negativos de 'i' referem-se ao final de 's' (-1 é o último caractere,
//   -2 o penúltimo, etc)
int str_char(Str s, int i);

// retorna uma substring de 's', com os 'n' caracteres a partir da posição 'p'
// a string retornada pode ter menos de 'n' caracteres se 's' não tiver
//   caracteres suficientes
// a string retornada pode ser vazia, se 'n' < 1 ou se 'p' fora de 's'
// valores negativos de 'p' referem-se ao final de 's'
//   (-1 é logo após o final de 's', -2 logo antes do último caractere, etc.)
// posição e tamanho referem-se a caracteres unicode
// a Str retornada deve ser destruída pelo usuário mais tarde
Str str_substr(Str s, int p, int n);

// retorna a posição da primeira ocorrência do caractere 'c' em 's' ou -1
int str_poschar(Str s, int c);

// retorna 'true' se 's' e 'o' forem iguais
bool str_igual(Str s, Str o);

// altera 's', substituindo os 'n' caracteres a partir de 'p' pelo conteúdo de 'o'
// valores negativos de 'n' são tratados como 0
// se 'p' além do final de 's', deve ser tratado como logo após o final
// se 'p' antes do início de 's', deve ser tratado cono logo antes do início
// valores negativos de 'p' referem-se ao final de 's'
//   (-1 é logo após o final de 's', -2 logo antes do último caractere, etc.)
// posição e tamanho referem-se a caracteres unicode
void str_altera(Str s, int p, int n, Str o);

#ifdef TESTE
// testa o TAD
void str_teste(void);
#endif
#endif // _STR_H_
