#ifndef _TEXTO_H_
#define _TEXTO_H_

#include <stdbool.h>
#include "str.h"

// texto
// -----
// TAD que representa o texto de um arquivo
// mantém uma lista com strings (as linhas do arquivo)
// contém operações para ler e gravar o texto no arquivo
//   e para editar o texto

typedef struct _txt *Txt;

// cria um texto vazio, correspondendo a um arquivo sem nome
Txt txt_cria(void);

// cria um texto a partir do conteúdo de um arquivo
Txt txt_cria_arq(Str nome);

// libera a memória ocupada pelo texto
void txt_destroi(Txt self);

// retorna true se o texto foi alterado desde que foi lido ou gravado
//   no arquivo
bool txt_alterado(Txt self);

// retorna o nome do arquivo correspondente ao texto
Str txt_nome(Txt self);

// grava o texto no arquivo
void txt_grava(Txt self);

// retorna o número de linhas no texto
int txt_nlin(Txt self);

// retorna uma linha do texto (a primeira é 0)
Str txt_linha(Txt self, int lin);

// retorna o número de caracteres em uma linha do texto
int txt_tamlin(Txt self, int lin);

// insere o caractere ch na posição col de uma linha do texto
void txt_insere_char(Txt self, int lin, int col, chu ch);

// remove um caractere na posição col da linha lin do texto
// se col estiver logo após o último caractere da linha e
//   houver outra linha depois, concatena as duas linhas
void txt_remove_char(Txt self, int lin, int col);

// quebra a linha lin do texto, de forma que o caractere antes
//   da posição col passe a ser o último da linha quebrada,
//   e os caracteres a partir da posição col sejam colocados
//   em uma nova linha, que deve ser inserida no texto após lin
// caso col esteja logo após o último caractere da linha, deve
//   ser inserida uma linha em branco
void txt_quebra_linha(Txt self, int lin, int col);

#endif // _TEXTO_H_
