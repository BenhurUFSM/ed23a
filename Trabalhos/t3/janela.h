#ifndef _JANELA_H_
#define _JANELA_H_

// janela
// ------
// TAD que permite alterar um texto e apresentá-lo na tela

#include "texto.h"

typedef struct _jan *Jan;

// cria uma janela para editar o texto 'txt'
Jan jan_cria(Txt txt);

// libera a memória da janela (mas não destroi o texto)
void jan_destroi(Jan self);

// retorna o texto sendo editado pela janela
Txt jan_texto(Jan self);

// desenha o texto na tela
void jan_desenha_txt(Jan self);

// trata uma tecla digitada pelo usuário
// pode ser um caractere ou uma tecla de controle
void jan_trata_tecla(Jan self, chu tecla);

#endif // _JANELA_H_
