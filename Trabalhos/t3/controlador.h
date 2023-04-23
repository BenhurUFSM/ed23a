#ifndef _CTRL_H_
#define _CTRL_H_

// controlador (Ctrl)
//
// controla a edição dos textos
// mantém uma lista com os textos de arquivos abertos
// e uma lista de janelas que podem mostrar/editar esses textos
// processa os caracteres da entrada, repassando-os à janela corrente
// organiza o acesso à tela e sua atualização

#include "str.h"

typedef struct _ctrl *Ctrl;

// construtor e destrutor
Ctrl ctrl_cria(void);
void ctrl_destroi(Ctrl self);

// abre uma janela para edição do conteúdo do arquivo nomeado
// torna essa janela a corrente
void ctrl_abre_nome(Ctrl self, Str nome);

// pede ao usuário um nome de arquivo e abre uma janela para editá-lo
void ctrl_abre(Ctrl self);

// executa o laço principal, que desenha a tela e processa a entrada
//   do usuário
// retorna quando não houver mais janelas com texto a editar
void ctrl_laco_principal(Ctrl self);

#endif // _CTRL_H_
