#include "janela.h"
#include "tela.h"
#include <stdlib.h>
#include <limits.h>

struct _jan {
  int curlin;    // linha do texto onde está o cursor
  int curcol;    // coluna do texto onde está o cursor
  int lin_topo;  // linha do texto que está na primeira linha da tela
  int col_esq;   // coluna do texto que está na primeira coluna da tela
  Txt txt;
};

// Funções internas

// qual a primeira linha da tela que pode ser usada pela janela
static int jan_plin_tela(Jan self);

// qual a última linha da tela que pode ser usada pela janela
static int jan_ulin_tela(Jan self);

// quantas linhas de texto podem ser desenhadas na tela
static int jan_nlin_tela(Jan self);

// qual a primeira coluna da tela que pode ser usada pela janela
static int jan_pcol_tela(Jan self);

// qual a última coluna da tela que pode ser usada pela janela
static int jan_ucol_tela(Jan self);

// quantas colunas da tela podem ser usadas pela janela
static int jan_ncol_tela(Jan self);

// em que linha da tela está o cursor
static int jan_clin_tela(Jan self);

// em que coluna da tela está o cursor
static int jan_ccol_tela(Jan self);

// coloca em 's' a string a desenhar na linha 'lin' da tela
// a string deve já estar cortada para caber na tela
static void jan_texto_para_linha(Jan self, int lin, int n, char s[n]);

// ajusta a posição do cursor para que fique dentro do texto,
// e a posição do texto na tela para que o cursor fique na tela
static void jan_ajusta_cursor(Jan self);

// trata um caractere (textual) digitado
static void jan_trata_char(Jan self, chu tecla);

// trata um caractere de controle digitado
static void jan_trata_controle(Jan self, chu tecla);

static int min(int a, int b)
{
  return a<b ? a : b;
}

static int max(int a, int b)
{
  return a>b ? a : b;
}

static int limita(int a, int inf, int sup)
{
  return max(inf, min(a, sup));
}

Jan jan_cria(Txt txt)
{
  Jan self = malloc(sizeof(*self));
  if (self != NULL) {
    self->txt = txt;
    self->curlin = 0;
    self->curcol = 0;
    self->lin_topo = 0;
    self->col_esq = 0;
  }
  return self;
}

void jan_destroi(Jan self)
{
  if (self == NULL) return;
  free(self);
}

Txt jan_texto(Jan self)
{
  return self->txt;
}

void jan_trata_tecla(Jan self, chu tecla)
{
  if (tecla > 0) jan_trata_char(self, tecla);
  else jan_trata_controle(self, tecla);
  // ajusta a posição do cursor para ficar no texto e na tela
  jan_ajusta_cursor(self);
}

void jan_desenha_txt(Jan self)
{
  jan_ajusta_cursor(self);
  // número máximo de bytes para representar uma linha da tela em utf8
  int tam_lin = (jan_ucol_tela(self) - jan_pcol_tela(self)) * 4 + 1;
  char s[tam_lin];
  tela_cor_normal();
  // desenha as linhas do texto na tela
  for (int l = jan_plin_tela(self); l < jan_ulin_tela(self); l++) {
    jan_texto_para_linha(self, l, tam_lin, s);
    tela_lincol(l, jan_pcol_tela(self));
    tela_texto(s);
  }
  // usa a última linha para colocar informação de posição
  tela_lincol(jan_ulin_tela(self), jan_pcol_tela(self));
  tela_cor_selecao();
  tela_textof("L%d/%d C%d", self->curlin+1, txt_nlin(self->txt),
                            self->curcol+1);
  tela_cor_normal();
  // coloca o cursor no lugar dele
  tela_lincol(jan_clin_tela(self), jan_ccol_tela(self));
}




static int jan_plin_tela(Jan self)
{
  // deveria ser informado pelo controlador
  return 1;
}

static int jan_ulin_tela(Jan self)
{
  // deveria ser informado pelo controlador
  return tela_nlin()-2;
}

static int jan_nlin_tela(Jan self)
{
  return jan_ulin_tela(self) - jan_plin_tela(self) + 1;
}

static int jan_pcol_tela(Jan self)
{
  // deveria ser informado pelo controlador
  return 0;
}

static int jan_ucol_tela(Jan self)
{
  // deveria ser informado pelo controlador
  return tela_ncol()-1;
}

static int jan_ncol_tela(Jan self)
{
  return jan_ucol_tela(self) - jan_pcol_tela(self) + 1;
}

static int jan_clin_tela(Jan self)
{
  return self->curlin - self->lin_topo + jan_plin_tela(self);
}

static int jan_ccol_tela(Jan self)
{
  return self->curcol - self->col_esq + jan_pcol_tela(self);
}

static void jan_ajusta_cursor(Jan self)
{
  // limita o cursor a regiões que contenham texto
  self->curlin = limita(self->curlin, 0, txt_nlin(self->txt));
  self->curcol = limita(self->curcol,
                        0, txt_tamlin(self->txt, self->curlin));
  // altera o início do texto na tela para que o cursor fique na tela
  self->lin_topo = limita(self->lin_topo,
                          self->curlin-(jan_nlin_tela(self)-1),
                          self->curlin);
  self->col_esq = limita(self->col_esq,
                         self->curcol-(jan_ncol_tela(self)-1),
                         self->curcol);
}

static void jan_texto_para_linha(Jan self, int nlin_tela, int n, char s[n])
{
  Str linha_txt;
  int nlin_txt = nlin_tela - jan_plin_tela(self) + self->lin_topo;
  linha_txt = txt_linha(self->txt, nlin_txt);
  if (linha_txt == NULL) {
    s[0] = '\0';
    return;
  }
  // quantos caracteres cabem em uma linha da tela
  int nchar = jan_ucol_tela(self)-jan_pcol_tela(self);
  // corta a linha do texto, pega só a substring que aparece na tela
  Str sub = str_substr(linha_txt, self->col_esq, nchar);
  str_cstring(sub, s);
  str_destroi(sub);
}

// trata um caractere digitado
// o editor está sempre em modo de inserção, então
// insere o caractere na posição do cursor do texto
// (poderia suportar substituição também)
static void jan_trata_char(Jan self, chu tecla)
{
  txt_insere_char(self->txt, self->curlin, self->curcol, tecla);
  self->curcol++;
}

// foi digitado um enter, quebra a linha
static void jan_enter(Jan self)
{
  txt_quebra_linha(self->txt, self->curlin, self->curcol);
  self->curlin++;
  self->curcol = 0;
}

// foi digitado del, apaga o caractere no cursor
static void jan_delete(Jan self)
{
  txt_remove_char(self->txt, self->curlin, self->curcol);
}

// foi digitado backspace, volta um caractere e faz de conta que foi del
static void jan_backspace(Jan self)
{
  if (self->curcol > 0) {
    self->curcol--;
  } else {
    if (self->curlin > 0) {
      self->curlin--;
      self->curcol = txt_tamlin(self->txt, self->curlin);
    } else {
      return;
    }
  }
  jan_delete(self);
}

// trata um caractere de controle digitado
static void jan_trata_controle(Jan self, chu tecla)
{
  switch (tecla) {
    case c_up:
      self->curlin--;
      break;
    case c_down:
      self->curlin++;
      break;
    case c_left:
      self->curcol--;
      break;
    case c_right:
      self->curcol++;
      break;
    case c_enter:
      jan_enter(self);
      break;
    case c_home:
      self->curcol = 0;
      break;
    case c_end:
      self->curcol = INT_MAX;
      break;
    case c_pgdn:
      self->curlin += jan_nlin_tela(self) * 4/5;
      break;
    case c_pgup:
      self->curlin -= jan_nlin_tela(self) * 4/5;
      break;
    case c_back:
      jan_backspace(self);
      break;
    case c_del:
      jan_delete(self);
      break;
    default:
      break;
  }
}
