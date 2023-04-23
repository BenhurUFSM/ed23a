#ifndef _TEXTO_H_
#define _TEXTO_H_

#include <stdbool.h>
#include "str.h"

typedef struct _txt *Txt;

Txt txt_cria(void);

void txt_destroi(Txt self);

Txt txt_cria_arq(Str nome);

bool txt_alterado(Txt self);

Str txt_nome(Txt self);

void txt_grava(Txt self);

int txt_nlin(Txt self);

Str txt_linha(Txt self, int lin);

int txt_tamlin(Txt self, int lin);

void txt_insere_char(Txt self, int lin, int col, chu ch);

void txt_remove_char(Txt self, int lin, int col);

void txt_quebra_linha(Txt self, int lin, int col);

#endif // _TEXTO_H_
