#ifndef _JANELA_H_
#define _JANELA_H_

#include "texto.h"

typedef struct _jan *Jan;

Jan jan_cria(Txt txt);

void jan_destroi(Jan self);

Txt jan_texto(Jan self);

int jan_plin_tela(Jan self);

int jan_ulin_tela(Jan self);

int jan_nlin_tela(Jan self);

int jan_pcol_tela(Jan self);

int jan_ucol_tela(Jan self);

int jan_ncol_tela(Jan self);

int jan_clin_tela(Jan self);

int jan_ccol_tela(Jan self);

void jan_texto_para_linha(Jan self, int lin, int n, char s[n]);

void jan_desenha_txt(Jan self);

void jan_trata_tecla(Jan self, chu tecla);

#endif // _JANELA_H_
